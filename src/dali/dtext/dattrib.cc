/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dattrib.cc,v 1.2 2002/03/18 19:43:12 hardaiz Exp $
*
* DESCRIPTION: Implementation of DaliAttributeZone class
*********************************************************************/

#include <dtext.h>

#define	INIT_ENTRIES		20		// initial array size

typedef DaliAttributeBlock* pAtrBlk;

static Int f_cmp(const char *l, const char *r)
{
	return pAtrBlk(l)->start.cmp((*(pAtrBlk*)r)->start);
}

void DaliAttributeZones::add(DaliAttributeBlock &ae) 
{
	idCheck();
	Int j = locate((const char*)&ae, (const char*)b_arr,
		UnInt(nelem), sizeof(pAtrBlk), (IFPCPCP)f_cmp);
	if (j < 0) {
		j = ~j;
		if (++nelem >= dim)
			reAlloc(dim + 10);
		for (Int k = nelem; k > j; k--)
			b_arr[k] = b_arr[k - 1];
	}
	b_arr[j] = tr_new DaliAttributeBlock(ae);
}

void DaliAttributeZones::del(Int n) 
{
	idCheck();
	nelem--;
	tr_delete(b_arr[n]);
	for (Int j = n; j < nelem; j++)
		b_arr[j] = b_arr[j + 1];
}

Int DaliAttributeZones::find(const DaliAttributeBlock &ae) 
{
	idCheck();
	return locate((const char*)&ae, (const char*)b_arr,
		UnInt(nelem), sizeof(pAtrBlk), (IFPCPCP)f_cmp);
}

void DaliAttributeZones::reAlloc(Int n)
{
	idCheck();
	if ( n < dim) 
		return;

	pAtrBlk *n_cont = tr_new pAtrBlk[n];

	for (Int j = 0; j < nelem ; j++)
		n_cont[j] = b_arr[j];
	tr_delete(b_arr);
	dim = n;
	b_arr = n_cont;
}

DaliAttributeZones::DaliAttributeZones()
{
	idStart();
	valid_last = false;
	nelem = 0;
	dim = INIT_ENTRIES;
	b_arr = tr_new pAtrBlk[dim];
}

DaliAttributeZones::~DaliAttributeZones()
{
	for (Int i = 0; i < nelem; i++)
		tr_delete(b_arr[i]);
	tr_delete_vec(b_arr);    
	idStop();
}

// test if TextCursor tc is bounded by s && e

static bool inZone(DaliTextCursor &tc, DaliTextCursor &s, DaliTextCursor &e)
{
	return 	(s && e && tc >= s && tc <= e);
}

Int DaliAttributeZones::getAttribute(DaliTextCursor &tc, Int &lc)
{
	idCheck();
	if (!tc || !nelem) {
		lc = INT_MAX;
		return 0;
	}

	// If cursors are not valid or we're not bounded by them recalc

	if (!valid_last || !inZone(tc, start, end)) {
		Int j;

		// flag we have a valid last accessed zone

		valid_last = true;

		DaliAttributeBlock ab(tc);
		if ((j = find(ab)) < 0)
			j = ~j - 1;

		if (j < 0) {
			j = 0;

			// update last accessed zone

			start = b_arr[j]->start;
			start.goToFirst();
			end   = b_arr[j]->start;
			end.moveCol(-1);
			attribute = 0;

			// up to what column is the attribute valid

			if (tc.order() == end.order())
				lc = end.column();
			else
				lc = INT_MAX;

			return attribute;
		}

		// see if we fall inside the found block

		if (inZone(tc, b_arr[j]->start, b_arr[j]->end)) {

			// update last accessed zone

			start = b_arr[j]->start;
			end = b_arr[j]->end;
			attribute = b_arr[j]->attribute;

			// up to what column is the attribute valid

			if (tc.order() == end.order())
				lc = end.column();
			else
				lc = INT_MAX;

			return attribute;
		}

		// we're not there so signal start && end to the block
		// between the end of b_arr[j] && b_arr[j + 1]

		start = b_arr[j]->end;
		start.moveCol(1);		// add one to the column

		// if (start.column() >= start.value().length())
		//		start.goTo(start.order()+1, 0);

		attribute = 0;

		if (j+1 < nelem) {		// next entry exists
			end = b_arr[j + 1]->start;
			if (end.column())
				end.moveCol(-1);
			else {
				assert(end.order()-1 >= 0);
				end.goTo(end.order()-1, INT_MAX);
			}
		}
		else {		// next entry does not exist, go to end of text
			end = start;
			end.goToLast();
			end.goToCol(INT_MAX);
		}

		// up to what column is the attribute valid
		if (tc.order() == end.order())
			lc = end.column();
		else
			lc = INT_MAX;

		return attribute;
	}
	else {
		// up to what column is the attribute valid

		if (tc.order() == end.order())
			lc = end.column();
		else
			lc = INT_MAX;

		return attribute;
	}
}

bool DaliAttributeZones::setAttribute(DaliTextCursor s, DaliTextCursor &e, 
								  Int attr)
{
	idCheck();
	// el rango esta mal
	if (s > e) return false;

	// flag we don't have a valid last accesed zone anymore

	valid_last = false;

	DaliAttributeBlock a_blk(s);
	Int pos;

	if ((pos = find(a_blk)) < 0)
		if ((pos = ~pos - 1) == -1)
			pos = 0;
		
	for (; pos < nelem; pos++) {

		// start before

		if (s < b_arr[pos]->start) {
			a_blk.start = s;

			if (e < b_arr[pos]->start) break;

			// Add first chunk and continue
			s = b_arr[pos]->start;
			a_blk.end = s; a_blk.end.moveCol(-1);
			a_blk.attribute = attr;
			add(a_blk);
			continue;
		}

		if (s > b_arr[pos]->end)	// starts after
			continue;

		// starts inside

		if (s > b_arr[pos]->start) {
			a_blk = *b_arr[pos];
			b_arr[pos]->start = s;

			a_blk.end = s; a_blk.end.moveCol(-1);
			add(a_blk);
			continue;
		}

		// Start together

		// Ends before

		if (e < b_arr[pos]->end) {
			b_arr[pos]->start = e; 
			b_arr[pos]->start.moveCol(1);
			if (b_arr[pos]->start.column() >= 
				b_arr[pos]->start.value().length())
				b_arr[pos]->start.goTo(b_arr[pos]->start.order()+1, 0);
			attr |= b_arr[pos]->attribute;
			break;
		}

		b_arr[pos]->attribute |= attr;

		// End together
		if (e == b_arr[pos]->end)
			return true;

		s = b_arr[pos]->end;
		s.moveCol(1);
		if (s.column() >= s.value().length())
			s.goTo(s.order()+1, 0);
	}

	a_blk.start = s;
	a_blk.end = e;
	a_blk.attribute = attr;
	add(a_blk);

	return true;
}

bool DaliAttributeZones::delAttribute(DaliTextCursor s, DaliTextCursor &e, 
								  Int attr)
{
	idCheck();
	// el rango esta mal
	if (s > e) return false;

	Int pos;

	// la zona ya no es mas valida porque puedo modificarla
	valid_last = false;

	// bloques utilizados para dividir bloques
	DaliAttributeBlock blk0, blk1;

	// veo donde deberia caer este bloque
	DaliAttributeBlock ab(s);
	if ((pos = find(ab)) < 0)
		if ((pos = ~pos - 1) == -1)
			pos = 0;

	// mientras tenga elementos 
	while(pos < nelem) {

		// este bloque comienza antes del encontrado
		if (s < b_arr[pos]->start) {

			// si no estoy dentro entonces me voy
			if (e < b_arr[pos]->start) break;

			// ajusto al comienzo del bloque
			s = b_arr[pos]->start;

			continue;
		}

		// estoy en el medio del bloque encontrado
		if (s > b_arr[pos]->start) {

			if (s <= b_arr[pos]->end) {

				// nuevos bloques

				blk0.attribute = b_arr[pos]->attribute;
				blk0.start 	   = b_arr[pos]->start;
				blk0.end   	   = s;
				blk0.end.moveCol(-1);
				
				blk1.attribute = b_arr[pos]->attribute;
				blk1.start 	   = s;
				blk1.end   	   = b_arr[pos]->end;

				// borro el bloque y lo divido en dos
				del(pos);

				// inserto los bloques nuevos
				add(blk0);
				add(blk1);
			}

			pos++;

			continue;
		}


		// comienza igual al encontrado
		if (s == b_arr[pos]->start) {
			
			// ahora analizo el final del bloque

			// el fin de este bloque es < que el encontrado
			if (e < b_arr[pos]->end) {
				
				// nuevos bloques

				blk0.attribute = b_arr[pos]->attribute & ~attr;
				blk0.start 	   = b_arr[pos]->start;
				blk0.end   	   = e;
				
				blk1.attribute = b_arr[pos]->attribute;
				blk1.start 	   = e;
				blk1.start.moveCol(1);
				if (blk1.start.column() >= blk1.start.value().length())
					blk1.start.goTo(blk1.start.order()+1, 0);
				blk1.end   	   = b_arr[pos]->end;

				// borro el bloque y lo divido en dos
				del(pos);

				// inserto los bloques nuevos
				if (blk0.attribute) add(blk0);
				add(blk1);

				// uff, termine !!!
				break;
			}		

			// terminan en el mismo lugar
			if (e == b_arr[pos]->end) {

				// si el bloque no tiene atributo lo borro
				blk0 = *b_arr[pos];
				blk0.attribute &= ~attr;
				del(pos);
				if (blk0.attribute) add(blk0);

				// uff, termine !!!
				break;
			}

			// este bloque esta pasando el final del corriente
			if (e > b_arr[pos]->end) {

				s = b_arr[pos]->end;
				s.moveCol(1);
				if (s.column() >= s.value().length())
					s.goTo(s.order()+1, 0);

				// si el bloque no tiene atributo lo borro
				blk0 = *b_arr[pos];
				blk0.attribute &= ~attr;
				del(pos);
				if (blk0.attribute) {
					add(blk0);
					pos++;
				}
				continue;
			}
		}
	}
	return true;
}

bool DaliAttributeZones::delAttribute(DaliTextCursor &, Int )
{
	idCheck();
	return false;
}

#include <local/strfile.h>

void DaliAttributeZones::debug()
{
#ifdef ISDEBUG
	idCheck();
	stdOut << "Table limit : " << nelem << endl;

	for (Int j = 0; j < nelem; j++) {
		stdOut << b_arr[j]->start.order() << "    ";
		stdOut << b_arr[j]->start.column() << "    ";
		stdOut << b_arr[j]->end.order() << "    ";
		stdOut << b_arr[j]->end.column() << "    ";
		stdOut << b_arr[j]->attribute << "    " << endl;
	}
	stdOut << endl << endl;
#endif
}


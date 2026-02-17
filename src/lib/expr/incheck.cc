/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: incheck.cc,v 1.4 1999/02/08 22:27:27 eduardoc Exp $
*
* DESCRIPTION
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound/parray.h>
#include <essentia/connect.h>
#include <essentia/incheck.h>
#include <ifound/str.h>

declare(PtrArray,AnyValue);
declare(PtrArrayCursor,AnyValue);

// InCheck class

InCheck::InCheck(bool negated)
:	neg(negated),
	keyValues(*(tr_new PtrArray(AnyValue)(0))),
	dspFields(*(tr_new PtrArray(AnyValue)(0)))
{
}

InCheck::InCheck(Int nk, Int nd, bool negated)
:	neg(negated),
	keyValues(*(tr_new PtrArray(AnyValue)(nk))),
	dspFields(*(tr_new PtrArray(AnyValue)(nd)))
{ }

InCheck::InCheck()
:	neg(BOOL_NULL),
	keyValues(*(tr_new PtrArray(AnyValue)(0))),
	dspFields(*(tr_new PtrArray(AnyValue)(0)))
{ }

InCheck::~InCheck()
{
	AnyValue *tmpv;
	PtrArrayCursor(AnyValue) pkc;

	for (pkc = keyValues; pkc; ++pkc)
		if ((tmpv = *pkc))
			tr_delete(tmpv);

	PtrArray(AnyValue) *pval = &keyValues;
	tr_delete(pval);

	for (pkc = dspFields; pkc; ++pkc)
		if ((tmpv = *pkc))
			tr_delete(tmpv);

	pval = &dspFields;
	tr_delete(pval);
}

InCheckType InCheck::getType() const
{
	return IC_EMPTY;
}

void InCheck::addDspFld(const AnyValue &v)
{
	dspFields.add(tr_new AnyValue(v));
}

void InCheck::addKeyVal(const AnyValue &v)
{
	keyValues.add(tr_new AnyValue(v));
}

void InCheck::setNValues(Int n)
{
	keyValues.reAlloc(n);
}

Int InCheck::nDspFields() const
{
	return dspFields.dim();
}

AnyValue &InCheck::dspField(Int i) const
{
	return *dspFields.elem(i);
}

Int InCheck::nKeyValues() const
{
	return keyValues.dim();
}

AnyValue &InCheck::keyValue(Int i) const
{
	return *keyValues.elem(i);
}

// end of InCheck class

// Integrity class

Integrity::Integrity()
:	InCheck()
{ }

Integrity::Integrity(bool negated, const ObjectId &indid)
:	InCheck(negated), ind(indid)
{ }

Integrity::Integrity(const ObjectId &indid, Int nk, Int nd,
		bool negated)
:	InCheck(nk, nd, negated), ind(indid)
{ }

Integrity::~Integrity()
{ }

InCheckType Integrity::getType() const
{
	return IC_INTEGRITY;
}

String Integrity::tabName() const
{
	return "tabName";
}

String Integrity::indName() const
{
	return "indName";
}

// end of Integrity class

// InList class

InCheckType InList::getType() const
{
	return IC_INLIST;
}

Int InList::maxDspLen() const
{
	Int i,
		l,
		maximum = 0,
		nf = dspFields.dim();

	for (i = 0; i < nf; i++) {
		if (dspFields.elem(i)) {
			l = toString(*dspFields.elem(i)).length();
			if (l > maximum)
				maximum = l;
		}
	}
	return maximum;
}

InList::InList()
:	InCheck()
{ }

InList::InList(bool negated)
:	InCheck(negated)
{ }

InList::InList(Int nk, Int nd, bool negated)
:	InCheck(nk, nd, negated)
{ }

InList::~InList()
{ }

// end of InList class

void InCheck::print() const
{
	Int i;
	if (getType() == IC_EMPTY) {
		fprintf(stderr, "Empty\n");
		return;
	}
	fprintf(stderr, "Negation: %d\n", int(neg));
	if (getType() == IC_INTEGRITY) {
		fprintf(stderr, "INTEGRITY\n");
//		stdErr << ind.classId() << ", " << ind.instId() << endl;
//		stdErr << "Table: " << tabName() << endl;
//		stdErr << "Index: " << indName() << endl;
	} else
		fprintf(stderr, "INLIST\n");

	fprintf(stderr, "%ld key values...\n", keyValues.dim());
	for (i = 0; i < keyValues.dim(); i++)
		keyValues.elem(i)->print();
	fprintf(stderr, "%ld dsp fields...\n", dspFields.dim());
	for (i = 0; i < dspFields.dim(); i++)
		dspFields.elem(i)->print();
}

String InCheck::dumpInCheck() const
{
	String ic = NULL_STRING;

	Int i;
	if (getType() == IC_EMPTY) {
		ic = String("Empty\n");
		return ic;
	}

	ic = String("Negation: ") + toString(int(neg));
	if (getType() == IC_INTEGRITY) {
		ic += String("INTEGRITY\n");
	}
	else {
		ic += String("INLIST\n");
	}

	ic += toString(keyValues.dim()) + " key values...\n";
	ic += "(";
	for (i = 0; i < keyValues.dim(); i++) {
		ic += toString(*(keyValues.elem(i)));
		if (i+1 < keyValues.dim()) {
			ic += ", ";
		}
	}
	ic += ")\n";

	ic += toString(dspFields.dim()) + " dsp fields...\n";
 	ic += "(";
	for (i = 0; i < dspFields.dim(); i++) {
		ic += toString(*(dspFields.elem(i)));
		if (i+1 < dspFields.dim()) {
			ic += ", ";
		}
	}
	ic += ")";

	return ic;
}

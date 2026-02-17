/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strhis.cc,v 1.1.1.1 1996/04/16 18:52:46 gustavof Exp $
*
* DESCRIPTION: Implementation of String history class
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/stdc.h>
#include <winapp/strhis.h>
#include <local/strfile.h>
#include <winapp/menu.h>

StrHistory::StrHistory(Int hs)
{           
	idStart();
	
	if ((hisSize = hs) == 0)
		history = NULL;
	else
		history = tr_new String[hs];	
	fullslots_ = 0;
}

StrHistory::~StrHistory()
{   
	idCheck();

	if (history)
		tr_delete_vec(history);
	
	idStop();
}

void StrHistory::execute(String &datum)
{
	idCheck();

	if (hisSize) {
		StringMenu sm(NULL_STRING, history, hisSize); 
		Int n = ERR;
		if(sm.execute() != REJECT_EXIT)
			n = sm.currOpt();

		if (n != ERR)	datum = history[n];
	}
}

void StrHistory::operator<<(const String &datum)
{
	idCheck();
	String hisaux, dataux;
	bool newquery = true;
	
	register Int n;
    dataux = datum;
    dataux.prune();
	
	if (datum != NULL_VALUE && hisSize) {
		for (n = 0; n < hisSize; n++)
			if ((hisaux = history[n]).prune() == dataux) {
				newquery = false;
				break;
			}
		
		if (n == hisSize) n--;

		while (n--)
			history[n+1] = history[n];

		if (newquery && fullslots_ < hisSize) 
			fullslots_++;

		history[0] = datum;
	}

}

void StrHistory::load(const char *&p)
{
	idCheck();

	if (history) {
		tr_delete_vec(history);
		history = NULL;
	}

	hisSize = ldlong(p);
	history = tr_new String[hisSize];	
	p += sizeof(Int);
	Int i;
	for (i = 0; i < hisSize; i++)
		history[i].load(p);
}

void StrHistory::store(char *&p) const
{
	idCheck();

	Int i;
	stlong(hisSize, p);
	p += sizeof(Int);
	for (i = 0; i < hisSize; i++)
		history[i].store(p);
}

void StrHistory::load(InputStream &stream)
{
	idCheck();

	Int i, n = 0;
	String str;
	stream >> str;
	if (str) {
		n = toInt(str);
		if (n > 0) {
			if (hisSize != n) {
				if (history)
					tr_delete_vec(history);
				hisSize = n;
				history = tr_new String[hisSize];
			}

			for (i = 0; i < hisSize; i++) {
				String tmp;
				stream >> tmp;
				history[i] = tmp.replace(String(1,char(1)),"\n");
				if (history[i] != NULL_STRING)
					fullslots_++;
			}				
		}
	}
}

void StrHistory::store(OutputStream &stream) const
{
	idCheck();

	Int i;
	stream << toString(hisSize) << "\n";
	for (i = 0; i < hisSize; i++)
		stream << history[i].replace("\n",String(1,char(1))) << "\n";
}                 

void ExtStrHistory::execute(String &datum)
{
	idCheck();

	if (hisSize) {                            		  
//usar tabla de mensajes
//5
//habria que recibir width!!
		ExtStringMenu querydesc("Query Selector", history, hisSize, 
									inforows_, 19, 20, 50);
		Int n = ERR, result = ERR;

		querydesc.display();
		while ((result = querydesc.execute()) != REJECT_EXIT ) {
			if (querydesc.lastKey() != K_TAB && result == ACCEPT)
				break;
			else if (querydesc.lastKey() == K_TAB) {
				querydesc.dispinfo();
			}
		}

		if(result != REJECT_EXIT)
			n = querydesc.currOpt();

		if (n != ERR)	{
			datum = history[n];
			currpos_ = n;
		}
	}
}

void ExtStrHistory::operator<<(const String &datum)
{
	idCheck();

	StrHistory::operator<<(datum);
	currpos_ = ERR;
}

const String& ExtStrHistory::operator -- (void)
{
	idCheck();

	if (currpos_ == (hisSize - 1))	
		return NULL_STRING;

	return history[++currpos_] == NULL_STRING 
			? --currpos_, NULL_STRING 
			: history[currpos_];
}

const String& ExtStrHistory::operator ++ (void)
{
	idCheck();

	if (currpos_ == 0)
		currpos_--;

	if (currpos_ < 0)
		return NULL_STRING;

	assert(currpos_ > 0);

	return history[--currpos_];
}


                     

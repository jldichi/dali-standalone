/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: help.cc,v 1.3 1998/11/19 15:08:16 ernestod Exp $
*
* DESCRIPTION: Help facility routines
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound/msgtable.h>
#include <winapp/textview.h>
#include <winapp/menu.h>
#include <ifound/list.h>
#include <help.h>

declare(List, String);
declare(ListCursor, String);

// Help Class

Help *Help::activeHelp = NULL;

Help::Help()
:	text(*tr_new List(String)()),
	tc(*tr_new ListCursor(String)())
{
	msgTable = tr_new MsgTable("app", "main", true);
}

Help::Help(const String &module)
:	text(*tr_new List(String)()),
	tc(*tr_new ListCursor(String)())
{
	msgTable = tr_new MsgTable(module, "main", true);
}

Help::~Help()
{
	tr_delete(msgTable);

	// Delete cursor BEFORE deleting text!
	ListCursor(String) *plcs = &tc;
	tr_delete(plcs);

	List(String) *pls = &text;
	tr_delete(pls);
}

String const Help::fpReadMsg(Int n)
{
	return HIGH_VALUE;
#if 0
	static Int prev = ERR;
	Int count = n;

	if (n < 0)
		return HIGH_VALUE;

	if (prev != ERR && prev == n - 1)
		activeHelp->msgCursor->next();
	else {
		activeHelp->msgCursor->first();
		while (count--)
			activeHelp->msgCursor->next();
	}
	prev = n;
	if (*(activeHelp->msgCursor) && activeHelp->msgCursor->key() !=
			NULL_STRING)
		return activeHelp->msgCursor->key();
	else
		return HIGH_VALUE;
#endif
}

bool Help::display(const String key) 
{
	// Save previous active help
	Help *savePrev = activeHelp;
	activeHelp = this;

	bool retVal = displayMsg(key);

	// Restore the active help
	activeHelp = savePrev;
	return retVal;
}

bool Help::display(Int item) 
{
	String key;

	// Save previous active help
	Help *savePrev = activeHelp;
	activeHelp = this;

	Int retVal = 0;
	// Should we display a help index?
	if (item == ERR) {
		retVal = ERR;
#ifndef __VC__
		ScrollMenu sm(&Help::fpReadMsg, 10, 35);
#else
		ScrollMenu sm((const String(__cdecl *)(long))&Help::fpReadMsg, 10, 35);
#endif
		if (sm.execute() != REJECT_EXIT) {
			retVal = sm.currOpt();
			key = fpReadMsg(retVal);      
		}                           
	} else
		key = getIndex(item);

	bool retCode = false;

	if (retVal != ERR)
		retCode = displayMsg(key);

	// Restore the active help
	activeHelp = savePrev;

	return retCode;
}

bool Help::displayMsg(const String &key)
{
	String contents;
	bool retVal = false;

	if ((contents = msgTable->find(key)) != HIGH_VALUE) {

		// Build a String list from the string (\n's are separators)
		Int nLine = -1, pNewLine = -1, lastLen;

		text.delAll();
		while ((nLine = contents.index('\n', nLine+1)) != ERR) {
			if (nLine-pNewLine-1)
				text.add(contents(pNewLine+1, nLine-pNewLine-1));
			else
				text.add(NULL_STRING);
			pNewLine = nLine;
		}
		if (lastLen = contents.length()-pNewLine-1)
			text.add(contents(pNewLine+1, lastLen));

		tc = text;

#ifndef __VC__
		TextView tv(&Help::fpRead, key);
#else
		TextView tv((const String (__cdecl *)(long))&Help::fpRead, key);
#endif
		tv.execute();
		retVal = true;
	}
	return retVal;
}

String const Help::fpRead(Int n)
{
	activeHelp->tc.goTo(n-1);

	if (activeHelp->tc)
		return activeHelp->tc.value();

	return HIGH_VALUE;
}

String Help::getIndex(Int index) const
{
	return toCharPtr(index);
}
// End of Help Class

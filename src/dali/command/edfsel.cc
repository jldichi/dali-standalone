/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: edfsel.cc,v 1.2 1997/11/18 18:29:55 guiller Exp $
*
* DESCRIPTION: Implementation of Editor File Selector
*********************************************************************/

#ifdef __GNUG__
#	pragma implementation
#endif

#include <edfsel.h>
#include <winapp/msgbox.h>
#include <local/dbrowser.h>
#include <ifound/rexp.h>

EdFileSelector::EdFileSelector(String lbl, Int nf, Int nc, Int fo,
	Int co, StrHistory *his, const String &cwd,const String &schpath)
#ifndef __NT__
:	FileSelector(lbl, nf, nc, fo, co, cwd, "*", schpath, his),
#else
:	FileSelector(lbl, nf, nc, fo, co, cwd, "*.*", schpath, his),
#endif
	strHis(his), fsPanel(false)
{
	idCheck();
}

EdFileSelector::~EdFileSelector() 
{
	idCheck();
}

static String expandFiles(const String &str)
{
	String fils;
	for (StrCursor sc(str, " "); sc; sc++) {
		
		// This shouldn't occur, but an assert seems to be to strong.
		if (sc.value().isNull())
			continue;
			
		LRegExp re(String(sc.value()).baseFullName());
		if (!re) {
			MsgBox invRegExp(libMsg("ERROR"), 
					Editor::editorMsg("INVREGEXP"), 5, 32, 6, 20);
			(void) invRegExp.execute();
			return NULL_STRING;
		}

		if (re.hasMetaChars()) {
			String d = String(sc.value()).pathName();
			DirBrowser db(d, &re);
			if (db) {
				for (; db; db++) {
					if (fils) fils += " ";
					if (d) fils += d + PATH_SLASH;
					fils += db.name();
				}
			}	
		} else {
			if (fils) fils += " ";
			fils += String(sc.value());
		}	
	} 
	return fils;
}

Int EdFileSelector::execute(UnChar) 
{                  
	idCheck();

	if (!active)
		return REJECT_EXIT;
		
	if (Editor::isCmdLine() && !fsPanel) {
		Int ch = Editor::getCmdLine(label + ": ", strres, strHis);
		if (ch != K_TAB) {

			strres = strres.expandEnvVars();
			strres.prune();
			strres = expandFiles(strres);

			if (ch == ERR || strres == NULL_STRING) {
				accepted_ = false;
				return REJECT_EXIT;                    
			}				

			fsPanel = false;
			accepted_ = (checkPerms() == OK);
			return ACCEPT_EXIT;
		}
	}   
	fsPanel = true;
	return FileSelector::execute();
}

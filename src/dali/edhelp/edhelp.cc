/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: edhelp.cc,v 1.1.1.1 1996/04/16 18:52:49 gustavof Exp $
*
* DESCRIPTION: Editor help facility routines
*********************************************************************/

#ifdef USES_OLD_HELP_MENU_STYLE 
#ifdef __GNUC__
#pragma implementation
#endif
#include <editor.h>
#include <edhelp.h>
#include <dsensor.h>

String EditorHelp::getIndex(Int index) const
{
	String tmp = Editor::sen->getCmdName(index);
	return Editor::editorMsg(toCharPtr(tmp));
}

#endif

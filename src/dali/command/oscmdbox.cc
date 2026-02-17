/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: oscmdbox.cc,v 1.1.1.1 1996/04/16 18:52:50 gustavof Exp $
*
* DESCRIPTION: Implementation of execute OS's panel
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <oscmdbox.h>
#include <ifound/str.h>
#include <editor.h>
#include <winapp/label.h>
#include <winapp/cell.h>

declare(PtrList,DaliView);
declare(PtrListCursor,DaliView);

#define EXECOSCMD_ROWS		3
#define EXECOSCMD_COLS		60
#define EXECOSCMD_ORG_ROW	5
#define EXECOSCMD_ORG_COL	10

ExecOSCmdDlog::ExecOSCmdDlog(StrHistory *his) 
: Panel(Editor::editorMsg("ExecOSCommand"), EXECOSCMD_ROWS,
		EXECOSCMD_COLS, EXECOSCMD_ORG_ROW, EXECOSCMD_ORG_COL)
{
	// inicializacion de los labels
	staticText = tr_new Label(Editor::editorMsg("COMMAND"), 1, 2); 

	// inicializacion de los objetos
	cmdFld = tr_new StrCell(128, 45, 0, 1, 12, his);

	// inicializacion del panel
	InitObjs((WinObj**) &cmdFld, 1); 
	InitLabels(&staticText, 1);

	// creo el string para retornar el commando
	cmd = tr_new String;
}

ExecOSCmdDlog::~ExecOSCmdDlog()
{
	tr_delete(cmdFld);
	tr_delete(staticText);
	tr_delete(cmd);
}

Int ExecOSCmdDlog::execute(UnChar c)
{
	if ( Panel::execute(c) == REJECT );
		return REJECT;

	return ((*cmd = cmdFld->value()) == NULL_STRING) 
			? REJECT_EXIT : ACCEPT;
}


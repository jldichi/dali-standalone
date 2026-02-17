/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: grepcomm.cc,v 1.1.1.1 1996/04/16 18:52:50 gustavof Exp $
*
* DESCRIPTION: Implementation of grep command
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM

#include <ifound.h>
#include <ifound/stdc.h>
#include <winapp/msgbox.h>
#include <edlbox.h>
#include <winapp/cell.h>
#include <ifound/rexp.h>
#include <winapp/strhis.h>

#include <command.h>
#include <grepcomm.h>
#include <grpmcomm.h>
#include <editor.h>
#include <dview.h>
#include <dtext.h>
#include <winapp/log.h>

declare(PtrList, DaliView);
declare(PtrListCursor, DaliView);

// clase GrepFiles

void GrepFiles::undo()
{
	idCheck();
}

#define GREPPANEL_ORG_ROW	6
#define GREPPANEL_ORG_COL	16

bool GrepFiles::redo()
{
	idCheck();

	GrepPanel menu(Editor::editorMsg("GrepFiles"), 
					GREPPANEL_ORG_ROW,GREPPANEL_ORG_COL,
					Editor::nroActViews(false) > 0);

	menu.execute();      

	if(menu.isAccepted() != true) 
		return false;

	// Find out which files should I use
	String files;
    if (menu.useEditedFiles()) 
    	files = Editor::ibFiles();
    else                       
    	files = menu.getFiles();
    
	// Execute ibuild
	Editor::execOSCommand(menu.getGrepCmd() + " " + files);
    
	return false;
}

// end clase GrepFiles


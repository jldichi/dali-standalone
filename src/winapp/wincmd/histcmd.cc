/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: histcmd.cc,v 1.1.1.1 1996/04/16 18:52:47 gustavof Exp $
*
* DESCRIPTION: Implementation of HistCmd class
*********************************************************************/

#define INITCOMM

#include <winapp/wincmd.h>
#include <winapp/histcmd.h>
#include <winapp/view.h>
#include <winapp/text.h>
#include <winapp/sensor.h>
#include <local/strfile.h>
#include <winapp/strhis.h>

// clase History

void HistCmd::undo()
{ 
	idCheck();
}

Bool HistCmd::redo()
{
	idCheck();
	Int cid;
	WinCmd *theCmd;
	UnChar c;                  
    
	String stUndo = App::editorMsg("UNDO")+"["+
					toString(App::his->currLevel())+"/"+
					toString(App::his->levels())+"]";
	view->writeStLine(stUndo);

	while ((c = App::getKey()) != K_UNDO && c != K_END) {
		switch(c) {
		case K_CURS_UP:
			if (App::his->mayUndo()) { 
				(App::his->retCommand())->_undo();
				App::his->prev();	
			}
			else App::beep();
			break;
		case K_PAGE_UP:
			if (App::his->mayUndo()) {
				cid = (theCmd = App::his->retCommand())->id();
				do {
					theCmd->_undo();
					App::his->prev();
				} while (App::his->mayUndo() &&
						(theCmd=App::his->retCommand())->id() == cid);
			} else
				App::beep();
			break;
		case K_CURS_DOWN:
			if (App::his->mayRedo()) {
				App::his->next();
				(App::his->retCommand())->_redo();
			} 
			else App::beep();
			break;
		case K_PAGE_DOWN:
			if (App::his->mayRedo()) {
				App::his->next();
				cid = (theCmd=App::his->retCommand())->id();
				do {
					theCmd->_redo();
				} while (App::his->mayRedo() && (App::his->next(),
						(theCmd=App::his->retCommand())->id()) == cid);

				if (theCmd->id() != cid)
					App::his->prev();
			} else
				App::beep();
			break;
		default:
			App::beep();
			break;
		}                                                                          
		stUndo = App::editorMsg("UNDO")+"["+
				 toString(App::his->currLevel())+"/"+
				 toString(App::his->levels())+"]";
    	view->writeStLine(stUndo);
	}   
	view->writeStLine();
	return FALSE;
}



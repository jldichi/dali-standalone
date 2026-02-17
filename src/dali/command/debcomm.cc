/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: debcomm.cc,v 1.2 1997/11/18 18:26:26 guiller Exp $
*
* DESCRIPTION: Implementation of command class
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM

#include <dsensor.h>
#include <command.h>
#include <editor.h>
#include <dview.h>
#include <dtext.h>
#include <edlbox.h>
#include <winapp/msgbox.h>
#include <winapp/strhis.h>
#include <debcomm.h> 
#include <ifound/stdc.h>
#include <ifound/str.h> 
#include <winapp/menu.h>
#include <debugger.h>
#include <debbox.h>

declare(PtrList,DaliView);
declare(PtrListCursor,DaliView);

// ********************************************
// Comandos que se usan solamente en modo DEBUG
// ********************************************

#define makeStr(str, ch)	(str[0]=(ch), str[1]=' ', str[2]=0)     
#define DEBUGCMD_ROWS		3
#define DEBUGCMD_COLS		60
#define DEBUGCMD_ORG_ROW	5
#define DEBUGCMD_ORG_COL	10

//
// Class StartDebug     
//
bool DbgStart::redo()
{  
	idCheck();
	String args, fname; 
	                     
	if (Editor::getMode() & DEBUG_MODE) {
		MsgBox errbox(libMsg("ERROR"), Editor::editorMsg("DEBMODEON"), 
					5, 35, 6, 20);
		errbox.execute();
   	 	return false;
   	}

	PDebugBox debugDlg("Debugger", 8, 36, 6, 21);
	
	debugDlg.execute();
	
	if(debugDlg.isAccepted() != true) 
			return false;
    
	Window::redraw();

	// cmd syntax: fname arg1 ... argN
	String cmd = debugDlg.value();    

	StrCursor stCmd(cmd, " ");
	fname = stCmd.value();			// get file name
	while (stCmd++)    		// get arguments
		args += String(" ") + stCmd.value();
	
	if (debugDlg.isWmClient()) {       

		// A tty is required for the Wm //
		String tty = debugDlg.getTTY();
		if (tty == NULL_STRING)	{
			MsgBox errbox(libMsg("ERROR"), Editor::editorMsg("DEBTTY"), 
							5, 35, 6, 20);
			errbox.execute();
	   	 	return false;        
		} 	   	

		//  Command syntax: fname !t device arg1 .. argN //
        args = String(" !t ") + tty + " " + args;
	}
		
	(*Editor::debugProgs) << cmd;

	// Create the Debugger Object   
	Editor::debugger = tr_new Debugger(fname, args);

	if (!*Editor::debugger) {
		MsgBox errbox(libMsg("ERROR"), Editor::editorMsg("DEBPIPE"), 
					5, 35, 6, 20);
		errbox.execute();

		tr_delete(Editor::debugger);
   	 	return false;
	}
	
	// Open main file
	if ( ! Editor::debugger->openMain()) {
		MsgBox errbox(libMsg("ERROR"), Editor::editorMsg("DEBNOFILE"), 
				5, 35, 6, 20);
		errbox.execute();

	 	// Destroy debugger object                              
	    tr_delete(Editor::debugger);
	    return false;
	}      

	// Starts debug mode
	Editor::setMode(DEBUG_MODE);

	// Display start message
	Editor::debugger->startMsg();

	// Arrange windows
	Editor::cascadeWindows();
	
	return true;
}

void DbgStart::undo()
{
	idCheck();
}

static bool checkMode()
{
	if (Editor::getMode() & DEBUG_MODE)
		return true;

	MsgBox errbox(libMsg("ERROR"), Editor::editorMsg("DEBMODEOFF"), 
				  5, 35, 6, 20);
	errbox.execute();
 	return false;
}

//                      
// Class StopDebug
//
bool DbgStop::redo()
{
	idCheck();
    if (!checkMode()) return false;

	// Restore editor mode
	Editor::setMode(NULL_MODE); 
    
	// Destroy debugger object                            
    tr_delete(Editor::debugger);
	
	return true;
}

void DbgStop::undo()   
{                  
	idCheck();
}

//  
// Class DbgNext
//
bool DbgNext::redo()
{
	idCheck();
    if (!checkMode()) return false;
    
 	Editor::debugger->doCommand("next");
	return true;
}

void DbgNext::undo()
{
	idCheck();
}
             
//  
// Class DbgNextI
//
bool DbgNextI::redo()
{
	idCheck();
    if (!checkMode()) return false;
    
 	Editor::debugger->doCommand("nexti");
	return true;
}

void DbgNextI::undo()
{
	idCheck();
}           

//  
// Class DbgStep
//
bool DbgStep::redo()
{
	idCheck();
    if (!checkMode()) return false;
    
 	Editor::debugger->doCommand("step");
 	
	return true;
}

void DbgStep::undo()
{
	idCheck();
}           

//  
// Class DbgStepI
//
bool DbgStepI::redo()
{
	idCheck();
    if (!checkMode()) return false;
    
 	Editor::debugger->doCommand("stepi");
	return true;
}

void DbgStepI::undo()
{
	idCheck();
}           

//  
// Class DbgRun
//
bool DbgRun::redo()
{
	idCheck();
    if (!checkMode()) return false;
    
 	Editor::debugger->doCommand("run");
	return true;
}

void DbgRun::undo()
{
	idCheck();
}
             
//  
// Class DbgContinue
//
bool DbgContinue::redo()
{
	idCheck();
    if (!checkMode()) return false;
    
 	Editor::debugger->doCommand("continue");
	return true;
}

void DbgContinue::undo()
{
	idCheck();
}

//  
// Class DbgPrintThis
//
bool DbgPrintThis::redo()
{
	idCheck();
    if (!checkMode()) return false;
    
 	Editor::debugger->doVarExpr("print *this", "*this");
	return true;
}

void DbgPrintThis::undo()
{
	idCheck();
}

//
//	Class DebugCmdBox
//             
bool DbgCmdBox::redo()
{
	idCheck();
    if (!checkMode()) return false;
    
	EdLineDialog debugCmd(Editor::debugProgs, "Debugger", 
		Editor::editorMsg("COMMAND"), DEBUGCMD_ROWS, DEBUGCMD_COLS,
		DEBUGCMD_ORG_ROW, DEBUGCMD_ORG_COL);
	  
	if (debugCmd.execute() == REJECT_EXIT) 
		return false;

	*Editor::debugProgs << debugCmd.value();

	Window::refresh();		// refresh all the windows
    Editor::debugger->doCommand(debugCmd.value());
    
    return true;
}

void DbgCmdBox::undo()
{
	idCheck();
}


//
//	Class DbgInsDelBreak
//
static bool execBreak(DaliView *view, UnChar key)
{
	Int breakLine = view->getRow();
	String sLine = toString(breakLine + 1);
	
	bool ff = Editor::debugger->doBreak(view->getFile().baseFullName(),
										sLine, key);
	if (!ff) {
		MsgBox errbox(libMsg("ERROR"), 
			Editor::editorMsg("DEBSETBREAK"), 5, 35, 6, 20);
		errbox.execute();
   	 	return false;
	}

	Editor::debugger->setAttr(breakLine, key == K_INS);

	return true;
}

//
//	Class DbgInsBreak
//
bool DbgInsBreak::redo()
{              
	idCheck();
	return checkMode() && execBreak(*cView, K_INS);
}

void DbgInsBreak::undo()
{
	idCheck();
}

//
//	Class DbgDelBreak
//
bool DbgDelBreak::redo()
{              
	idCheck();
	return checkMode() && execBreak(*cView, K_DEL);
}

void DbgDelBreak::undo()
{
	idCheck();
}

                  
//                  
// Class DbgPrintWatchDisplay
//
#define WPCMD_ROWS		3
#define WPCMD_COLS		60
#define WPCMD_ORG_ROW	5
#define WPCMD_ORG_COL	10

static String getExpr(DaliView *view)
{
	DaliTextCursor block;
	List(String) thelist;
	String expr = NULL_STRING;

	if (view->hasBlock()) {
		block = view->getBlock();
		view->getRegion(thelist, block);
		ListCursor(String) lc(thelist);
		   
		// Get current block
		for (; lc; lc++) 
			expr += lc.value();

		// Strip breakpoints attribute
		if (expr[0] == *BEGIN_ATTR) {
			while (expr[0] != END_ATTR) expr.delChar(0);
			expr.delChar(0);
		}
	}
	else {
		EdLineDialog wpDlg(Editor::debugEntries, Editor::editorMsg("DEBPW"),
		 	Editor::editorMsg("DEBEXPR"),
			WPCMD_ROWS, WPCMD_COLS, WPCMD_ORG_ROW, WPCMD_ORG_COL);
	  
		if (wpDlg.execute() == REJECT_EXIT) 
			return NULL_STRING;

		expr = wpDlg.value();
		*Editor::debugEntries << expr;
	} 

	Window::refresh();

	return expr;
}

//                  
// Class DbgPrint
//
bool DbgPrint::redo()
{                       
	idCheck();
    if (!checkMode()) return false;
    
	String expr = getExpr(*cView);
	if (!expr) return false;
	Editor::debugger->doVarExpr(String("print ") + expr, expr); 
	return true;
}

void DbgPrint::undo()
{
	idCheck();
}

//                  
// Class DbgWatch                  
//
bool DbgWatch::redo()
{                       
	idCheck();
    if (!checkMode()) return false;
    
	String expr = getExpr(*cView);
	if (!expr) return false;
	Editor::debugger->doCommand(String("watch ") + expr); 
	return true;
}

void DbgWatch::undo()
{
	idCheck();
}

//                  
// Class DbgDisplay
//
bool DbgDisplay::redo()
{                       
	idCheck();
    if (!checkMode()) return false;
    
	String expr = getExpr(*cView);
	if (!expr) return false;
	Editor::debugger->doCommand(String("display ") + expr); 
	return true;
}

void DbgDisplay::undo()
{
	idCheck();
}

//
//	Class DbPrintTypeVar
//
static bool isIdentifier(char *var)
{      
	if (isalpha(*var) || *var == '_') {
		do {       
			var++;                   
		} while (*var && (isalnum(*var) || *var == '_' || *var == ':'));
		return (*var) ? ERR : OK;
	}		                       
	return ERR;
}

//
//	Class DbPrintVar
//
bool DbgPrintVar::redo()
{                
	idCheck();
    if (!checkMode()) return false;
    
	String var;
	DaliView *cview = cView.value();

	if ((var = cview->getToken()) != NULL_STRING) {
		if (isIdentifier((char*) toCharPtr(var)) != OK) {
			Editor::beep();                              
			return false;
		}			
	 	if (chr == 'v') 
	 		Editor::debugger->doVarExpr(String("print ") + var, var);
	 	else                                                 
	 		Editor::debugger->doVarExpr(String("print (char *) debugString(")+var+")", var);
	 	cview->redraw();
	 	cview->refresh(); 
   	}
		
	return (var != NULL_STRING) ? true : false;
}

void DbgPrintVar::undo()
{
	idCheck();
}

//
//	Class DbTypeVar
//
bool DbgTypeVar::redo()
{                
	idCheck();
    if (!checkMode()) return false;
    
	String var;
	DaliView *cview = cView.value();

	if ((var = cview->getToken()) != NULL_STRING) {
		if (isIdentifier((char*) toCharPtr(var)) != OK) {
			Editor::beep();                              
			return false;
		}			
	 	Editor::debugger->doVarExpr(String("ptype ") + var, var);
	 	cview->redraw();
	 	cview->refresh(); 
   	}
		
	return (var != NULL_STRING) ? true : false;
}

void DbgTypeVar::undo()
{
	idCheck();
}

//
//	Class DbgStack
//
#define DBST_HEIGHT			14
#define DBST_WIDTH			60
bool DbgStack::redo()
{   
	idCheck();
    if (!checkMode()) return false;
    
	Int bb;
	
	// Read the hole stack                                                  
	bb = Editor::debugger->fillMenu("backtrace", 
					Debugger::stFrames, DBST_WIDTH);
	if (bb == ERR) return false;
	
	String nfr = Editor::debugger->getCurrentFrame();
	String title = Editor::editorMsg("DEBSTACK") + nfr + ")";
	if (Debugger::stFrames.dim()) {
		Int height = Debugger::stFrames.dim();
		if (height > DBST_HEIGHT) height = DBST_HEIGHT;
		
		ScrollMenu stViewer(title, Debugger::fpGetFrames, 
							height,DBST_WIDTH+2);

		if (stViewer.execute() != REJECT_EXIT) {
			char buffer[10];
			sprintf(buffer, "%d",stViewer.currOpt());    
			Editor::debugger->doCommand(String("frame ") + String(buffer));
		}			
		
		Debugger::stFrames.reAlloc(0);
	}                

	return true;
}

void DbgStack::undo()
{
	idCheck();
}

//
//	Class DbgRegs
//                        
#define DBREG_HEIGHT		14
#define DBREG_WIDTH			60
bool DbgRegs::redo()
{              
	idCheck();
    if (!checkMode()) return false;
    
	Int bb = Editor::debugger->fillMenu("info registers", 
			Debugger::regList, DBREG_WIDTH, DbgRegs::fmtRegs);
	
	if (bb == ERR) {
		Editor::beep();
		return false;
	}
	
	String nfr = Editor::debugger->getCurrentFrame();
	String title = Editor::editorMsg("DEBREGS") + nfr + ")";
	if (Debugger::regList.dim()) {
		Int height = Debugger::regList.dim();
		if (height > DBREG_HEIGHT) height = DBREG_HEIGHT;
		
		ScrollMenu stViewer(title, Debugger::fpGetRegs, 
							height,DBREG_WIDTH+2);

		stViewer.execute();
		Debugger::stFrames.reAlloc(0);
	}                

	return true;
}

void DbgRegs::undo()
{
	idCheck();
}

String *DbgRegs::fmtRegs(const String &streg)
{
	return tr_new String(streg.field(0, "\t"));
}


//
//	Class DebugDisplays
//                        
#define DBDSP_HEIGHT		14
#define DBDSP_WIDTH			60
bool DbgDisplays::redo()
{
	idCheck();
    if (!checkMode()) return false;
    
	Int bb = Editor::debugger->fillMenu("display", 
			Debugger::dspList, DBDSP_WIDTH, DbgDisplays::fmtDisplays);
	
	if (bb == ERR) {
		Editor::beep();
		return false;
	}
	
	if (Debugger::dspList.dim()) {
		Int height = Debugger::dspList.dim();
		if (height > DBDSP_HEIGHT) height = DBDSP_HEIGHT;
		
		String title = "Display (ENTER to delete)";
		ScrollMenu stViewer(title, Debugger::fpGetDisplays, 
							height, DBDSP_WIDTH+2);

		if (stViewer.execute() != REJECT_EXIT) {
			String dspln = *Debugger::dspList.elem(stViewer.currOpt());
			Editor::debugger->doCommand("undisplay " + dspln.field(0, ":"));
		}			

		Debugger::dspList.reAlloc(0);
	}                
	return true;
}

void DbgDisplays::undo()
{
	idCheck();
}

String *DbgDisplays::fmtDisplays(const String &streg)
{
	return tr_new String(streg);
}


//
//	Class DbgWatchBreaks
//                        
#define DBWB_HEIGHT			14
#define DBWB_WIDTH			60
bool DbgWatchBreaks::redo()
{
	idCheck();
    if (!checkMode()) return false;
    
	Int bb = Editor::debugger->fillMenu("info break", 
		Debugger::bwList, DBWB_WIDTH, DbgWatchBreaks::fmtWatchBreaks);
	if (bb == ERR) return false;

	if (Debugger::bwList.dim()) {
		Int height = Debugger::bwList.dim();
		if (height > DBWB_HEIGHT) height = DBWB_HEIGHT;
		
		String title = "Breaks/Watchs (ENTER to delete)";
		ScrollMenu stViewer(title, Debugger::fpGetWatchBreaks, 
							height, DBWB_WIDTH+2);

		Int dspn = ERR;
		if (stViewer.execute() != REJECT_EXIT && (dspn = stViewer.currOpt())) {
			String dspln = *Debugger::bwList.elem(dspn);
			if (dspln(4, 10) == "breakpoint") {
				String nline = dspln.field(-1, ":");
				Editor::debugger->setAttr(toInt(nline)-1, false);
			}
			Editor::debugger->doCommand("delete" + dspln.field(0," "));
		}			

		Debugger::bwList.reAlloc(0);
	}                
	return true;
}

void DbgWatchBreaks::undo()
{
	idCheck();
}

String *DbgWatchBreaks::fmtWatchBreaks(const String &streg)
{                          
	return tr_new String(streg);
}


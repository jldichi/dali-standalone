/********************************************************************
* Copyright	(c)	1991 InterSoft Co.	All	Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright	notice above does not evidence any
* actual or	intended publication of	such source	code.
*
* $Id: debugger.cc,v 1.4 2002/03/18 21:39:50 hardaiz Exp $
*
* DESCRIPTION: Implementation of debugger class
*********************************************************************/
#define	INITCOMM
#ifdef __GNUC__
#pragma implementation
#endif

#include <dtext.h>
#include <dsensor.h>
#include <command.h>
#include <editor.h>
#include <dview.h>
#include <edlbox.h>
#include <winapp/strhis.h>
#include <ifound/str.h>
#include <debugger.h>
#include <local/strfile.h>

declare(PtrList,DaliView);
declare(PtrListCursor,DaliView);

PtrArray(String) Debugger::regList;		//	registers list
PtrArray(String) Debugger::stFrames;	//	stack frames
PtrArray(String) Debugger::dspList;		//	displays list
PtrArray(String) Debugger::bwList;		// breaks &	watchs

#define	CTRLZ				'\032'
#define	PROMPT				"\001"

static bool _reloadView(DaliView *cview)
	// reload view
{            
    // En que fila-columna estoy
    Int inrow = cview->getRow();
	Int atcol = cview->getCol();
     
    cview->reload();
    cview->goCursor(inrow,atcol,true);

	return false;
}

Debugger::Debugger(String fname, String	args)
: 	fileName(fname),
#ifndef HAVE_CONSTRUCTORS_BUG
	ioPipe (tr_new InOutPipe(String(DEBUGGER) + fname)),
	dbView (tr_new DaliView(false,String(DEBUGGER_VIEW))),
	fArgs(args)
{         
#else
	fArgs(args)
{         
	ioPipe = tr_new InOutPipe(String(DEBUGGER) + fname);
	dbView = tr_new DaliView((bool)false,DEBUGGER_VIEW);
#endif 

	debuggerOK 	= true;
	pline 		= ERR;
	pname 		= NULL_STRING;
	
	Editor::addFooterView(dbView);
	dbView->setDisplayMode();

	*ioPipe <<	String("set prompt ") +	PROMPT + "\\n" << endl;
	*ioPipe >>	rcvcmd;	  	// receive first	prompt
	*ioPipe >>	ch;			// consume space

}

Debugger::~Debugger() // LOOK HERE FOR BUG IN GCC v2.4.2
{
	//tr_delete(ioPipe); ---------->>> toDo	: Fix this when	possible
	tr_delete((InputStream	*) ioPipe);
  	Editor::delView(dbView);
  	Editor::doForAllViews(_reloadView);
}

void Debugger::startMsg()
{
 	dbView->appendLine(String("　　　　　　　 Debugger started : ") + fileName	+ " 　　　　　　　");
}

void Debugger::redrawView()
{
	dbView->redraw();
}

//
//	Parse a debugger	message, open the file
//	(if	needed)	and	position the cursor.
//
void Debugger::parseCmd(String command)
{              
	//	Delete ctrl	chars
	command.delChar(0);
	command.delChar(0);

	StrCursor stFields(command, ":");

	String	fName =	stFields.value();
	stFields++;					//	go to next field
	String	nLine =	stFields.value();

	DaliView *view	= NULL;
    assert(*cView);

	if (!pname.isNull() && ((view = view->existText(pname)) != NULL)) {
		Editor::makeCurrentView(view);
		setAttr(pline == ERR ? cView->getRow() : pline,false,A_REVERSE);
	} 
    
	if	((view = view->existText(fName)) ==	NULL) {
		Editor::openFile(fName);
		Window::refresh();		
	}
	else 
		Editor::makeCurrentView(view);
    
	cView->setDisplayMode();
	cView->goCursor(toInt(nLine) - 1);
	setAttr(pline = cView->getRow(),true,A_REVERSE);
	cView->redrawCurrLine();
	pname = cView->getFileName();
}

//
//	Send	a command to the debugger and
//	display	the	response on	the	output window.
//
void Debugger::doCommand(String	sendcom, bool output)
{
	Int pos = ERR;

	*ioPipe << sendcom << endl;
	while ((*ioPipe >>	rcvcmd).ok() && rcvcmd != PROMPT) {
		if	(rcvcmd.index("No such file") != ERR) {
			debuggerOK = false;
			return;
		}	    

		// WARNING: CTRLZ may not be at position 0 of rcvcmd !!		
		// For example, after output is received from user`s program. 
		if (!rcvcmd.isNull() && (pos = rcvcmd.index(CTRLZ)) != ERR) 
			parseCmd(rcvcmd(pos));                          
		else {
			if (output) 
				dbView->appendLine(rcvcmd);
		}
	}

	*ioPipe >>	ch;		//	consume	space char

	//	Refresh	the	hole desktop
	Window::refresh();
}

//
//	Search for the main function	and	open
//	the file	using the parseCmd() method
//
bool Debugger::openMain()
{              
	if (!debuggerOK) 
		return false;
		
	doCommand(String("set args ")+fArgs, false);
	doCommand(String("break main"), false);
	doCommand(String("run"));
	//doCommand(String("disable"), false);
	return true;
}

void Debugger::doVarExpr(String	sendcom, String	varexp)
{
	char theChar;
	String rcvCmd, out;

	*ioPipe <<	sendcom	<< endl;
	*ioPipe >>	rcvCmd;

	//
	//	Check to see if	the	symbol or expression
	//	is defined in the current context and is
	//	not	a syntax error.
	//
	String	token =	rcvCmd.field(0,	" ");
	if	(token == "No" || token	== "parse")	{
		*ioPipe >> rcvCmd; 		// read prompt
		dbView->appendLine(rcvCmd);
	} else	{
		Int pos = rcvCmd.index(' ');
		// assert(pos != ERR);
		out = varexp + rcvCmd(pos);
		dbView->appendLine(out);
		while ((*ioPipe >> rcvCmd).ok() && rcvCmd != PROMPT) 
			dbView->appendLine(rcvCmd);
	}
	*ioPipe >> theChar;			// consume space char
}

bool Debugger::doBreak(const String	&fname,	const String &line,	UnChar c)
{
	char theChar;
	String rcvCmd, cmd;

	cmd = (c == K_INS)	? "break " : "clear ";
	*ioPipe << cmd + fname + ":" + line << endl;
	*ioPipe >> rcvCmd;

	String token = rcvCmd.field(0, " ");

	if	(token == "No")	{
		*ioPipe >> rcvCmd;		// read prompt
		*ioPipe >> theChar;		// consume space char
		return false;
	}

	while ((*ioPipe >> rcvCmd).ok() && (rcvCmd != PROMPT) &&
			(rcvCmd.index(CTRLZ) == ERR))
		dbView->appendLine(rcvCmd);

	*ioPipe >> theChar;			// consume space char

	return	true;
}

bool Debugger::setAttr(Int	nline, bool	bb, Int attr)
{   
	DaliView *view = cView.value();
		
	DaliTextCursor	tc2, tc1;
	tc1 = *view->text;	tc2	= *view->text;		

	//	Set	tc1	at the beginning of	the	line
	tc1.goTo(nline);

	//	Set	tc1	at the end of the line
	tc2.goTo(nline);
	tc2.goToCol(tc2.value().length() -	1);
    
// Para la barra larga
//	tc2.goToCol(80);
//  tc2.setChar(' ');
    
	if	(bb)
		view->text->setAttr(tc1, tc2,	attr);
	else
		view->text->delAttr(tc1, tc2,	attr);
	view->redraw();

	return	true;
}

bool Debugger::fillMenu(String sencmd, PtrArray(String)	&array,
	Int ww, SPFPS fmtFunc)
{
	char theChar;
	String rcvCmd;

	*ioPipe << sencmd << endl;
	*ioPipe >> rcvCmd;

	if (rcvCmd == PROMPT) {
		*ioPipe >> theChar;			// consume space char
		return false;
	}

	String token = rcvCmd.field(0, " ");
	if	(token == "No")	{
		*ioPipe >> rcvCmd; 		// read prompt
		*ioPipe >> theChar;		// consume space char
		return false;
	}

	do	{
		if (fmtFunc)
			array.add(fmtFunc(rcvCmd));
		else array.add(tr_new String(rcvCmd(0,	ww-1)));
	} while ((*ioPipe >> rcvCmd).ok() && rcvCmd != PROMPT);

	*ioPipe >> theChar;			// consume space char

	return	true;
}

String Debugger::getCurrentFrame()
{
	char theChar;
	String rcvCmd;

	*ioPipe <<	"info frame" <<	endl;
	*ioPipe >>	rcvCmd;

	String	token =	rcvCmd.field(0,	" ");

	if	(token == "No")	{
		*ioPipe >> rcvCmd; 		// read prompt
		*ioPipe >> theChar;		// consume space char
		return "#";
	}

	token =	rcvCmd.field(2,	" ");
	token =	token.field(0, ",");

	while ((*ioPipe >> rcvCmd).ok() && rcvCmd != PROMPT)
		;

	*ioPipe >> theChar;			// consume space char

	return	token;
}

const String Debugger::fpGetFrames(Int n)
{
	if	(stFrames.isInRange(n))
		return *stFrames.elem(n);
	else
		return HIGH_VALUE;
}

const String Debugger::fpGetRegs(Int n)
{
	if	(regList.isInRange(n))
		return *regList.elem(n);
	else
		return HIGH_VALUE;
}

const String Debugger::fpGetDisplays(Int n)
{
	if	(dspList.isInRange(n))
		return *dspList.elem(n);
	else
		return HIGH_VALUE;
}

//Mientras se mejora ScrollMenu	...
static String makePrintable(const String &s)
{
	Int len = s.length();
	String	rs;
	for (Int a	= 0; a < len; a++) {
		if	(s[a] != '\t') {
			if (isprint(s[a]))
				rs += s[a];   
		}
		else             //Tabsize
			rs += String(4,' ');  
	}
	return	rs;
}

const String Debugger::fpGetWatchBreaks(Int n)
{
	if	(bwList.isInRange(n))
		return makePrintable(*bwList.elem(n));
	else
		return HIGH_VALUE;
}


/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: debugger.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION: Implementation of debugger class
*********************************************************************/

#ifndef DEBUGGER_H
#define DEBUGGER_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <local/strfile.h>
#include <ifound/parray.h>

// Class declarations
class DaliView;
class String;        

#ifndef DEF_PARRAY_STRING
declare(PtrArray,String);
#define DEF_PARRAY_STRING
#endif

typedef String* (*SPFPS) (const String &);	

#define DEBUGGER		"gdb -fullname -quiet "

class Debugger {
   
  private:
            
	String 	 	fileName;
	DaliView    *dbView;
	InOutPipe 	*ioPipe;
	String		fArgs;
	bool		debuggerOK;

	Int 		pline;	// Previous line and view name
	String		pname;

	char		ch;                
	String 		rcvcmd, sndcmd;                
	
	Debugger (const Debugger&) ;
	void operator=(const Debugger&) ;
	
  public:          
  
  	Debugger(String fname, String args);
  	~Debugger();
                                      
	int 	operator!() const 	{ return !*ioPipe; }
	
	void 	startMsg();
	bool	openMain(); 
	void	redrawView();
	void 	doCommand(String sendcom, bool output = true);
	void 	parseCmd(String command);
	bool 	setAttr(Int nline, bool bb, Int attr = A_BOLD);
	bool 	doBreak(const String &fname, const String &line, UnChar c);
	void 	doVarExpr(String sendcom, String varexp);
	String	getCurrentFrame();
	bool 	fillMenu(String sencmd, PtrArray(String) &array,
				Int ww, SPFPS fmtFunc = NULL);
		
	// Stack frames array & get function
	static  PtrArray(String) stFrames;
	static  const String fpGetFrames(Int n);

	// Register list array & get function
	static  PtrArray(String) regList;
	static  const String fpGetRegs(Int n);
	
	// Displays list array & get function
	static PtrArray(String) dspList;
	static const String fpGetDisplays(Int n);

	// Breaks and Watchs list array & get function
	static PtrArray(String) bwList;
	static const String fpGetWatchBreaks(Int n);

};

#endif


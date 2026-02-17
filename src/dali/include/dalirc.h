/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dalirc.h,v 1.2 1997/10/08 19:39:47 eduardoc Exp $
*********************************************************************/

#ifndef DALIRC_H
#define DALIRC_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <ifound/str.h>
#include <local/strfile.h>

enum RcToken {
	AUTOSAVEFREQ,
	BUILDCOMMAND,
	BUILDFLAGS,
 	COMMANDMODE,
 	DALIMAN,
 	DALITAGS,
	DESKTOPCHAR,
    DIFFCMD,
    FOOTERHEIGHT,
	FUNCEXPBROWSER,
	FUNCTIONSENSOR,
	HISTORYSIZE, 
	MATCHTAB,
 	MAXVIEWS,
    SAVEDESKTOP,
    SCRATCHVIEWS,
 	SEARCHPATH,
    SOURCEBROWSER,
 	TABSIZE,
	USERMODE,
	WINDOWSTILE
};

class DaliRc {
	useIdCheck(DaliRc_id);

private:
	String fileName;	// file name             
	InputFile ifile;  	// input stream

	RcToken findToken(String &lexeme);

    DaliRc(const DaliRc&) ;
    void operator=(const DaliRc&) ;
    
public:            

	DaliRc(const String &file);
	~DaliRc();

	bool    readFile();
	Int operator! () const 	{ idCheck(); return !ifile; }
	operator int() const 	{ idCheck(); return (!ifile == 0); }
	
};
#endif

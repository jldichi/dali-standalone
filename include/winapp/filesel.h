/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: filesel.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION: Class definitions for standard get file type.
*
*********************************************************************/

#ifndef FILESEL_H
#define FILESEL_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <winapp/panel.h>

class StrCell;
class FileMenu;
class DirMenu;
class FileSelector;
typedef	void (*FPFS) (FileSelector&);

// clase FileSelector

#define READ_PERM	1
#define WRITE_PERM	2
class StrHistory;
class DefaultButton;
class PushButton;

class FileSelector : public Panel {

    String searchPath;                      
    String directory;

	// Methods to build filename string after being selected 
	// with Cell or FileMenu
    void calcCellStr();
    void calcMenuStr();
	FileSelector(const FileSelector &);
	    
protected:

	String	strres;
	Int		permMode;  
	FPFS	userfunc;
    
public:

	StrCell 		*name;
	FileMenu 		*files;
	DirMenu  		*dir;
	DefaultButton	*ok_;
	PushButton		*cancel_;

	Label	 *curdir;
	Label	 *curfilter;

	FileSelector(String l, Int nf, Int nc, Int fo, Int co,
				const String &cwd = NULL_STRING, const String &ftr = "*", 
				const String &schpath = NULL_STRING, StrHistory *his = NULL);

	virtual ~FileSelector();

	void 	setUserFunc(FPFS f);
	const 	String &value()	{ return strres; }
	
	Int 	perms() const		{ return permMode; }
	Int 	checkPerms();
	String	shrunkDirLabel(const String &x);
	void	updateFilterLabel(const String &f);
	Int 	setFile();
    Int 	doSelection();
	Int 	setDirectory();

	Int   	ok();
 	Int   	cancel(); 
};
#endif

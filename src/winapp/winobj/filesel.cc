/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: filesel.cc,v 1.5 2004/09/29 21:06:35 damianm Exp $
*
* DESCRIPTION: Implementation of standard get file class
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <unistd.h>

#include <ifound.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/stdc.h>
#include <errno.h>
#include <local/osfile.h>

#ifdef HAVE_USERS
#	include <sys/types.h>
#	include <sys/stat.h>
#endif

#include <winapp/filesel.h>
#include <winapp/menu.h>
#include <winapp/button.h>
#include <winapp/cell.h>
#include <winapp/label.h>
#include <winapp/msgbox.h>
#include <ifound/rexp.h>

#ifdef HAVE_UNDERSCORE_NAMES
#	define stat _stat
#endif

#define MAX_GROUPS	17
#define MAX_DIR_LABEL 39

declare(ObjectAction, FileSelector);
implement(ObjectAction, FileSelector);

FileSelector::FileSelector(String l, Int nf, Int nc, Int fo, Int co,
							const String &cwd, const String &ftr, 
							const String &schpath, StrHistory *his)
: Panel(l, nf, nc, fo, co), searchPath(schpath)
{
	idCheck();

	name  	= tr_new StrCell(64, 39, 0, 1, 2, his);
	files 	= tr_new FileMenu(10, 16, 5, 2, cwd, ftr);
	dir   	= tr_new DirMenu(*files, 10, 16, 5, 23, cwd);

 	// OK and CANCEL buttons
	ok_      = tr_new DefaultButton(16, 8,libMsg("OK"), 10,
		defaultAction = tr_new ObjectAction(FileSelector)(this, &FileSelector::ok));

	cancel_  = tr_new PushButton(16, 23,libMsg("CANCEL"), 10,
		tr_new ObjectAction(FileSelector)(this, &FileSelector::cancel));

	name->setAction(tr_new ObjectAction(FileSelector)(this, &FileSelector::setFile));   
	files->setAction(tr_new ObjectAction(FileSelector)(this, &FileSelector::doSelection));   
	dir->setAction(tr_new ObjectAction(FileSelector)(this, &FileSelector::setDirectory));   

	curdir		= tr_new Label(shrunkDirLabel(cwd), 2, 2, MAX_DIR_LABEL); 
	curfilter	= tr_new Label(ftr, 3, 2, 10); 
 	
	InitObjs((WinObj**)&name, 5);
	InitLabels(&curdir, 2);
	
    userfunc	= NULL;

	files->setCurrent(ERR, true);
	dir->setCurrent(0, false);
}                               

FileSelector::~FileSelector()
{
	idCheck();

	tr_delete(name);
	tr_delete(files);
	tr_delete(dir);                    
	tr_delete(curdir);
	tr_delete(curfilter);

	tr_delete(ok_);
	tr_delete(cancel_);
}

#include <stdio.h>

Int FileSelector::ok()
{     
	idCheck();

    if (name->value() != NULL_STRING) {
	   	calcCellStr();                 
	}
    else {
		if (files->currOpt() != ERR) 
			calcMenuStr();              // Is this ever used ??
		else 
			return REJECT_EXIT;
	}
    
	// Check file permisions and exit.
    return (checkPerms() == ERR) ? REJECT_EXIT : ACCEPT_EXIT;
}

Int FileSelector::cancel()
{     
	idCheck();

	return REJECT_EXIT;
}

void FileSelector::setUserFunc( FPFS f )
	// 	Allows the execution of a user-provided function after 
	//	a file-selection is done via the FileMenu with Space bar. 
{     
	idCheck();

	userfunc = f;
}

void FileSelector::calcMenuStr()
	// Builds string with file selected with StrCell 
{                  
	idCheck();

    String s1,temp;
    
	files->textOpt(files->currOpt(), strres, s1);
	temp = files->retDir();
	if (temp != files->retCwd()) {
		directory = temp.field(1, files->retCwd()+PATH_SLASH);
		if (directory == NULL_STRING)
			if (temp == PATH_SLASH)
				strres = temp + strres;
			else
				strres = temp + PATH_SLASH + strres;
		else
			strres = directory + PATH_SLASH + strres;
	}              
}

void FileSelector::calcCellStr()
	// Builds string with file selected with FileMenu
{                  
	idCheck();
	
	String temp;

	temp = files->retDir();
	bool cond = (temp == files->retCwd());
	if (cond || name->value().index(PATH_SLASH[0]) != ERR)
		strres = name->value();
	else {	
		directory = temp.field(1, files->retCwd()+PATH_SLASH);
		if (directory == NULL_STRING) 
			if (temp == PATH_SLASH)
				strres = temp + name->value();
			else
				strres = temp + PATH_SLASH + name->value();
		else
			strres = directory+ PATH_SLASH + name->value();
	}
}

Int FileSelector::checkPerms()
{
	idCheck();

	struct stat	st;
	static GidT grpList[MAX_GROUPS];
	static int ngrps = ERR;
	String filelist;

	if (ngrps == ERR) {

		grpList[0] = getegid();
		ngrps = 1;

#ifdef HAVE_GETGROUPS
		int ng;
		if ((ng = getgroups(MAX_GROUPS-1, &grpList[1])) != ERR)
			ngrps += ng;
#endif
	}

	for (StrCursor c(strres, " "); c; c++) { 
		String filech = findInPath(String(c.value()).expandEnvVars(), searchPath);
		Int	tmpRes, ult = filech.backIndex(PATH_SLASH[0]);

		permMode = READ_PERM|WRITE_PERM;
		if (ult != ERR && ult != 0) {
			directory = filech(0, ult);
			if (stat((char *) toCharPtr(directory), &st)) {
				MsgBox errbox(libMsg("ERR"),
						libMsg("CANTACCESSDIR")+" ("+filech+")",
						5,35,6,20);
				errbox.execute();
				continue;
			} else {
#ifndef HAVE_USERS
				if (!canSearch(st.st_mode, 0, 0, 0, NULL, 0))
					permMode &= ~(WRITE_PERM|READ_PERM);

				if (!canWrite(st.st_mode, 0, 0, 0, NULL, 0))
					permMode &= ~WRITE_PERM;

				if (!canRead(st.st_mode, 0, 0, 0, NULL, 0))
					permMode &= ~READ_PERM;
#else
				if (!canSearch(st.st_mode, st.st_uid, st.st_gid,
						geteuid(), grpList, ngrps))
					permMode &= ~(WRITE_PERM|READ_PERM);

				if (!canWrite(st.st_mode, st.st_uid, st.st_gid,
						geteuid(), grpList, ngrps))
					permMode &= ~WRITE_PERM;

				if (!canRead(st.st_mode, st.st_uid, st.st_gid,
						geteuid(), grpList, ngrps))
					permMode &= ~READ_PERM;
#endif
			}
		}

		tmpRes = stat((char *) toCharPtr(filech), &st);
		if (tmpRes) {
			if (errno != ENOENT)
				permMode &= ~(READ_PERM|WRITE_PERM);
		} else {
			if (!(st.st_mode & S_IFREG)) {
				MsgBox errbox(libMsg("ERR"),
						libMsg("NOTREGFILE")+"("+filech+")",
						5,40,6,20);
				errbox.execute();
				continue;
			}
#ifndef HAVE_USERS
			if (!canRead(st.st_mode, 0, 0, 0, NULL, 0))
				permMode &= ~READ_PERM;

			if (!canWrite(st.st_mode, 0, 0, 0, NULL, 0))
				permMode &= ~WRITE_PERM;
#else
			if (!canRead(st.st_mode, st.st_uid, st.st_gid,
					geteuid(), grpList, ngrps))
				permMode &= ~READ_PERM;

			if (!canWrite(st.st_mode, st.st_uid, st.st_gid,
					geteuid(), grpList, ngrps))
				permMode &= ~WRITE_PERM;
#endif
		}               

		if (filelist)    
			filelist += " ";
		filelist += filech; 
	}     

	return ((strres = filelist) == NULL_STRING) ? ERR : OK;
}

String FileSelector::shrunkDirLabel(const String &x)
{
	// toThink: Should this be a method in class Label ?
	
	idCheck();

	assert(wi != NULL);

	String s = x;
	Int dlen = x.length(); 

	if (dlen > MAX_DIR_LABEL ) 
		s = s(0,10) + " ... " + s(dlen - 23,23);		
    
    return s;
}

void FileSelector::updateFilterLabel(const String &f)
{
	idCheck();

	assert(wi != NULL);
 
	curfilter->setValue(f);
	curfilter->draw();
}

Int FileSelector::setFile()
{
	idCheck();
    
    // Get out of here if the user wants to exit panel
    if (name->lastKey() == K_END)
    	return REJECT_EXIT;
    	
	// Es un directorio ?
	struct stat st;
	String newdir = String(name->value()).expandEnvVars(); 
	if (newdir != NULL_STRING) {
		String stataux = directory.isNull() || isAbsolutePath(newdir) 
							? newdir 
							: String(directory + PATH_SLASH + newdir);

		if (statFile(stataux, &st) == OK) {
			if ((st.st_mode & S_IFDIR)) {
				newdir = dir->buildDirStr(newdir);
				if (newdir != NULL_STRING) {
					dir->setDir(directory = newdir);
					files->setDir(directory);       
					files->setCurrent( 0 , true);
					name->setValue(NULL_STRING);
				} 
				// Update directory label
				curdir->setValue(shrunkDirLabel(directory));
				curdir->draw();
				return REDO_OBJECT;
			}
		}
    } else return ACCEPT;

    // Armo la reg exp
	LRegExp re(name->value());
	
	if (!re) {
		MsgBox errbox(libMsg("ERR"),
				libMsg("NOTREGFILE"),
				5,40,6,20);
		errbox.execute();
		return REDO_OBJECT;
	}

	if (!(re.operator void* ())) {
		wi->beep();
		return REDO_OBJECT;
	}

    // Es una reg exp ?  
    if (name->lastKey() == K_ENTER) {
		if (re.hasMetaChars()) {
			files->setFile(name->value());
		    updateFilterLabel(name->value());
		    
		    // Actualizo
			name->setValue(NULL_STRING);
			name->writeStr();

			return REDO_OBJECT;
		}
		else {
			calcCellStr();
		    return (checkPerms() == ERR) ? REJECT_EXIT : ACCEPT_EXIT;
		} 
   }
   
	return ACCEPT;
}

Int FileSelector::doSelection()
{  
	idCheck();
    
    // Check if there is a user function to execute
	if (files->lastKey() == ' ') {
		if (userfunc != NULL) {
			calcMenuStr();
			if (checkPerms() != ERR)
				userfunc(*this);
			front();   
		}           
		return REDO_OBJECT; 
	} 
    
    // Calculate filename string and exit
	if (files->lastKey() == K_ENTER) {
		calcMenuStr();
	    return (checkPerms() == ERR) ? REJECT_EXIT : ACCEPT_EXIT;
    }
    
	// Any other key stays in panel, except K_END that exits it.
	return (files->lastKey() == K_END) ? REJECT_EXIT : ACCEPT;
}

Int FileSelector::setDirectory()
{
	idCheck();

	String tmp = dir->retDir();

	if (tmp != directory) {
		directory = tmp;
		files->setCurrent(ERR, true);
 	} 

	// Update directory label
	curdir->setValue(shrunkDirLabel(directory));
	curdir->draw();

	return ACCEPT;
}

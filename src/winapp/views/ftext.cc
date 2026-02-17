/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ftext.cc,v 1.4 2004/04/30 20:53:56 gsposito Exp $
*
* DESCRIPTION: Implementation of file-locking & file-preserving 
*				methods for class FileText   
*
*********************************************************************/

#ifdef __GNUG__
#	pragma implementation
#endif

//
#include <errno.h>
#include <sys/stat.h>

// Local library
#include <local/dirname.h>
#include <local/osfile.h>
#include <local/strfile.h>

// Foundation library
#include <ifound/stdc.h>

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

// WinApp library
#include <winapp/ftext.h>
#include <winapp/waitmsg.h>
#include <winapp/window.h>

// Static variables
String FileText::preserveDir;
String FileText::cwdir;

FileText::FileText(String f, bool isPres) 
: EditableText()
{
	idCheck();

	// Si es un texto "preservado"	
	if (isPres) {
		preserveFilename = f;
		filename 		 = NULL_STRING;
		ttype 			 = PRESERVE;
	}
	else {         
		preserveFilename 	= tempFile();
		filename 			= f;
		ttype 	 			= READWRITE;         

		// Si no existe el archivo trato con el administrado
#ifdef __VC__
		struct _stat st;
#else
		struct stat st;
#endif		
		if ( statFile(f,&st) != OK ) 
			if (loadAdminFile(f)) 
				preserveFilename 	= NULL_STRING;
	}
}

FileText::~FileText()
{ 
	idCheck();

}

bool FileText::storePreserve()
{
	idCheck();

	if (ttype&(ISTREAM|ADMIN) || !active) return false;

	{
		OutputFile outputf(preserveFile);

		outputf.seek(0);

		// save file name and length
		outputf << file.name() << "\t" << size() << "\n";

		Int curratr = A_NORMAL;

		for (TextCursor lin(*this); lin; lin++)
			outputf << decodeAttrs(lin, curratr) << "\n";

		outputf.sync();
	}

	return true;
}

bool FileText::rmFiles()
{
	idCheck();

	if (!active || ttype&(ISTREAM|ADMIN)) return false;

	preserveFile.free(0,0);
	preserveFile.close();
	preserveFile.unLink();

	file.free(0,0);
	file.close();
	if (!existfile) file.unLink();
		
	return true;
}

void FileText::mkFiles()
{
	idCheck();

	if (ttype&(ISTREAM|ADMIN)) return;

	if (ttype&PRESERVE) {
		preserveFile.open(NULL_STRING, preserveFilename, NULL_STRING, 
					 OsFile::READ|OsFile::WRITE);
		(void)preserveFile.lock(OsFile::LOCK);

		// obtengo el nombre del archivo 
		InputFile inputf(preserveFile);
		inputf >> filename;
		filename = String(filename.field(0, "\t")).baseFullName();

	} else {
		preserveFile.open(NULL_STRING, preserveFilename, NULL_STRING, 
					 OsFile::READ|OsFile::WRITE|OsFile::CREAT);
		(void)preserveFile.lock(OsFile::LOCK);
	}

    String dir;

    // The File has already an absolute dir 
	if (filename[0] == PATH_SLASH[0])
		dir = filename.pathName();
	else
		dir = cwdir+String(PATH_SLASH)+filename.pathName();	

	file.open(dir, filename.baseFullName(), NULL_STRING, 
			  OsFile::READ|OsFile::WRITE|OsFile::FAIL);
			  
	fileperm = PERM_WRITE|PERM_READ;
	existfile = true;

	// el archivo no existe
	if (!file == ENOENT) {
		file.close();
		file.open(dir, filename.baseFullName(), NULL_STRING, 
				  OsFile::READ|OsFile::WRITE|OsFile::CREAT);
		existfile = false;
		(void)file.lock(OsFile::LOCK);
	// ya esta lockeado -> solo lo puedo abrir read only
	} else if (!file || file.lock(OsFile::TEST|OsFile::LOCK)==IO_LOCKED) {
		fileperm = PERM_READ;
		if (file) fileperm |= PERM_LOCKED;
		file.close();
		file.open(dir, filename.baseFullName(),NULL_STRING, OsFile::READ);
	}
}

const String FileText::tempFile()
{
	return tempnam(toCharPtr(FileText::preserveDir), PREFIX_TMP);
}

void FileText::setFile(const String &f, bool load_new_file)
{ 
	idCheck();

	// debo dejar el text en el mismo estado que estaba
	if (!active && (!(ttype & (ADMIN|ISTREAM)))) {
		ttype 		= READWRITE;
		filename 	= f;
		preserveFilename = tempFile();
	} 
	else {                 
		if (!(ttype & (ADMIN|ISTREAM)))
			rmFiles();
	
		ttype 				= READWRITE;
		preserveFilename 	= tempFile();
		filename 			= f;
        
		mkFiles();

		if (load_new_file) 
			load();
		resetModif();
	}
}

const String &FileText::getTextName()
{
	idCheck();

	if (ttype&PRESERVE && !active) 
		return preserveFilename;
	return filename;
}

bool FileText::load()
{
	idCheck();

	if (!active || ttype&(ISTREAM|ADMIN)) return false;

	delAll();

	String aux;

	if (ttype & PRESERVE) {
		InputFile inputf(preserveFile);
		// Read file name and file length (in lines)
		inputf >> aux;
		Int nl = toInt(aux.field(1, "\t"));
	
		for (Int i = 0; i < nl; i++) {
			inputf >> aux;
			add(encodeTabs(aux));
		}
	} else {
		InputFile inputf(file);

		for(Int i = 0; inputf >> aux; i++) 
			add(encodeTabs(aux));
	}

	 // Encode attributes
	encodeAttrs();

	resetModif();

	return true;
}



bool FileText::store()
{
	idCheck();

	if (!active) return false;
    
    String newfile = filename;
    
	if(ttype&(READWRITE)) {
		storePreserve();
		file.close();            
		newfile = file.fullName(); 
	}	                         
	
	{        
		OutputFile outputf(newfile);

		Int curratr = A_NORMAL;
			for (TextCursor cursor(*this); cursor; cursor++) 
				outputf << decodeTabs(decodeAttrs(cursor,curratr)) << "\n";
	}
 
	file.open(OsFile::READ|OsFile::WRITE|OsFile::FAIL);

	assert(file);
	
	// si ahora me falla el lock -> queda locked and read only
	if (file.lock(OsFile::LOCK) == IO_LOCKED)
		fileperm = PERM_LOCKED|PERM_READ;

	existfile = true;
	ttype     = READWRITE ;

	resetModif();

	return true;
}

bool FileText::activate()
{
	idCheck();

	if (active || (ttype&(ISTREAM|ADMIN)))
		return false;

	if (preserveFilename == NULL_STRING) return false;

	active = true;

	mkFiles();

	load();

	return true;
}

void FileText::deActivate()
{
	idCheck();

	if (!active || ttype&(ISTREAM|ADMIN)) return;

	rmFiles();

	delAll();

	resetModif();

	ttype = READWRITE;
	active = false;
}

void FileText::addPath()
	// Adds path to filename, just to display it.
{         
	idCheck();

    DirName	fileDir(filename.pathName()),
    	    currDir(cwdir);
    
   	currDir.chDir(fileDir.value());
	filename = currDir.value()+String(PATH_SLASH)+filename.baseFullName();	
}                                  

// end of FileText

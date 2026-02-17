/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ftext.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION
* Class definitions for FileText type.
*********************************************************************/

#ifndef FTEXT_H
#define FTEXT_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>
#include <winapp/etext.h>
#include <winapp/etextcur.h>
#include <ifound/list.h>
#include <ifound/str.h>
#include <local/osfile.h>
#include <ifound/itypes.h>

// prefijo para los archivos temporarios
#define PREFIX_TMP		"file"
#define PREFIX_REXP		"file.*"

class FileText: public EditableText 
	// Implements file-locking and file-preserving methods on Text
{
	// Make some friends
	friend class FileView;
	
// private:

	static 	String cwdir;		// directorio Corriente de Trabajo
	
	static 	String preserveDir;	// directorio para 'preserve'
	OsFile 	preserveFile;		// osfile para el archivo 'preserve'
	String 	preserveFilename;	// nombre del archivo 'preserve'
	
	Int 	fileperm;			// permiso del archivo abierto

	// Avoid accidental use of these
    void operator=(const FileText&) ;
    FileText(const FileText&) ;

public:

	// FileText Constructors
		FileText(String f, bool isPres);
		~FileText();
    
	// Provide access to file text
	    virtual FileText *fileText() { idCheck(); return this;}

	// Source file management
		void  	setFile(const String &f, bool load_new_file = true);
		const 	String &getTextName();  
		bool 	store();	  		// Stores file
		bool 	load(); 		   	// Loads  file
		bool 	activate();			// Activates text
		void 	deActivate();		// Deactivates text
		bool 	rmFiles();			// Removes preserve copy
		void 	mkFiles();			// Creates preserve copy

    // Source information
		Int   	getFilePerm()						{ return fileperm; }  

	// Preserve file management	
		bool storePreserve();			
		static const String	tempFile(); 
		static const String	&getPreserveDir()		{ return preserveDir; }
		static void setPreserveDir(const String &s)	{ preserveDir = s; }
		static void setCwd(const String &s)			{ cwdir = s; }

	//  X X X X  X
		void addPath();

};

#endif

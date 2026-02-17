/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: fview.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION
* Class definitions for FileView type.
*********************************************************************/

#ifndef FVIEW_H
#define FVIEW_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>
#include <winapp/window.h>
#include <ifound/str.h>
#include <ifound/itypes.h>
#include <ifound/message.h>

#include <winapp/etext.h>
#include <winapp/etextcur.h>
#include <winapp/eview.h>

class FileView : public EditableView 
	// Implements file-locking and file-preserving methods on a View
{       

// private:

	// Used if text has not already been constructed by derived classes
		static Text *constructText(const String &f, bool isTemp);

	// Avoid accidental use of these
	    void operator=(const FileView&) ;
	    FileView(const FileView&) ;

public:

	// FileView Constructors 
		FileView(String f, bool isPres, Text *t=NULL );
		~FileView();

	// Access to FileView
		FileView *fileView() { idCheck(); return this; }

	// Access to FileText
		FileText *fileText(); 
     
	// File management methods
		bool load(String f);
		bool reload();
		bool store();
		void storePreserve();
    
    // Static Preserve methods
		static void setPreserveDir(const String &s);
		static const String &getPreserveDir();
    
    // Text Permisions
		bool isRdOnly();
		bool isLocked();

	// X X X X X X X X
		void addPath();	  // Adds path to filename

};

#endif

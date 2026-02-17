/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: eview.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION
* Class definitions for EditableView type.
*********************************************************************/

#ifndef EVIEW_H
#define EVIEW_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <winapp/window.h>
#include <ifound/str.h>
#include <ifound/itypes.h>
#include <ifound/msgtable.h>

#include <winapp/etext.h>
#include <winapp/etextcur.h>
#include <winapp/view.h>

class EditableView : public View 
	// Implements edition methods and a simple store method on a View
{

// private:

	// Used if text has not already been constructed by derived classes
		static Text *constructText(const String &f);
		static Text *constructText(const String &cmd, bool abortable);

protected:

	// Blocks
		TextCursor *_block;		// marca el pricipio de bloque

public: 
	// Constructors
		EditableView(String f, Text *t = NULL, TextCursor *tc = NULL);
		EditableView(const String &cmd, bool abortable,
					 Text *t = NULL, TextCursor *tc = NULL);
						
		virtual ~EditableView();
	
	// Access to EditableText && EditableTextCursor	
     	EditableText		*edText(); 
		EditableTextCursor	*edCursor(); 
    
	// Access to EditableView
    	virtual EditableView *edView() { idCheck(); return this; }

	//
		virtual void _writeStLine();
 	
	// Edit methods
		bool backSpace();
		bool backTab();
		bool delChar();
		bool delEndOfLine();
		bool delLine();
		bool delWord();
		bool insChar(UnChar c);
		bool insLine(const String &text);
		Int  insNewLine(bool deflt = true);
		bool joinLines();
		void setLine(const String &text);
		bool tab();

	// Attribute management methods
		void setAttribute(TextCursor &tc, Int attr);
		void delAttribute(TextCursor &tc, Int attr);

	// Block management methods
		void		blockOn (bool rw = true);
		void 		blockOff(bool rw = true);
		void 		setBlock(TextCursor &tc);
		void 		delBlock(bool rw=true);
		TextCursor &getBlock();
		bool 		hasBlock();
		bool 		beginBlock();
		void 		delRegion(TextCursor &tc, bool compress = true);
		void 		getRegion(List(String) &l, TextCursor &tc);
		void 		insertText(List(String) &l);

	// File-related methods
		bool store();
	    bool chgFileName(const String &fileName);
		Int  insertFile(const String &fname);

	// Change insert/overwrite mode 
		void chgInsMode(bool redrw);

	// Informational Methods
		bool inInsMode() 		 { return mode&INSERT; }
		bool isTextMod();               
	
	// Can the view be closed ?
		virtual bool exit(bool igntxt = true);

};
#endif

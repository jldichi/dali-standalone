/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: etext.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION
* Class definitions for EditableText type.
*********************************************************************/

#ifndef ETEXT_H
#define ETEXT_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <winapp/text.h>
#include <winapp/etextcur.h>
#include <ifound/list.h>
#include <ifound/str.h>
#include <local/osfile.h>
#include <ifound/itypes.h>


class EditableText: public Text 
	// Implements edition methods and a simple store method on Text
{
	// Make some friends
	friend class TextCursor;
	friend class View;
	friend class EditableTextCursor;
	friend class EditableView;
	
// private:

	// Avoid accidental use of these
    void operator=(const EditableText &);
    EditableText(const EditableText &);

public:

	// EditableText Constructors
	EditableText(String f);
	EditableText(const String &cmd, bool abortable);
	EditableText();
	~EditableText();
    
	// Provide access to editable text
    virtual EditableText *edText() { return this;}

	// Source file management
	bool 	store();	  		// Stores file
	void 	deActivate();		// Deactivates text

	// Block management
	bool insertText(List(String) &l, EditableTextCursor &c);
	bool getRegion(List(String) &l, TextCursor begin, TextCursor end,
				   bool add);
	bool delRegion(EditableTextCursor &begin, TextCursor &end, bool compress,
				   bool correc);

};

#endif

/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: etextcur.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION
* Class definitions for EditableTextCursor type.
*********************************************************************/

#ifndef ETEXTCUR_H
#define ETEXTCUR_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <ifound/list.h>
#include <ifound/str.h>
#include <ifound/itypes.h>
#include <winapp/textcur.h>

#ifndef DEF_LIST_STRING
#define DEF_LIST_STRING
	declare(List, String);
	declare(ListCursor, String);
#endif

class EditableTextCursor: public TextCursor
{
	friend class Text;
	friend class EditableText;
	
// private:

	// Finds spaces needed to perform autoindent
	String findIndentSpace();

public:

	// EditableTextCursor Constructors
	EditableTextCursor();
	EditableTextCursor(Text &txt);
	EditableTextCursor(TextCursor &tc);
	~EditableTextCursor();

    // Provide access to editable text
    EditableTextCursor 	*edTextCur() { return this;}
    
	// Insertion methods
	void 	insString(const String &s);	
	Int		insNewLine(bool indent = true);
	bool	joinLines();
	bool	setChar(char c);	
	char	getChar();					

    // Deletion methods
	void 	del();
	bool 	delRange(Int from, Int to, bool compress = true);
	bool 	delTo(Int row, Int col, 
					bool joinLastEOL = true, bool compress = true);

	// Tab managment methods
	bool 	backTab();		
	void 	delFillTabs();	
	void 	rebuildTab(String &replaced);
	void 	reAssignNextTab(Int col, Int n = 1);
};

#endif

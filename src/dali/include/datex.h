/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: datex.h,v 1.1.1.1 1996/04/16 18:52:55 gustavof Exp $
*
* DESCRIPTION: The routines in this file are for the "troff" mode of 
*              the editor.
*********************************************************************/

#ifndef DATEX_H
#define DATEX_H

#include <ifound/str.h>

#define TEX_TABLE_SIZE		40
class TeXSpecialChar {
private:

	UnChar IdeaChar;
	String TeX;

	void operator=(const TeXSpecialChar&); 
	TeXSpecialChar(const TeXSpecialChar&); 

public:

	TeXSpecialChar()							{ }
	TeXSpecialChar(UnChar ic, const char *tr)	{ IdeaChar = ic; TeX = tr; }

   // Static Members
	static TeXSpecialChar *TeXSpecialCharTab[TEX_TABLE_SIZE];
	static void InitTable();
	static void DestroyTable();
	static Int SearchChar(const UnChar ch, String &str);
	static Int SearchString(const String str, UnChar &ch);
};

#endif

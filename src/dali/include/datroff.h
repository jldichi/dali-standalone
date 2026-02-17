/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: datroff.h,v 1.1.1.1 1996/04/16 18:52:55 gustavof Exp $
*
* DESCRIPTION: The routines in this file are for the "troff" mode of 
*              the editor.
*********************************************************************/

#ifndef TROFF_H
#define TROFF_H

#include <ifound/str.h>

#define TABLE_SIZE		50 
class TroffSpecialChar {
private:

	UnChar IdeaChar;
	String Troff;

	void  operator=  (const TroffSpecialChar&); 
	      TroffSpecialChar(const TroffSpecialChar&); 

public:

	TroffSpecialChar()	{ }
	TroffSpecialChar(UnChar ic, const char *tr)
		{ IdeaChar = ic; Troff = tr; }

	static TroffSpecialChar *TroffSpecialCharTab[TABLE_SIZE];

	// Static Members
	static void InitTable();
	static void DestroyTable();
	static Int SearchChar(const UnChar ch, String &str);
	static Int SearchString(const String str, UnChar &ch);

};

#endif

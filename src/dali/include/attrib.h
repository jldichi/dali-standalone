/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: attrib.h,v 1.1.1.1 1996/04/16 18:52:52 gustavof Exp $
*
*
* DESCRIPTION:
*				Text attribute management classes
*
*********************************************************************/


#ifndef	ATTRIB_H
#define	ATTRIB_H	1

#ifdef DEBUG
#	include <eddebug.h>
#endif

#include <text.h>

class AttributeZones 
{
	useIdCheck(ATTRIBUTEZONES_ID);

private :

	void *b_arr;				// block array

	TextCursor start, end;		// Last accessed zone
	Int attribute;				// Attribute of last accessed zone
	Bool valid_last;			// signal if start/end are valid

public :

	AttributeZones();
	~AttributeZones();

	Int getAttribute(TextCursor &tc, Int &lc);
	void setAttribute(TextCursor &s, TextCursor &e, Int attr);
	void delAttribute(TextCursor &s, TextCursor &e, Int attr);
	void delAttribute(TextCursor &s, Int attr);

	void debug();

};


#endif		//	ATTRIB_H

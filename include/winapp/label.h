/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: label.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION: Class definitions for label type.
*
*********************************************************************/

#ifndef LABEL_H
#define LABEL_H

#include <ifound.h>

#ifdef __GNUC__
#	pragma interface
#endif

#include <ifound/str.h>
#include <winapp/winobj.h>

class Label : public WinObj
{

// private:

	Int		forig;		// origin row
	Int		corig;		// origin column
	String	dato; 		// displayed string
	Int		attr; 		// attribute to print dato
	Int		lblwidth; 	// width of the label

	Int wordWrap(String &line, Int width);
	Label(const Label &);
	
public:

	// constructores y destructores

	Label(const String &l, Int fo=ERR, Int co=ERR, 
						   Int width=ERR, Int a = ERR);
	~Label();

	// metodos virtuales
	virtual bool display(Int fo=ERR, Int co=ERR);
	virtual Int  execute(UnChar ) { assert(0); return REJECT_EXIT; }

	void draw();
	const String &value()	{ idCheck(); return dato; }
	void setValue(const String &newlabel);
};

#endif

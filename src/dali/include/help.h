/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: help.h,v 1.2 1998/11/19 15:11:16 ernestod Exp $
*
* DESCRIPTION
* Interface to help class
*********************************************************************/

#ifndef HELP_H
#define HELP_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

class MsgTable;

// declare de las listas genericas
class gtype(List, String);
class gtype(ListCursor, String);

class Help {

// private:

		static Help *activeHelp;
		static String const fpRead(Int n);
		static String const fpReadMsg(Int n);

		gtype(List, String) &text;
		gtype(ListCursor, String) &tc;

		MsgTable *msgTable;

		Help(const Help &hlp)  ;
		void operator=(const Help &hlp)  ;

public:

		// Constructors & destructors
		Help();
		Help(const String &module);

		virtual ~Help();
		virtual String getIndex(Int index) const;

        bool display(Int item = ERR);
        bool display(const String key);
        bool displayMsg(const String &key);
};

#endif // HELP_H

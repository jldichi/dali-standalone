/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: waitmsg.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Class definitions for message box type.
*
*********************************************************************/

#ifndef WAITMSG_H
#define WAITMSG_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>
#include <winapp/winobj.h>

class WaitMsg : public WinObj
{    
//private:

	String message;
	UnChar abortchar;
	WaitMsg(const WaitMsg &);
				
public:

	WaitMsg(const String &m, const String &l = NULL_STRING, UnChar c = 0);
	virtual ~WaitMsg();

	virtual Int execute(UnChar c = 0);

	bool isAborted();
};

#endif

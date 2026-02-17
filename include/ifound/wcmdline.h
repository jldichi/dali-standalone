/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wcmdline.h,v 1.1.1.1 1996/04/16 18:51:33 gustavof Exp $
*
* DESCRIPTION
* Interface to option parsing class
*********************************************************************/

#ifndef WCMDLINE_H
#define WCMDLINE_H

#include <ifound/oparser.h>

class WinOptionParser : public OptionParser {

	typedef OptionParser inherited;

// private:

    WinOptionParser(const WinOptionParser &);
    void operator=(const WinOptionParser &);

public:

	// Constructors & destructors
	WinOptionParser(const String &vers = NULL_STRING);
	~WinOptionParser();

	virtual bool execute();
};

#endif	// WCMDLINE_H

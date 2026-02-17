/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: imain.h,v 1.1.1.1 1996/04/16 18:51:34 gustavof Exp $
*
* DESCRIPTION
* Class definitions for uniform application startup mechanism
*********************************************************************/

#ifndef IMAIN_H
#define IMAIN_H

#include <ifound/str.h>

class OptionParser;

class ISApp
{

public:

	inline ISApp();
	virtual ~ISApp();
	virtual OptionParser *cmdLine();
	virtual String appName() const;

};

ISApp::ISApp()
{
}

#endif // IMAIN_H

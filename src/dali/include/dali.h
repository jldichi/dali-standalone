// --------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: dali.h,v 1.1.1.1 1996/04/16 18:52:55 gustavof Exp $
//
// Description:
// --------------------------------------------------------------------

#include <ifound/isapp.h>
#include <ifound/str.h>
#include <ifound/oparser.h>

class dali : public ISApp { 

public:

	dali();
	~dali();
	Int execute();
	static OptionParser *cmdLine();	
	static String appName();
		
};

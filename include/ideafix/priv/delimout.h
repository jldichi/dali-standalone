/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: delimout.h,v 1.1.1.1 1996/04/16 18:51:38 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef DELIMITEDOUTPUT_H
#define DELIMITEDOUTPUT_H

#include <ifound/output.h>

class String;

class DelimitedOutput : public BaseOutput {

protected:

	UnShort	dest;		// Output destination
	char fsep, rsep;	// Field and record separators

public:

	DelimitedOutput(Int dest, const String &arg, const String &sep, 
					Int ncopies);
	virtual ~DelimitedOutput();	

	virtual Int operator<<(const char *bp);

	DelimitedOutput *getDelimitedOutput()	 { return this; }
};

#endif

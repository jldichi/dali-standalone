/********************************************************************
* Copyright (c) 1991 longerSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF longerSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: toint.cc,v 1.2 2004/09/29 16:21:10 damianm Exp $
*
* DESCRIPTION
* Convert a 'c' string, asciiz to a long number
*********************************************************************/

#include <stdlib.h>

#include <ifound.h>
#include <ifound/stdc.h>

Int toInt(const char *cs)
{
	return atol(cs);
}

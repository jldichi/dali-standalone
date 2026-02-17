/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numstr.cc,v 1.1.1.1 1996/04/16 18:51:54 gustavof Exp $
*
* DESCRIPTION:
* Implementation of Num class string formatting method
*********************************************************************/

#include <ifound.h>
#include <ifound/itypes.h>

String toString(const Num &num, Int n, Int p)
{ 
	return String(toCharPtr(num, n, p)); 
}


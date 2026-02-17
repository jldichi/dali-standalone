/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: strgenv.cc,v 1.1.1.1 1996/04/16 18:51:54 gustavof Exp $
*
*
* DESCRIPTION: Read Environment variables.
*				
*
*********************************************************************/

#include <ifound/str.h> 
#include <ifound/stdc.h> 
#include <ifound/priv/itypesp.h>
#include <ifound/priv/gfuncs.h>

String String::getEnv() const
{
	if (!VALID_PTR(p))
		return *this;

	return readEnvVar(p->s());
}

void String::putEnv() const
{
	if (VALID_PTR(p))
		putEnvVar(p->s());
} 	
 	

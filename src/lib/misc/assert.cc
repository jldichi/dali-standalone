/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: assert.cc,v 1.3 1998/09/04 18:39:36 mauro Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound.h>
#include <ifound/str.h>

#ifndef __VC__ /* use _assert from standard runtime library */
extern "C" void _assert(char const *ex, char const *file, int line)
{
	error(String("Assertion failed (%s).\n%s:%d.\n"), ex, file, line);
}
#endif

extern "C" void _precond(char const *ex, char const *file, int line)
{
	error(String("Precondition failed (%s).\n%s:%d.\n"), ex, file, line);
}

extern "C" void _postcond(char const *ex, char const *file, int line)
{
	error(String("Postcondition failed (%s).\n%s:%d.\n"), ex, file, line);
}

extern "C" void _abnormal(char const *file, int line)
{
	error(String("Program reached an invalid address at %s:%d.\n"), file, line);
}

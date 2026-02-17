// --------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended
// publication of such source code.
//
// libstamp.cc,v 1.2 1996/04/30 16:15:13 gustavof Exp
//
// Description:
// Routines that implement a uniform application startup mechanism
// --------------------------------------------------------------------------

#include iRootClass

#ifdef __SC__
#	define makegood(a)	quote(a)
#else
#	define makegood(a)	a
#endif
#ifndef COMPILE_USER
#	define COMPILE_USER "unknown"
#endif

#ifndef COMPILE_HOST
#	define COMPILE_HOST	"unknown"
#endif

#ifdef __GNUC__
#	define COMPILE_COMP	"gcc"
#else
#	ifdef __SC__
#		define COMPILE_COMP "Symantec C++"
#	elif __VC__
#		define COMPILE_COMP "Visual C++"
#	else
#		define COMPILE_COMP "unknown"
#	endif
#endif

#ifndef __VERSION__
#	ifdef __SC__
#   	define __VERSION__ __SC__
#	elif __VC__
#		define __VERSION__	"5.0"
#	else
#		define __VERSION__	"?.?.?"
#	endif
#endif

#define COMPILE_DATE	__DATE__
#define COMPILE_TIME	__TIME__

#ifndef PATCH_LEVEL
#	include <ifound/plevel.h>
#endif
static const char compileData[] =
	"User: " makegood(COMPILE_USER) "\n"
	"O.S.: " makegood(COMPILE_HOST) "\n"
#if defined (Linux5) || defined (Linux6) || defined (Linux7)
	"Comp: " makegood(__VERSION__) "\n"
#else
	"Comp: " COMPILE_COMP " v" makegood(__VERSION__) "\n"
#endif
	"Date: " COMPILE_DATE " " COMPILE_TIME "\n"
	"Build : " PATCH_LEVEL "\n";

class Library : public LibraryInfo
{
public:
	Library();
	static void foo();
};

Library::Library()
:	LibraryInfo(makegood(QuoteLibrary), compileData)
{
}

#if defined (LIBINFOSHAREDBUG)
Library *LibVar = NULL;
void
Library::foo()
{
	if (LibVar == NULL)
		LibVar = new Library();
}

#else
static Library LibVar1;
void Library::foo() { }
#endif

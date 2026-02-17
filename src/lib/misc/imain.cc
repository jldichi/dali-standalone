// --------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended
// publication of such source code.
//
// imain.cc,v 1.1.1.1 1996/04/16 18:51:59 gustavof Exp
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

#if defined (IDEAWIN) && ! defined(_CONSOLE)
extern "C" IdeaWinMain(int,char**);
#	define main IdeaWinMain
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
#	else
#		define COMPILE_COMP "unknown"
#	endif
#endif

#ifndef __VERSION__
#	ifdef __SC__
#   	define __VERSION__ __SC__
#	else
#		define __VERSION__	"?.?.?"
#	endif
#endif


#define COMPILE_DATE	__DATE__
#define COMPILE_TIME	__TIME__

#include <ifound/plevel.h>

static const char compileData[] =
	"User: " makegood(COMPILE_USER) "\n"
	"O.S.: " makegood(COMPILE_HOST) "\n"
#if defined (Linux5) || defined (Linux6) || defined (Linux7)
	"Comp: " makegood(__VERSION__) "\n"
#else
	"Comp: " COMPILE_COMP " v" makegood(__VERSION__) "\n"
#endif
	"Date: " COMPILE_DATE " " COMPILE_TIME "\n"
	"P.L.: " PATCH_LEVEL "\n";

void startUp(const String &appName, int &argc, char **&argv);

int
main(int argc, char **argv)
{
	String appName = RootClass::appName();
	if (!appName)
		appName = String(argv[0]).baseName();

	startUp(appName, argc, argv);

#if GUI_APPLICATION

	extern Int executeGUIApplication(const String &appName, bool local);

	return executeGUIApplication(appName,
#if LOCAL_APPLICATION
				true
#else
				false
#endif
			);

#else

	extern void parseAppArgs();

	parseAppArgs();
	return RootClass().execute();

#endif
}

OptionParser *
getAppCmdLine()
{
	return RootClass::cmdLine();
}

String
getAppCompileData()
{
	return compileData;
}

ISApp *
createAppRootClass()
{
	return new RootClass;
}

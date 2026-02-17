/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: banner.cc,v 1.7 2010/01/07 19:29:26 hardaiz Exp $
*
* DESCRIPTION
* identification banner
*********************************************************************/
#include <ideafix.h>
#include <ifound.h>
#include <ifound/str.h>
#include <stdio.h>
#ifdef IDEAWIN
#include <mswi/ideadll.h>
#endif
// Configure this two lines for version identification

static char *libLabel = "@(#)Intersoft C++ lib: $Date: 2010/01/07 19:29:26 $ ";

void banner(const String &prog, const String &version)
{
	String dateTo = String("$Date: 2/3/2015 $").field(1, " ");
	//String tmp = prog.baseFullName() + ": " + version + EXTRA_BANNER_INFO +
	//		 " (" + _OS_ + " " +
	//		_OSVER_ + ") " + "Copyright InterSoft (c)1988-" +
	//		 &IDEA_RELEASE_DATE[6];
			//dateTo(2, 2);

	String tmp = prog.baseFullName() + ": " + version + EXTRA_BANNER_INFO +
			 " (" + _OSVER_ + ") " + "Copyright InterSoft (c)1988-2015";


#ifdef IDEAWIN
	if (GetDLLData() && GetDLLData()->bClient && _GetServerDLLData())
   		SendMessage(_GetServerDLLData()->hAppWnd, WM_ADDSTRING, (WPARAM) STDERR, (LPARAM) toCharPtr(tmp));
	else
		DisplayMsg(0, toCharPtr(tmp));
#else
	fprintf(stderr, "%s\n", toCharPtr(tmp));
#endif
}

/*
	Convert version number to version string.

	For example:

		1100 --> 1.1
		1101 --> 1.1.1
		1112 --> 1.1.12
		4110 --> 4.1.10
*/
String versionStr(Int vn)
{
	String v = toString(vn / 1000) + "." + toString((vn % 1000)/100);
	Int release = vn % 100;
	if (release != 0)
		v += String(".") + toString(release);
	return v;
}

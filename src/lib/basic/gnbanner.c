/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gnbanner.c,v 1.4 2010/01/07 19:22:00 hardaiz Exp $
*
* DESCRIPTION
* identification banner
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*banner			| Print a banner in stderr.	
*IdeaVersion    | Returns a char * with version Id.
*********************************************************************/

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>
#ifdef IDEAWIN
#	include <mswi/ideadll.h>
#endif

/*******************************************************/
/* Configure this line for version identification      */
static char _LIBLABEL_[] = "@(#)IDEAFIX LIB: 4.2 $Date: 2010/01/07 19:22:00 $ ";
/*******************************************************/

void Banner(const char *prog)
{
	_Banner(prog, IdeaVersion());
}

char *IdeaVersion(void)
{
	return _IdeaVersion('\0');
}

void _Banner(const char *prog, char *ideaVer)
{
	char *OpLic;
	char szBanner[128];
	FPCPCPI OldHandler;
	//sprintf(szBanner, "%s: %s (%s %s) Copyright InterSoft (c)1988-%s\n",
	//	prog, ideaVer, _OS_, _OSVER_, &IDEA_RELEASE_DATE[6]);
	sprintf(szBanner, "%s: Ideafix %s [%s] (%s) Copyright InterSoft (c). 1988-2013\n",
		prog, ideaVer, IDEA_RELEASE_DATE, _OSVER_);
#if IDEAWIN
	if (GetDLLData() && GetDLLData()->bClient && _GetServerDLLData())
   		SendMessage(_GetServerDLLData()->hAppWnd, WM_ADDSTRING, (WPARAM) STDERR, (LPARAM) szBanner);
	else
		DisplayMsg(0, szBanner);
#else
    fprintf(stderr, szBanner);

	OldHandler = SetReadEnvHandler((FPCPCPI)NULLFP);
	OpLic = ReadEnv("OPENLIC");
	SetReadEnvHandler(OldHandler);

	if (OpLic && *OpLic) {
		fprintf(stderr,
				"Version generada para uso EXCLUSIVO de: %s\n", OpLic);
		fprintf(stderr,
				"El uso fuera de este ambito dara lugar a las acciones legales correspondientes\n");
	}
#endif
}

char *_IdeaVersion(char dbe)
{

/*
	static char version[100] = "";
	static bool init = FALSE;

	if (!init) {
		// Build version string (brute force) 
		char *p = version;
		if (dbe != '\0')
			*p++ = dbe;
		IToStr(IDEA_VERSION / 100, p);
		p += strlen(p);
		*p++ = '.';
		IToStr((IDEA_VERSION % 100) / 10, p);
		p += strlen(p);
		if (IDEA_VERSION % 10 != 0) {
			*p++ = '.';
			IToStr(IDEA_VERSION % 10, p);
			p += strlen(p);
		}
		strcpy(p, EXTRA_BANNER_INFO);
		p+=strlen(p);
		strcpy(p, " [");
		p += strlen(p);
		strcpy(p, IDEA_RELEASE_DATE);
		p += strlen(p);
		strcpy(p, "]");
		init = TRUE;
	}
	return version;
*/
	return IDEA_VERSION_STRING;
}


/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_start.cc,v 1.24 2010/09/28 22:25:21 hardaiz Exp $
*
* DESCRIPTION
* Program start up
*********************************************************************/

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <ideafix.h>
#include <cfix.h>
#include <cfix/priv/macros.h>
#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/ixsystem.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/priv/pconfig.h>
#include <unistd.h>

// DALI-STANDALONE: Removed cracker/pserconn.h, IxSCA, license checks, password code.
// This is a standalone editor - no user authentication, licensing, or client/server.

extern "C" bool crackerClient();


extern "C" char *timeConversion(int c, const char **, int size, Int *maxwidth,
		    va_list *ap, char *wbuff, char *, Int *)
{
	Int v = size == 'l' ? va_arg(*ap, Int) : va_arg(*ap, int);
	if (*maxwidth == INT_MAX)
		*maxwidth = 0;
	switch (c) {
	case 'T' :
		TToStr(TIME(int((Int(v)))), wbuff, *maxwidth);
		break;
	case 'D' :
		DToStr(DATE(int((Int(v)))), wbuff, *maxwidth);
		break;
	default :
		assert(0);
		break;
	}		
	char *cp = wbuff;
	*maxwidth = INT_MAX;
	return cp;
}


/**
 *
 * void Start(int *_argc  char **argv, int module)
 *
 * This function gets called by Program() and wcmd()
 * and performs initialization.
 *
 * This function  accepts a parameter
 * containing the ID of the program to let the user licenses system
 * know which program is running and work on the licenses based on
 * that information.
 * By default, PKG_RUNTIME is used as the module ID.
 * 
 *
 * module = APPL_AURUS || APPL_DENARIUS || APPL_IANUS etc.
 *
 *
 */

extern "C" void Start(int *_argc, char **argv, int module)
{
	startUp(BaseName(argv[0]), *_argc, argv);

	// DALI-STANDALONE: Removed user license checks (CheckUserLicenses).

	if (!IxSystem::readPermission(appName()))
		_processConfig()->sig = ERR;
	addConvertFunction('T', timeConversion);
	addConvertFunction('D', timeConversion);
}

extern "C" void StartUp(const char *sys, const char *prog, const char *, 
						int *_argc, char **argv)
{
	String fName;
	if (sys == NULL || *sys == '\0')
		fName = prog;
	else
		fName = String(sys) + SYSNAME_SEPAR + prog;		
	startUp(fName, *_argc, argv);
	if (!IxSystem::readPermission(appName()))
		_processConfig()->sig = ERR;
	addConvertFunction('T', timeConversion);
	addConvertFunction('D', timeConversion);
}

extern "C" void SetUpdatePerm(bool v)
{
	procUpdatePerm(v);
}

extern "C" bool GetUpdatePerm(void)
{
	return procUpdatePerm();
}

extern "C" void SetAddPerm(bool v)
{
	procAddPerm(v);
}

extern "C" bool GetAddPerm(void)
{
	return procAddPerm();
}

extern "C" void SetDelPerm(bool v)
{
	procDelPerm(v);
}

extern "C" bool GetDelPerm(void)
{
	return procDelPerm();
}

// DALI-STANDALONE: Removed sendPermsToCracker() - no cracker client/server.

#if HAVE_SAVEMEM
extern "C" bool ProcSaveMem(void)
{
	return procSaveMem();
}
#endif

extern "C" UShort ProcUserId(void)
{
	return procUserId();
}

extern "C" UShort ProcUserGid(void)
{
	return procUserGid();
}

extern "C" Int ProcPid(void)
{
	return procPid();
}

extern "C" const char *ProcTtyName(void)
{
	return toCharPtr(procTtyName());
}

extern "C" const char *ProcTty(void)
{
	return toCharPtr(procTty());
}

extern "C" Int ProcSig(void)
{
	return procSig();
}

extern "C" char **ProcArgs(void)
{
	return procArgs();
}

extern "C" Int ProcNArgs(void)
{
	return argCnt();
}

extern "C" TIME ProcInitTime(void)
{
	return LTIMETOS(toInt(procInitTime()));
}

extern "C" DATE ProcInitDate(void)
{
	return LDATETOS(toInt(procInitDate()));
}
      
extern "C" char COMMA()
{
	return thousandSep();
}

extern "C" char DECPOINT()
{
	return decPoint();
}


// DALI-STANDALONE: Removed entire IxSCA session/login/password section.
// No user authentication in standalone editor.

/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* gpenv.cc,v 1.1.1.1 1996/04/16 18:51:58 gustavof Exp
*
* DESCRIPTION: Read Environment variables.
*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ifound.h>
#include <ifound/str.h>
#include <ifound/stdc.h>

#define STRLEN          256

#if defined(__NT__) && !defined(IDEAWIN)
extern "C" unsigned long __stdcall GetEnvironmentVariableA(const char *, char *, long);
extern "C" unsigned long __stdcall GetLastError();
extern "C" void __stdcall SetLastError(long);

#endif
#ifdef HAVE_SECURITY
static char *_licname(void);
#endif

static const struct DefEnv {
	char *var,
		 *value;
} DefaultEnv [] =
	{
		{ "PATH",		DEFAULT_PATH },
#ifdef __NT__
		{ "IDEAFIX",	"C:\\ideafix" },
#else
		{ "IDEAFIX",	"/usr/local/ideafix" },
#endif
		{ "LANGUAGE",	"spanish" },
		{ "MAXFILES",   "16" },
		{ NULL, NULL }
	};

#if IDEAWIN
#	include <cfix/types.h>
#	include <cfix/st.h>
#	include <mswi/ideadll.h>

static char *_setEnvVar(const char *name, const char *value);
static char *_findEnvVar(const char *name);
static void _putEnvVar(char *name);

#define MAX_VAR         128

typedef struct {
        char *name ;
        char *value;
} envar;

static envar vartab[MAX_VAR] = {NULL};
static char szOptSection[]  = "ENVIRONMENT";

static char *_findEnvVar(const char *name)
{
        int i;
        for (i = 0; i<MAX_VAR; i++) {
                if(vartab[i].name)
                        if(!strcmp(vartab[i].name,name))
                                return vartab[i].value;
        }
        return NULL;
}

static char *_setEnvVar(const char *name, const char *value)
{
        int i;
        for (i = 0; i<MAX_VAR; i++) {
                if(vartab[i].name == NULL) {
                        vartab[i].name  = (char *) tr_malloc(strlen(name) + 1);
                        vartab[i].value = (char *) tr_malloc(strlen(value) + 1);
                        strcpy(vartab[i].name, name);
                        strcpy(vartab[i].value, value);
                        return vartab[i].value;
                }
        }
        return NULL;
}

const char * readEnvVar(const char *x)
{
        char    p[STRLEN] = "\0";
        char    * c;
        sDLL    * pDLL = GetDLLData();

        StrToUpper((char *)x);
        if(c = _findEnvVar(x))
                return c;
        if (pDLL && GetPrivateProfileString(pDLL->ModuleName, x, "", p, STRLEN, IwGenMsg(IDS_INIFNAME)) == 0)
            if (GetPrivateProfileString(szOptSection, x, "", p, STRLEN, IwGenMsg(IDS_INIFNAME)) == 0)

#ifdef __NT__
#	ifndef IDEAWIN
                        GetEnvironmentVariableA(x, p, STRLEN);
#	else
                        GetEnvironmentVariable(x, p, STRLEN);
#	endif
#else
                        if ((c = getenv(x)) != NULL)
                                lstrcpy(p,c);
#endif
		if (p == NULL) {
			for (const DefEnv *ptr = DefaultEnv; ptr->var; ++ptr)
				if (strcmp(x, ptr->var) == 0)
					return ptr->value;
		}

        if (lstrlen(p) != 0)
                return ( _setEnvVar(x,p));

        return NULL;
}

static void _putEnvVar(const char *name)
{
	char *value = strchr(name,'=');
	*value++ = '\0';

	WritePrivateProfileString(szOptSection, name,value, IniFName);
	_setEnvVar(name,value);
}

#else /* IDEAWIN */

const char *readEnvVar(const char *varName)
{
#if HAVE_SECURITY
	if (strcmp(varName, "OPENLIC") == 0)
		return _licname();

	if (strcmp(varName, "empresa") == 0 || strcmp(varName, "company") == 0) {
		char *p = _licname();
		if (p && *p)
			return p;
	}
#endif

#if 0 // __NT__
	static char p[STRLEN];
	const char *ret = NULL;
	SetLastError(0);
	Int ist;
	*p = '\0';
    if((ist = GetEnvironmentVariableA(varName, p, STRLEN) )>0)
		ret = p;
#else
	const char *ret = getenv(varName);
#endif
	if (ret == NULL) {
		for (const DefEnv *ptr = DefaultEnv; ptr->var; ++ptr)
			if (strcmp(varName, ptr->var) == 0)
				return ptr->value;
	}
	return ret;
}
#endif

void putEnvVar(const char *var)
{
	int len = strlen(var);
	if (len > 0) {
		char *tmp = tr_new char[len + 1];
		strcpy(tmp, var);
#ifdef IDEAWIN
		_putEnvVar(var);
#else
		putenv(tmp);
#endif
	}
}

#ifdef HAVE_SECURITY

extern char *defTermType();

static char *_licname(void)
{
	static char licname[65] = { 1 };
	char fileName[80];
	FILE *fp;

	if (licname[0] != 1) return licname;

	sprintf(fileName, "%${IDEAFIX}/data/others");

	if ((fp = fopen(fileName, "r")) == NULL)
		licname[0] = 0;
	else {
		int i = 0;
		int c;
		char *pass = defTermType();
		char workPass[20];
		char *p;

		strcpy(workPass, pass);
		initCrypt(workPass, true);
		while ((c = fgetc(fp)) != EOF)
			licname[i++] = cryptChar(c);

		if (i)
			i--;

		licname[i] = '\0';
		fclose(fp);

		p = licname + strlen(licname) - strlen(pass);
		if (strcmp(p, pass) == 0)
			*p = '\0';
		else
			licname[0] = '\0';
	}

	return licname;
}

#endif /* HAVE_SECURITY */

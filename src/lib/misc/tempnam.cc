#include <unistd.h>
#include <string.h>

#include <ideafix.h> 
#include <ifound.h>
#include <ifound/stdc.h>
#include <ifound/str.h>
#include <sys/stat.h>
#include <ifound/priv/gfuncs.h>

#ifdef HAVE_UNDERSCORE_NAMES
#define	stat	_stat
#endif

#ifdef HAVE_TEMPNAM_BUG
#define DEFAULT_DRIVE "C:"

class String;

#ifdef __NT__
char* GetTmpDir()
{
	char *tempf;
	tempf=(char*)malloc(200);
	strcpy(tempf, getenv("SystemDrive") ? getenv("SystemDrive") : "");
	if (tempf[0] == '\0') {
		strcpy(tempf, DEFAULT_DRIVE);
	}
	strcat(tempf, DEFAULT_TMPDIR);
	return tempf;
}
#endif

char *tempnam(TEMPNAMPARAM1TYPE pth, TEMPNAMPARAM2TYPE pfx)
{
	char* p;
#ifdef HAVE_STATIC_STRING_BUG //f....ing VC++
    String aux;
#else
    static String aux;
#endif	
	aux = NULL_STRING; 
 
    static int a = time(NULL);
	struct stat info;
	a++;

	if (!pth) {
		p = (char *)readEnvVar("TMPDIR");
		if (!p) {
#ifdef __NT__
			p = GetTmpDir();
#else 
			p = DEFAULT_TMPDIR;
#endif
		}
	}
	else
		p = (char *)pth;
		
	aux = toString(p) + String(PATH_SLASH) + toString(pfx) ;
	aux += toString(getpid()+a, 5, 16) +toString(a)(4,5);
	while  (stat(toCharPtr(aux), &info) != ERR)
		aux = toString(toInt(aux) + 1);	
	return toCharPtr(aux);
}

#ifndef __VC__ // use tmpnam and tmpfile from runtime library(msvcrt.dll | libcmt.lib)
char *tmpnam(char  *s)
{
	if (s != NULL)
		return strcpy(s, tempnam(P_tmpdir, ""));

#ifdef HAVE_STATIC_STRING_BUG //f....ing VC++
       String a;
#else
	static String a;
#endif
	a = tempnam(P_tmpdir, "");
	return toCharPtr(a);
}

FILE *tmpfile(void)
{
#ifdef __VC__ //use tempnam instead of tmpnam due to path problem.
     char *p= tempnam(NULL, "");
#else
     char *p = tmpnam(NULL);
#endif
					
	FILE * ft = fopen(p, "w+b");

   	unlink( p );
	
	return ft;
}     
#endif
#endif

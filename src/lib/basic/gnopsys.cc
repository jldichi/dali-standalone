/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* gnopsys.cc,v 1.1.1.1 1996/04/16 18:52:01 gustavof Exp
*
* DESCRIPTION
* General routines related with the Operating System.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
* mkdir			|  Creates a directory
* rmdir			|  Removes a directory
* SetSigInt     |  Set the interrupt handler
****************** UNIX ONLY ***************************
* MemSet        |  Set a memory zone with a value
* AbortHand		|  Generates core
* SetIntChar    |  Define the interrupt character
* SetSigInt
* UserId		|  Retorna el UID del USER_NAME pasado como param.
* UserName		|  Retorna el Name del UID pasado como param.
* GroupId		|  Retorna el GID del GROUP_NAME pasado como param.
* GroupName		|  Retorna el Name del GID pasado como param.
* UserGid		|  Retorna 1
****************** DOS Only ****************************
* getuid		|  Get numerical User ID
* cuserid		|  Get character string User Name
* fopen         |  fopen Routine tailored for networking
* sleep         |  Suspend execution allowing interrupts
* CallIntHandler|  Call the interrupt handler
* UserId		|  Retorna 1
* UserName		|  Retorna 'usuario'
* GroupId		|  Retorna 1
* GroupName		|  Retorna 'grupo'
* UserGid		|  Retorna 1
*
*********************************************************************/

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/c_ixsca.h>
#include <ideafix/priv/c_ina.h>
#include <ifound/priv/gfuncs.h>
#include <signal.h>


#if HAVE_SHORT_MEMSET
/*
 * Problems in some 286 machines that receive an int in 'n'.
 * We use the C library provided function 'cause it is suposed
 * to be optimized.
 */
extern "C" MemSet(char *p, int c, unsigned n)
{
	if (n < 32767)
		memset(p, c, n);
	else {
		memset(p, c, 32767);
		memset(p + 32767, c, n - 32767);
	}
}
#endif // HAVE_SHORT_MEMSET

// extern char* IxSCAUserLogin( int uid ) ;

extern "C" Int GetUid() {
	if(getenv("MSCIUID")!= NULL)
                return atol(getenv("MSCIUID"));//Eternum
        
        else if (getenv("IxSCA_enabled") != NULL) {
		if (strcmp(getenv("IxSCA_enabled"), "true") == 0)
			return IxScaGetUid();
		else return getuid();
	}
	else return getuid();
}

extern "C" Int IxScaGetUid()
{
	if (getenv("IxSCA_enabled") != NULL) {
        if (strcmp(getenv("IxSCA_enabled"), "true") == 0) {
            if (getenv("IxSCA_idUser") != NULL) {
                return atol(getenv("IxSCA_idUser"));
			}
            else return ERR;
        }
		else return ERR;
    }
	else return ERR;
}

extern "C" int UserId(char *name)
{
	if (getenv("IxSCA_enabled")) {
		if (strcmp(getenv("IxSCA_enabled"), "true") == 0)
			return IxScaUserId(name);
		else return userId(name);
	}
	else return userId(name);
}

extern "C" char *UserName(int uid)
{
	static char name[20] = {0};
	memset(name, 0, 20);

	if (getenv("IxSCA_enabled"))
	{
		if (strcmp(getenv("IxSCA_enabled"), "true") == 0)
		{
#ifdef IXSCA
			strncpy(name, IxSCAUserLogin(uid), 19);
#else
			strncpy(name, IxScaUserName(uid), 19);
#endif
			return name;
		}
	}

	strncpy(name, userName(uid), 19);
	return name;
}

extern "C" char *FullUserName(int uid)
{
	if (getenv("IxSCA_enabled")) {
		if (strcmp(getenv("IxSCA_enabled"), "true") == 0) {
/***
* @DIEGOS 2006-11-10 (15:36)
* Ocurre lo mismo en la funcion IxScaUserName.
*
* Estas condiciones de precompilacion se agregan ya que la funcion 'IxSCAFullUserName' solo esta
* definida en la libreria 'libixsca*' (estatica o dinamica) y no la quiero incluir porque no 
* estoy compilando el port con IXSCA.  Entonces para no juntar la libreria 'libidea*' y la 
* 'libixsca' se opto por sacar el codigo de llamado a funcion.
* Si por alguna casualidad se llega a llamar el codigo correspondiente a IXSCA dentro de un port
* compilado sin IXSCA, el programa se interrumpe ejecutando un ASSERT.
*
**/
#ifndef IXSCA
			assert(0);
			return 0 ;
#else
			return IxSCAFullUserName(uid);
#endif
		}
		else return userName(uid); 
	}
	else return fullUserName(uid);
}

extern "C" int GroupId(char *name)
{
	return groupId(name);
}

extern "C" char *GroupName(int gid)
{
	return groupName(gid);
}

extern "C" int UserGid(int uid)
{
	return userGid(uid);
}

int getIxScaIdUser(char *name);
char *getIxScaUserName(Int uid);

extern "C" Int IxScaUserId(char *name) {
	char ixscaEnabled[6] = { 0 };
	
	if (getenv("IxSCA_enabled") != NULL) {
		strncpy(ixscaEnabled, getenv("IxSCA_enabled"), 5);
		for (int i = 0; i < strlen(ixscaEnabled); i++)
			ixscaEnabled[i] = tolower(ixscaEnabled[i]);
		if (strcmp(ixscaEnabled, "true") == 0) {
			if (getenv("IxSCA_idUser") != NULL) {
				if (getenv("IxSCA_UserName") != NULL) {
					if (strcmp(name, getenv("IxSCA_UserName")) == 0) {
						return atol(getenv("IxSCA_idUser"));
					}
					else {
						return getIxScaIdUser(name);
					}
				}
				else return ERR;		
			}
			else return ERR;
		}
		else return ERR;
	}
	return ERR;
}

extern "C" char *IxScaUserName(Int uid) {
	char ixscaEnabled[6] = { 0 };
	char *strUid;
	
	strUid = (char *)malloc(20);
	memset(strUid, 0, 20);

	if (getenv("IxSCA_enabled") != NULL) {
		strncpy(ixscaEnabled, getenv("IxSCA_enabled"), 5);
		for (int i = 0; i < strlen(ixscaEnabled); i++)
			ixscaEnabled[i] = tolower(ixscaEnabled[i]);
		if (strcmp(ixscaEnabled, "true") == 0) {
			if (getenv("IxSCA_UserName") != NULL) {
				if (uid == atol(getenv("IxSCA_idUser"))) {
					return getenv("IxSCA_UserName");
				}
				else {
#ifndef IXSCA
				//	assert(0);
					return InaUserLogin(uid) ;
#else
					return IxSCAUserLogin( uid ) ;
#endif
				}
			}
			else {
				sprintf(strUid, "%ld", uid);
				return strUid;
			}
		}
		else {
			sprintf(strUid, "%ld", uid);
			return strUid;
		}
	}
	sprintf(strUid, "%ld", uid);
	return strUid;
}

#if defined(HAVE_NETAPI)
#define GetUserName GetUserNameA
extern "C" __stdcall GetUserName(char *,unsigned long *);
extern "C" char * currentUser(char *, int);

char *cuserid(char *s, int l)
{
	if(!s || (s && l == 0)) {
		static char userName[64+1];
		unsigned long len = 64;
		currentUser(userName, 64);
		return userName;
	}
	currentUser(s, l);
	return s;
}

unsigned short getuid(void)
{
	return userId(cuserid(NULL));
}

unsigned short getgid(void)
{
	return userGid(getuid());
}

unsigned short geteuid(void)
{
	return getuid();
}

unsigned short getegid(void)
{
	return getgid();
}

int getpw(int userid, char *s)
{
	sprintf(s, "usuario%d", userid);
	return 0;
}

#else
#if !defined(HAVE_USERS)

char *cuserid(char *s, int l)
{
	return "usuario";
}

unsigned short getuid(void)
{
	return 1;
}

unsigned short geteuid(void)
{
	return getuid();
}

int getpw(int userid, char *s)
{
	sprintf(s, "usuario%d", userid);
	return 0;
}
#endif
#endif




FP SetSigInt(FP fp)
{
	return (FP) SigSet(SIGINT, (FPI) fp);
}

#if HAVE_FULL_SIGNALS
#if defined(HAVE_TERMIO_H) || defined(HAVE_TERMIOS_H)
#	if defined(HAVE_TERMIO_H)
#		include <termio.h>
#	else
#		include <termios.h>
#	endif

int SetIntChar(int c)
{
	char save_intr;
#if HAVE_TERMIOS_H
	struct termios t;

	tcgetattr(0, &t);
	save_intr = t.c_cc[VINTR];
	t.c_cc[VINTR] = (unsigned char) c;
	tcsetattr(0, TCSANOW, &t);
#else
	struct termio t;

	ioctl(0, TCGETA, &t);
	save_intr = t.c_cc[VINTR];
	t.c_cc[VINTR] = (unsigned char) c;
	ioctl(0, TCSETAF, &t);
#endif
	return save_intr;
}

#else /* HAVE_TERMIO_H */

#include <sgtty.h>

SetIntChar(int c)
{
	/* toDo: ver como hacer en este caso */
	/* ??????????????????????????? */
}
#endif /* HAVE_TERMIO_H */
#endif /* HAVE_FULL_SIGNALS */

#if NO_MKRMDIR
int mkdir(const char *dirname, int mode)
{
	char command[256];

	sprintf(command, "mkdir %s", dirname);
	if (system(command) < 0)
		return ERR;
	chmod(dirname, mode);
	return OK;
}

int rmdir(const char *dirname)
{
	char command[256];

	sprintf(command, "rmdir %s", dirname);
	if (system(command) < 0)
		return ERR;

	return OK;
}
#endif /* NO_MKRMDIR */

#if NEED_NETBIOS_FOPEN

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <share.h>

/*
 * FOPEN "C" Routine for networking
 * This table defines the open strategy used for fopen,
 * and the sharing modes when in Multi User in the
 * Network environment (when Ideafix MULTI is true).
 */
static struct modes {
	char fmode[3];
	int omode;
	int shflag;
} modes[] = {
	"r",	O_RDONLY, 						SH_DENYNO,
	"r+",	O_RDWR, 						SH_DENYNO,
	"w",	(O_WRONLY|O_CREAT|O_TRUNC),		SH_DENYNO,
	"w+",	(O_RDWR|O_CREAT|O_TRUNC),		SH_DENYNO,
	"a",	(O_WRONLY|O_CREAT|O_APPEND),	SH_DENYNO,
	"a+",	(O_RDWR|O_CREAT|O_APPEND),		SH_DENYNO,
	"",		0,
};

extern int errno;

FILE *fopen(char const *name, char const *mode)
{
	register struct modes *mp;
	register int fd;
	char auxmode[4];
	char *p;
	int  omode = 0;
	int  aux;
	bool MULTI = false;
	FILE *fp;
	FPCPCPI  save;

	/* Save the original mode, 'cause we will hack with it */
	strncpy(auxmode, mode, 3);
	auxmode[3] = '\0';

	/* Strip the 'b' or 't' from the mode */
	aux = strlen(auxmode) - 1;
	switch (auxmode[aux])  {
	case 't':
		omode |= O_TEXT; auxmode[aux] = '\0'; break;
	case 'b':
		omode |= (O_BINARY | EXTBIN_MODE); auxmode[aux] = '\0'; break;
	}

	/* Find the mode 				*/
	for (mp = modes; ; ++mp) {
		if (*mp->fmode == 0) {
			errno = EINVAL;
			return NULL;
		}
		if (strcmp(mp->fmode, auxmode) == 0) {
			omode |= mp->omode;
			break;
		}
	}

	/* Guess if we are in Multiuser mode			*/
	save = SetReadEnvHandler(NULL);

	if ((p = ReadEnv("MULTI")) != NULL) MULTI = bool(atoi(p));
	SetReadEnvHandler(save);

	/* Now open the file			*/
	if (MULTI)
		fd = sopen(name, omode, mp->shflag, S_IREAD|S_IWRITE);
	else
		fd = open(name, omode, S_IREAD|S_IWRITE);

	/* If failed return 			*/
	if (fd == -1) return NULL;

	/* Try to fdopen 				*/
	if ((fp = fdopen(fd, (char *) mode)) == NULL)
		close(fd);

	/* Return the file pointer		*/
	return fp;
}
#endif


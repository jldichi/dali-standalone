/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $id$
*
* DESCRIPTION
* General routines related with the Operating System.
*********************************************************************/
#include <ifound.h>
#include <ifound/stdc.h>
#include <ifound/str.h>

#include <local/slog.h>

#if defined(HAVE_USERS) && !defined(HAVE_NETAPI) && !defined(NHAVE_USERS)

#include <pwd.h>
#include <grp.h>

int userId(const char *name)
{
	struct passwd *pas;
		
	if ((pas = getpwnam(name)) == NULL) return ERR;
	return pas->pw_uid;
}

char *userName(int uid)
{
	struct passwd *pas;

	if ((pas = getpwuid(uid)) == NULL) {
		static char xaux[8] = { '\0' };
		sprintf(xaux, "%d", uid);
		return xaux;
	}
	return pas->pw_name;
}

char *userHomeDir(int uid)
{
	struct passwd *pas;
	if ((pas = getpwuid(uid)) == NULL) {
		static char xaux[8] = { '\0' };
		sprintf(xaux, "%d", uid);
		return xaux;
	}
	return pas->pw_dir;
}

char *fullUserName(int uid)
{
	struct passwd *pas;
	if ((pas = getpwuid(uid)) == NULL) {
		static char xaux[8] = { '\0' };
		sprintf(xaux, "%d", uid);
		return xaux;
	}
#if HAVE_PW_COMMENT
	return pas->pw_comment;
#else
	return pas->pw_gecos;
#endif
}

int groupId(const char *name)
{
	struct group *grp;
	if ((grp = getgrnam(name)) == NULL) return ERR;
	return grp->gr_gid;
}

char *groupName(int gid)
{
	struct group *grp;
	if ((grp = getgrgid(gid)) == NULL) {
		static char xaux[8] = { '\0' };
		sprintf(xaux, "%d", gid);
		return xaux;
	}
	return grp->gr_name;
}

int userGid(int uid)
{
	struct passwd *pas;
	if ((pas = getpwuid(uid)) == NULL) return ERR;

	return pas->pw_gid;
}

#endif

// I'not sure (yet !) if leave NT with or without users
// yet I'm sure: NT have users (and they are domain users!)
// but Win3.x || Win95 doesn't have it.(NHAVE_USERS defined)

#if defined(HAVE_NETAPI) && !defined (NHAVE_USERS)

#include <windows.h>
#include <lmaccess.h>
#include <lmserver.h>
#include <lmerr.h>
#include <lmapibuf.h>
#include <cfix/types.h>

struct _userInfo {
	char name[32];
	char fname[64];
	DWORD uid;
	DWORD   gid;
	char home[128];
};

typedef _userInfo userInfo;
typedef _userInfo *puserInfo;

struct _groupInfo {
	char name[32];
	DWORD gid;
};
typedef _groupInfo groupInfo;
typedef _groupInfo *pgroupInfo;

struct _localGroupInfo {
	char name[32];
};

typedef _localGroupInfo localGroupInfo;
typedef _localGroupInfo *plocalGroupInfo;

struct _serverInfo {
	char name[128];
};

typedef _serverInfo serverInfo;
typedef _serverInfo *pserverInfo;

static pserverInfo ps = NULL;
static puserInfo pu = NULL;
static pgroupInfo pg = NULL;
static plocalGroupInfo pl = NULL;
static puserInfo pglobal_uinfo = NULL;

static Int nUsers = -1;
static Int nGroups = -1;
static Int nLocalGroups = -1;
static Int nServers = -1;

extern "C" {
#ifndef __VC__
	static BOOL (WINAPI *fpNetUserEnum)     (
	IN  LPWSTR     servername OPTIONAL,
	IN  DWORD      level,
	IN  DWORD      filter,
	OUT LPBYTE     *bufptr,
	IN  DWORD      prefmaxlen,
	OUT LPDWORD    entriesread,
	OUT LPDWORD    totalentries,
	IN OUT LPDWORD resume_handle OPTIONAL

	)  = NULL;

	static NET_API_STATUS (WINAPI *fpNetUserGetInfo) (
	IN  LPWSTR     servername OPTIONAL,
		IN      LPWSTR     username,
	IN  DWORD      level,
	OUT LPBYTE     *bufptr
	)  = NULL;

	static NET_API_STATUS (WINAPI *fpNetApiBufferAllocate)(
		IN      DWORD      ByteCount,
	OUT     LPVOID     *Buffer
	)  = NULL;

	static NET_API_STATUS (WINAPI *fpNetApiBufferFree)(
	IN      LPVOID     Buffer
	)  = NULL;

	static BOOL (WINAPI *fpNetGroupEnum)    (
	    IN  LPWSTR      servername OPTIONAL,
	IN  DWORD       level,
	OUT LPBYTE      *bufptr,
	IN  DWORD       prefmaxlen,
	OUT LPDWORD     entriesread,
	OUT LPDWORD     totalentries,
	IN OUT LPDWORD  resume_handle OPTIONAL
    )   = NULL;

	static BOOL     (WINAPI *fpNetUserGetLocalGroups)        (
	IN  LPWSTR    servername OPTIONAL,
	IN  LPWSTR    username,
	    IN  DWORD     level,
	IN  DWORD     flags,
	    OUT LPBYTE    *bufptr,
	IN  DWORD     prefmaxlen,
	    OUT LPDWORD   entriesread,
	OUT LPDWORD   totalentries
    )   = NULL;

#endif
	extern char *cuserid(char *s, int l = 0);
}

static bool debugEnable()
{
	String dbg = "debug_en";
	return toInt(dbg.getEnv()) == 1 ? true: false;
}

#ifdef __VC__  //static dynamic linking(as opposed as runtime dynamic linking(netapi32.lib))
#       define fpNetUserEnum NetUserEnum
#       define fpNetUserGetInfo NetUserGetInfo
#       define fpNetGroupEnum NetGroupEnum
#       define fpNetUserGetLocalGroups NetUserGetLocalGroups
#endif

/* browseDC search for a list of PDC and BDCs available in the domain
 * At least a PDC is mandatory, otherwise nServers will be zero and
 * getUserInfo will request the information to localhost.
 */


void browseDC()
{
	DWORD totalservers;

	totalservers = 0;

	if (nServers < 0) {
		NET_API_STATUS result = 0;
		LPBYTE dc;
		if ((result = NetGetAnyDCName (
				NULL,
				NULL,
				&dc)) != NERR_Success) {
			NetApiBufferFree(&dc);
			sysWarning("Libidea: browseDC: NetGetAnyDCName", "(%d) %s", result, formatErrorCode(result));
		} else 
			nServers=1;

		if (nServers > 0)
			ps = (pserverInfo)malloc (sizeof(serverInfo) * (nServers + 1));

		for(int i = 0; i< nServers; i++) {
			WideCharToMultiByte(
					CP_ACP,
					0,
					(LPCWSTR)dc,
					-1,
					ps[i].name,
					128,
					NULL,
					NULL);
		}
		NetApiBufferFree(&dc);
	}

	if (nServers < 0)
		int result = 0;
}

static bool getUserList()
{
	char *PDC = NULL;
	WCHAR server[128];
	WCHAR _user_name[128];
	LPUSER_INFO_3 ui = NULL;          // User structure
	PGROUP_INFO_2 gi = NULL;
	PLOCALGROUP_INFO_0 lgi = NULL;
	DWORD total;
	int i;
#ifndef __VC__
	HANDLE hNetLib = NULL;

	if(hNetLib == NULL)
		if((hNetLib=LoadLibrary("NETAPI32.DLL")) != NULL){
			(FARPROC)fpNetUserEnum                          = GetProcAddress(hNetLib,"NetUserEnum");
			(FARPROC)fpNetUserGetInfo                       = GetProcAddress(hNetLib,"NetUserGetInfo");
			(FARPROC)fpNetGroupEnum                         = GetProcAddress(hNetLib,"NetGroupEnum");
			(FARPROC)fpNetUserGetLocalGroups        = GetProcAddress(hNetLib,"NetUserGetLocalGroups");
	    }

	if(!fpNetUserEnum || !fpNetGroupEnum){
		FreeLibrary(hNetLib);
		nUsers = nGroups =0;
		return false;
	}
#endif

	if (nServers < 0) browseDC();

	if(nUsers < 0) {
		nUsers = 0;
		for (Int i = 0; i < nServers && nUsers <= 0; i++) {
			total = 0;
			// To translate de PDC name to Unicode we consider that if PDC is NULL,
			// the list must be obtained from the local machine.
			MultiByteToWideChar( CP_ACP, 0, ps[i].name,
					-1, (LPWSTR)server, sizeof(server));

			/* First, we call NetUserEnum with 0x0 in the buffer boundary
			 * only to obtain the total of accounts.
			 */
			(void)fpNetUserEnum (
					(LPWSTR)server,
					3,
					FILTER_NORMAL_ACCOUNT,
					(LPBYTE *)&ui,
					0x0,
					(DWORD *)&nUsers,
					&total,
					NULL);

			if (total <= 0) {
				if (debugEnable()) {
			sysError("Libidea",
							"No se encontro una lista de usuarios en: %s\n",
					ps[i].name);
			}
		nUsers = 0;
		continue;
	    }

			/* Now we calculate the buffer size ... */
			DWORD prefmaxlen = (total + 2) * sizeof(userInfo);

			/* ... and recall NetUserEnum with the total buffer size. */
			int ress = 0;
			if ((ress = fpNetUserEnum (
					(LPWSTR)server,
					3,
					FILTER_NORMAL_ACCOUNT,
					(LPBYTE *)&ui,
					prefmaxlen,
					(DWORD *)&nUsers,
					&total,
					NULL)) != 0) {

				if (debugEnable())
					sysError("Libidea", "(%d) %s", ress, formatErrorCode(ress));

				if (ress != ERROR_MORE_DATA) {
					nUsers = 0;
					continue;
				}
			}
		}

		if (nUsers <= 0) return false;

		pu = (puserInfo)malloc (sizeof(userInfo) * (nUsers+1));

		for(i = 0; i < nUsers; i++) {
			WideCharToMultiByte( CP_ACP, 0, ui[i].usri3_name,
				-1, pu[i].name, 32, NULL, NULL );
			WideCharToMultiByte( CP_ACP, 0, ui[i].usri3_full_name,
				-1, pu[i].fname, 64, NULL, NULL );
			WideCharToMultiByte( CP_ACP, 0, ui[i].usri3_home_dir,
				-1, pu[i].home, 128, NULL, NULL );
			pu[i].uid = ui[i].usri3_user_id;
			pu[i].gid = ui[i].usri3_primary_group_id;
		}
	}
	if(nGroups < 0) {
		nGroups = 0;
		if(fpNetGroupEnum ((LPWSTR)server,2,(LPBYTE *)&gi,0xFFFF,
				(DWORD *)&nGroups,&total,NULL)){
#ifndef __VC__
			FreeLibrary(hNetLib);
#endif
			return false;
		}
		pg = (pgroupInfo)malloc (sizeof(groupInfo) * (nGroups+1));
		for(i = 0; i< nGroups; i++) {
			WideCharToMultiByte( CP_ACP, 0, gi[i].grpi2_name,
				-1, pg[i].name, 32, NULL, NULL );
			pg[i].gid = gi[i].grpi2_group_id;
		}

	}
	if ( nLocalGroups < 0 )
	{
		nLocalGroups = 0;
		WCHAR  wszUserName[256];           // Unicode user name
		// Convert ASCII user name and domain to Unicode.
		char *_uName = cuserid(NULL);
		MultiByteToWideChar( CP_ACP, 0, _uName,
			strlen(_uName)+1, wszUserName, sizeof(wszUserName) );

		if(fpNetUserGetLocalGroups ((LPWSTR)server,wszUserName,0,
				LG_INCLUDE_INDIRECT,(LPBYTE *)&lgi,
				0xFFFF,(DWORD *)&nLocalGroups,&total)){
#ifndef __VC__
			FreeLibrary(hNetLib);
#endif
			return false;
		}
		pl = (plocalGroupInfo)malloc (sizeof(localGroupInfo) * (nLocalGroups+1));
		for(i = 0; i< nLocalGroups; i++) {
			WideCharToMultiByte( CP_ACP, 0, lgi[i].lgrpi0_name,
				-1, pl[i].name, 32, NULL, NULL );
		}
	}
#ifndef __VC__
	FreeLibrary(hNetLib);
	hNetLib = NULL;
#endif
	return true;
}

static bool getUserInfo_(const char * _usrname, const char * PDC)
{
	WCHAR server[128];
	WCHAR _user_name[128];
	LPUSER_INFO_3 ui = NULL;          // User structure
	PGROUP_INFO_2 gi = NULL;
	PLOCALGROUP_INFO_0 lgi = NULL;
	DWORD total;
	int i;
#ifndef __VC__
	HANDLE hNetLib = NULL;

	SetLastError(0);

	if(hNetLib == NULL)
		if((hNetLib=LoadLibrary("NETAPI32.DLL")) != NULL){
			(FARPROC)fpNetUserEnum                          = GetProcAddress(hNetLib,"NetUserEnum");
			(FARPROC)fpNetUserGetInfo                       = GetProcAddress(hNetLib,"NetUserGetInfo");
			(FARPROC)fpNetGroupEnum                         = GetProcAddress(hNetLib,"NetGroupEnum");
			(FARPROC)fpNetUserGetLocalGroups        = GetProcAddress(hNetLib,"NetUserGetLocalGroups");
	    }

	if(!fpNetUserEnum || !fpNetGroupEnum){
		FreeLibrary(hNetLib);
		nUsers = nGroups =0;
		return false;
	}
#endif
	// To translate de PDC name to Unicode we consider that if PDC is NULL,
	// the list must be obtained from the local machine.
	MultiByteToWideChar( CP_ACP, 0, PDC ? PDC : "",
				-1, (LPWSTR)server, sizeof(server));
	MultiByteToWideChar( CP_ACP, 0, _usrname,
				-1, (LPWSTR)_user_name, sizeof(_user_name));

	int ress = 0;
	if ((ress = fpNetUserGetInfo (
					PDC ? (LPWSTR)server : NULL,
					(LPWSTR)_user_name,
					3,
					(LPBYTE *)&ui)) != 0) {
		NetApiBufferFree(ui);
		if (debugEnable())
			sysError("Libidea", "Account search failed on (%s), reason %s. User: %s",
				PDC, formatErrorCode(ress), _usrname);
#ifndef __VC__
		FreeLibrary(hNetLib);
#endif
		return false;
	}
	if (!pglobal_uinfo)
		pglobal_uinfo = (puserInfo)malloc (sizeof(userInfo) * 2);

	WideCharToMultiByte( CP_ACP, 0, ui->usri3_name,
		-1, pglobal_uinfo->name, 32, NULL, NULL );
	WideCharToMultiByte( CP_ACP, 0, ui->usri3_full_name,
		-1, pglobal_uinfo->fname, 64, NULL, NULL );
	WideCharToMultiByte( CP_ACP, 0, ui->usri3_home_dir,
		-1, pglobal_uinfo->home, 128, NULL, NULL );
	pglobal_uinfo->uid = ui->usri3_user_id;
	pglobal_uinfo->gid = ui->usri3_primary_group_id;

	NetApiBufferFree(ui);

	if(nGroups < 0) {
		nGroups = 0;
		if(fpNetGroupEnum ((LPWSTR)server,2,(LPBYTE *)&gi,0xFFFF,
				(DWORD *)&nGroups,&total,NULL)){
			NetApiBufferFree(gi);
#ifndef __VC__
			FreeLibrary(hNetLib);
#endif
			return false;
		}
		pg = (pgroupInfo)malloc (sizeof(groupInfo) * (nGroups+1));
		for(i = 0; i< nGroups; i++) {
			WideCharToMultiByte( CP_ACP, 0, gi[i].grpi2_name,
				-1, pg[i].name, 32, NULL, NULL );
			pg[i].gid = gi[i].grpi2_group_id;
		}
		NetApiBufferFree(gi);
	}

	if ( nLocalGroups < 0 ) {
		nLocalGroups = 0;
		WCHAR  wszUserName[256];           // Unicode user name
		// Convert ASCII user name and domain to Unicode.
		char *_uName = cuserid(NULL);
		MultiByteToWideChar( CP_ACP, 0, _uName,
			strlen(_uName)+1, wszUserName, sizeof(wszUserName) );

		if(fpNetUserGetLocalGroups ((LPWSTR)server,wszUserName,0,
				LG_INCLUDE_INDIRECT,(LPBYTE *)&lgi,
				0xFFFF,(DWORD *)&nLocalGroups,&total)){
			NetApiBufferFree(lgi);
#ifndef __VC__
			FreeLibrary(hNetLib);
#endif
			return false;
		}
		pl = (plocalGroupInfo)malloc (sizeof(localGroupInfo) * (nLocalGroups+1));
		for(i = 0; i< nLocalGroups; i++) {
			WideCharToMultiByte( CP_ACP, 0, lgi[i].lgrpi0_name,
				-1, pl[i].name, 32, NULL, NULL );
		}
		NetApiBufferFree(lgi);
	}
#ifndef __VC__
	FreeLibrary(hNetLib);
	hNetLib = NULL;
#endif
	return true;
}

/* getUserInfo retrieve user information loading relevant data into the
 * the global structure global_uinfo. */

static bool getUserInfo(const char* name)
{
 	String logonserver=String("LOGONSERVER").getEnv();
	
	if (logonserver && getUserInfo_(name,toCharPtr(logonserver)))
			return true;

	if (nServers < 0) browseDC();

	for (Int i = 0; i < nServers; i++) {
		if (getUserInfo_(name, ps[i].name))
			return true;
	}

	if (!getUserInfo_(name, NULL))
		return false;
	else
		return true;
}

extern "C" int userId(const char *name)
{
	if (strlen(name) > 0) {

		if (pglobal_uinfo)
			if (!strcmpi(pglobal_uinfo->name, name))
				return (int) pglobal_uinfo->uid;

		if (!getUserInfo(name)) return 0;

		assert(pglobal_uinfo);

		if (pglobal_uinfo->name == NULL) return 0;

		if(!strcmpi(pglobal_uinfo->name, name))
				return (int) pglobal_uinfo->uid;
	}
	return 0;
}

extern "C" char *userName(int uid)
{
	if (!pglobal_uinfo)
		(void)getUserInfo(currentUser(NULL, 0));

	if(pglobal_uinfo->uid == uid)
		return (char *) pglobal_uinfo->name;

	if (nUsers < 0)
		(void)getUserList();

	for (int i=0; i < nUsers; i++)
		if(pu[i].uid == uid)
			return pu[i].name;

	return "unknown";
}

extern "C" char *fullUserName(int uid)
{
	if (!pglobal_uinfo)
		getUserInfo(currentUser(NULL, 0));

	if(pglobal_uinfo->uid == uid)
		return (char *) pglobal_uinfo->fname;

	if (nUsers < 0)
		(void)getUserList();

	for (int i=0; i < nUsers; i++)
		if(pu[i].uid == uid)
			return pu[i].fname;

	return "Unknown User";
}

extern "C" char *userHomeDir(int uid)
{
	if (!pglobal_uinfo)
		(void)getUserInfo(currentUser(NULL, 0));

	if(pglobal_uinfo->uid == uid)
		return (char *) pglobal_uinfo->home;
		// toDo : if (!homedir[0]) return somethig_suitable;

	if (nUsers < 0)
		(void)getUserList();

	for (int i=0; i < nUsers; i++)
		if(pu[i].uid == uid)
			return pu[i].home;

	return NULL_STR;
}

extern "C" int groupId(const char *name)
{
	int i;
	if (!pglobal_uinfo)
		(void)getUserInfo(currentUser(NULL, 0));

	for (i=0;i<nGroups;i++)
		if(!strcmpi(pg[i].name,name))
			return (int) pg[i].gid;
	return 0;
}

extern "C" char *groupName(int gid)
{
	int i;
	if (!pglobal_uinfo)
		(void)getUserInfo(currentUser(NULL, 0));

	for (i=0;i<nGroups;i++)
		if(pg[i].gid == gid)
			return (char *) pg[i].name;
	return NULL_STR;
}

extern "C" int userGid(int uid)
{
	if (!pglobal_uinfo)
		(void)getUserInfo(currentUser(NULL, 0));

	if(pglobal_uinfo->uid == uid)
		return  pglobal_uinfo->gid;

	if (nUsers < 0)
		(void)getUserList();

	for (int i=0; i < nUsers; i++)
		if(pu[i].uid == uid)
			return pu[i].gid;

	return 0;
}

extern "C" bool amIinLocalGroup(char *grp)
{
	int i;
	if (!pglobal_uinfo)
		(void)getUserInfo(currentUser(NULL, 0));

	for (i=0;i<nLocalGroups;i++)
		if(!strcmpi(pl[i].name,grp))
			return  true;
	return false;
}

extern "C" int getFileOwner(const char *fname)
{
	PSID ps;
	BOOL def;
	DWORD dl=256;
	DWORD len=1000;
	char dom[256];
	char buff[1000];
	PSECURITY_DESCRIPTOR pd=(PSECURITY_DESCRIPTOR) buff;
	SID_NAME_USE snu;

	if(!GetFileSecurity(fname, (SECURITY_INFORMATION) OWNER_SECURITY_INFORMATION,
		pd,len,&len) || ! IsValidSecurityDescriptor(pd))
		return 0;

	if(!GetSecurityDescriptorOwner(pd,&ps,&def) || def)
		return getuid();

	char acct[32];
	len = 32;
	if(!LookupAccountSid(NULL,ps,acct,&len,dom, &dl,&snu))
		return getuid();
	if(snu == SidTypeUser)
		return userId(acct);
	return amIinLocalGroup(acct) ? getuid() : 0;

}

extern "C" char * currentUser(char *usname, int usname_len)
{
	HANDLE thisProcess = GetCurrentProcess();
	HANDLE token;
	TOKEN_USER *User;
	static char Name_user[80], domainName[80];
	DWORD userNamelen, domainNamelen, usersize;
	SID_NAME_USE sidtype;

	if (!OpenProcessToken(
			thisProcess,
			TOKEN_QUERY,
			&token))
		if (debugEnable())
			sysWarning("Libidea", "OpenProcessToken,Error: %s",
					getLastError());

	GetTokenInformation(
			token,
			TokenUser,
			NULL,
			0,
			&usersize);

	User = (TOKEN_USER *) malloc (usersize);

	if (!GetTokenInformation(
			token,
			TokenUser,
			User,
			usersize,
			&usersize))
		if (debugEnable())
			sysError("Libidea", "GetTokenInformation, Error: %s",
					getLastError());


	domainNamelen = 80;

	if (!usname || (usname && usname_len == 0)){
		userNamelen = 80;
		if (!LookupAccountSid(
				NULL,
				User->User.Sid,
				Name_user,
				&userNamelen,
				domainName,
				&domainNamelen,
				&sidtype))
			if (debugEnable())
				sysError("Libidea", "Error, LookupAccountSid: %s\n",
						getLastError());
		free(User);
		return Name_user;
	}
	else {
		userNamelen = usname_len;
		if (!LookupAccountSid(
				NULL,
				User->User.Sid,
				usname,
				&userNamelen,
				domainName,
				&domainNamelen,
				&sidtype))
			if (debugEnable())
				sysError("Libidea", "Error, LookupAccountSid: %s\n",
						getLastError());

		free(User);
		return usname;
	}
}

extern "C" int getuId()
{
	DWORD size_id=0;
	char *buff=NULL;

	GetUserName(buff,&size_id);
	buff= (char *)malloc(size_id);
	assert(GetUserName(buff,&size_id));
	return userId(buff);
}

#ifndef __VC__
#	include <pwd.h>
#else
#	include <ifound/ntadic.h>
#endif
static int usr_curr = 0;

extern "C" struct passwd * getpwent(void)
{
	if(nUsers < 0) {
		getUserList();
		usr_curr = 0;
	}
	if (usr_curr < nUsers)
	{
		struct passwd * ps = (passwd *)malloc (sizeof(struct passwd));

		ps->pw_name = new char[strlen(pu[usr_curr].name)];
		strcpy(ps->pw_name, (char*)pu[usr_curr].name);

		ps->pw_passwd = NULL;
		ps->pw_uid = pu[usr_curr].uid;
		ps->pw_gid = pu[usr_curr].gid;
		ps->pw_age = NULL;
		ps->pw_comment = NULL;

		ps->pw_gecos = new char[strlen(pu[usr_curr].fname)];
		strcpy(ps->pw_gecos, (char*)pu[usr_curr].fname);

		ps->pw_dir = new char[strlen(pu[usr_curr].home)];
		strcpy(ps->pw_dir, (char*)pu[usr_curr].home);

		ps->pw_shell = NULL;
		usr_curr++;
		return ps;
	}
	return NULL;
}

extern "C" struct passwd * getpwuid(uid_t uid)
{
	if(nUsers < 0)
		getUserList();

	for(int usr_cursor = 0; usr_cursor < nUsers; usr_cursor++)
		if (uid == pu[usr_cursor].uid) {
			struct passwd * ps = (passwd *)malloc (sizeof(struct passwd));

			ps->pw_name = new char[strlen(pu[usr_cursor].name)];
			strcpy(ps->pw_name, (char*)pu[usr_cursor].name);

			ps->pw_passwd = NULL;
			ps->pw_uid = pu[usr_cursor].uid;
			ps->pw_gid = pu[usr_cursor].gid;
			ps->pw_age = NULL;
			ps->pw_comment = NULL;

			ps->pw_gecos = new char[strlen(pu[usr_cursor].fname)];
			strcpy(ps->pw_gecos, (char*)pu[usr_cursor].fname);

			ps->pw_dir = new char[strlen(pu[usr_cursor].home)];
			strcpy(ps->pw_dir, (char*)pu[usr_cursor].home);

			ps->pw_shell = NULL;
			return ps;
		}
	return NULL;
}


extern "C" void setpwent(void)
{
	if(nUsers < 0)
		getUserList();
	usr_curr = 0;
}

extern "C" void endpwent(void)
{
	usr_curr = nUsers;
}

#elif !HAVE_USERS || NHAVE_USERS

extern "C" int getFileOwner(const char *fname)
{
	return 0;
};

extern "C" char * currentUser(char *usname, int usname_len)
{
	
	return "Unknown User";
};

int userId(const char *)
{
	return 0;
}

char *userName(int)
{
	return "unknown";
}

char *fullUserName(int)
{
	return "Unknown User";
}

char *userHomeDir(int uid)
{
	return PATH_SLASH;
}

int groupId(const char *)
{
	return 0;
}

char *groupName(int)
{
	return "unknown";
}

int userGid(int)
{
	return 0;
}

extern "C" int getuId()
{
	return 0;
}

#endif

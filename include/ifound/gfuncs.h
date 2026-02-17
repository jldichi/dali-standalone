/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* gfuncs.h,v 1.1.1.1 1996/04/16 18:51:33 gustavof Exp
*
* DESCRIPTION
* General purpose function prototypes
*********************************************************************/

#ifndef GFUNCS_H
#define GFUNCS_H

#include <ifound/osdep.h>
#include <ifound/functype.h>
#include <ifound/macros.h>
#include <ifound/btypes.h>
#include <ifound/istdarg.h>

#define UNAME_ERROR		0
#define UNAME_CHECK		1
#define UNAME_NOCHECK	2
#define UNAME_OLDSHAD	3

//typedef char *(*ConvertFunction)(int, const char **, int, Int *, va_list *,
typedef char *(*ConvertFunction)(int, const char **, int, Int *, va_list ,
		char *, char *, Int *);

class String;
class Time;
class Date;
class OptionParser;
class Num;
class GUIConnection;

// toDo: Dudosas
void turnIntoDaemon(IFPICCPCCP fatalHandler = NULL,
					IFPICCPCCP dispErrHandler = NULL,
					IFPICCPCCP dispHandler = NULL);
void setEnvironment(const char *file, bool mandatory = false);
void banner(const String &prog, const String &version);
String versionStr(Int vers);
// toDo: Duplicadas? procTty() y procPid()?
const String &getTty();
Int getPid();
int unlinkFile(const String &file);
#ifdef HAVE_UNDERSCORE_NAMES
int statFile(const String &file, struct _stat *st = NULL);
#else
int statFile(const String &file, struct stat *st = NULL);
#endif
Time timeStamp(const String &file);
bool renameFile(const String &oldName, const String &newName);

#ifdef toascii
# undef toascii
#endif

extern "C" {
	extern DLLIMPORT int tolower(int x);
	extern DLLIMPORT int toupper(int x);
	extern int toascii(int);
	extern int fromascii(int);
}

void startUp(const String &applName, int &argc, char **&argv,
		OptionParser &);
void startUp(const String &applName, int &argc, char **&argv);
extern "C" void CatchFd60();

IFPICCPCCP displayHandler(IFPICCPCCP handler);
IFPICCPCCP displayErrorHandler(IFPICCPCCP handler);
IFPICCPCCP fatalErrorHandler(IFPICCPCCP handler);
IFPICCPCCP ariErrorHandler(IFPICCPCCP handler);

int ariError(const String &msg, ...);
void error(const String &msg, ...);
void warning(const String &msg, ...);
void notice(const String &msg, ...);
void msgCtrl(int flag);
void display(const String &label, const String &msg);
void displayError(const String &msg);
Int fatalError(const String &msg);


const String &appName();
const String &sysName();
void setAppName(const String &sys, const String &prg,
		const String &vers);

GUIConnection *_appConnection();
GUIConnection *appConnection();

const String &dayOfWeekName(Int d);
const String &monthName(Int m);

String libMsg(const String &msg);
String libMsg(const char *msg);
String appMsg(const String &msg);
String appMsg(const String &msg, const char *dmsg);
String appMsg(const char *msg);
String appMsg(const String &msg, const char *dmsg);

#ifdef __NT__
extern "C" int userId(const char *);
extern "C" char *userName(int);
extern "C" char *fullUserName(int);
extern "C" int groupId(const char *);
extern "C" char *groupName(int);
extern "C" int userGid(int);
extern "C" char *userHomeDir(int);
extern "C" int getuId();
extern "C" char * currentUser(char *, int);
#ifdef HAVE_NETAPI
extern "C" int getFileOwner(const char *fname);
extern "C" bool amIinLocalGroup(char *grp);
#endif
#else
int userId(const char *);
char *userName(int);
char *fullUserName(int);
int groupId(const char *);
char *groupName(int);
int userGid(int);
char *userHomeDir(int);
#ifdef HAVE_NETAPI
int getFileOwner(const char *fname);
bool amIinLocalGroup(char *grp);
#endif

#endif

void background();
void procUpdatePerm(bool v);
bool procUpdatePerm();
void procAddPerm(bool v);
bool procAddPerm();
void procDelPerm(bool v);
bool procDelPerm();

#if HAVE_SAVEMEM
bool procSaveMem();
#endif

void setProcUserId(int uid);
void setProcUserGid(int gid);
int procUserId();
int procUserGid();
Int procPid();
const String &procTtyName();
char **procArgs();
Time procInitTime();
Date procInitDate();
Int procSig();
const String &procTty();
void stop(int val);
void onStop(FP func);
void onAbnormalStop(FP func);
extern "C" void SetDecPointToComma();
extern "C" void SetDecPointToPoint();
extern "C" void SetThousandSepToComma();
extern "C" void SetThousandSepToPoint();
char decPoint();
char thousandSep();
String argVal(Int n);
DateFmt dateFmt();
void dateFmt(DateFmt dfmt);
Int argCnt();
char **argRefVal();

void addConvertFunction(int c, ConvertFunction fp);
void processMask(char *obuff, char *mask, char *ibuff);
Int compileMask(char *mask, char *tstmask, char *omask);
Int sizeOfMask(char *mask);
char *baseName(char const *path);
void setEnableIntHandler(FPI f);
void enableInterrupts(FPI fp);
void initCrypt(char *pw, int sflag);
String encryption(const String &txt, const String &key);
Int swapInfo(Num &swapin, Num &swapout);
int cryptChar(int i);
void cryptString(char *str);
int locate(const char *datum, const char *table, UnInt nel, UnInt size,
		IFPCPCP comp);
const char *toCharPtr(Int v, Int length = 0, Int base = -10);
Int toInt(const char *);
Int memCmpI(const char *, const char *, Int);
const char *strStr(const char *, const char *);
const char *strStrBackw(const char *s1, const char *s2, const char *s3);

void swap(Int &a, Int &b);

#if (__GNUC__ >= 8)
namespace idx 
{
	Int add(Int a, Int b);
	Int div(Int a, Int b);
	Int mult(Int a, Int b);
}
#else
	Int add(Int a, Int b);
	Int div(Int a, Int b);
	Int mult(Int a, Int b);
#endif

#if !defined (_AFXDLL) & !defined(ODBC32)
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
inline Int min(Int a, Int b) { return a < b ? a : b; }
inline Int max(Int a, Int b) { return a > b ? a : b; }
#endif

void chkheap();

#endif

/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gn.h,v 1.31 2010/09/28 19:44:26 hardaiz Exp $
*
* DESCRIPTION
* General definitions.
* Interface & definition header for GN routines.
*
* Files that must be included before this:
* ----------------------------------------
* stdio.h
*********************************************************************/

#ifndef GN_H
#define GN_H

// DALI-STANDALONE: C files that include this header need bool type
#ifndef __cplusplus
#include <stdbool.h>
#endif

#include <ifound/num.h>

#if	HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

#ifdef HAVE_CLOSEPRINTER_CONFLICT
#	define ClosePrinter _ClosePrinter
#	define OpenPrinter	_OpenPrinter
#	define GetMessage	_GetMessage
#endif

#define RndDec(n, nd) 											\
	(n < 0.0	? ceil(n * pow(10.0, (double) nd) - 0.5)		\
						/ pow(10.0, (double) nd)				\
				: floor(n * pow(10.0, (double) nd) + 0.5)		\
						/ pow(10.0, (double) nd))

#define Rnd(n)	(n < 0.0 ? ceil(n - 0.5) : floor(n + 0.5))
#define CUserId(s)				cuserid(s)
#define GetPWEntry(a, b)		getpw((a), (b))

#if MSDOS
#undef SYS_TYPES
#endif

/* Typedefs for the timestamp */

typedef struct {
	long t;
	long d;
	UShort nsec;
	UShort uid;
} time_stamp;

typedef time_stamp TIMESTAMP;

/* Union to point to any object */

typedef union {
	char	*C;
	short	*S;
	long	*L;
	double	*F;
	DATE	*D;
	TIME	*T;
	NUM	*N;
} pointer;

/* permission macros */

#define A_PERM	(1 << 0)
#define D_PERM	(1 << 1)
#define U_PERM	(1 << 2)

#define notA(x)	((x) & A_PERM)
#define notD(x)	((x) & D_PERM)
#define notU(x)	((x) & U_PERM)

/* Internal data types */

typedef UShort itype;
#define	IT_CHAR		0
#define	IT_BYTE		1
#define	IT_SHORT	2
#define	IT_LONG		3
#define	IT_DOUBLE	4
#define	IT_DATE		5
#define	IT_TIME		6
#define IT_FLOATDEC	7
#define IT_NUM		8
#define IT_BINARY	9
#define IT_OBJECT	10
#define IT_DEFAULT	IT_CHAR

#define sizeof_itype(n)(int)((n)==IT_CHAR||(n)==IT_BYTE?sizeof(char)	: \
					(n)==IT_LONG ? sizeof(long)							: \
					(n)==IT_DOUBLE||(n)==IT_FLOATDEC?sizeof(double)		: \
					(n)==IT_BINARY ? (3 * sizeof(long))					: \
					(n)==IT_NUM ? NUM_STORE_LEN							: \
					(n)==IT_OBJECT ? (2 * sizeof(ULong))				: \
					sizeof(short))

#define isnumeric(itype) (itype==IT_SHORT||itype==IT_LONG||\
						  itype==IT_DOUBLE||itype==IT_BYTE||\
						  itype==IT_NUM||itype==IT_FLOATDEC)

/* Max identificators characters */
#define MAX_ID_CHARS    16

/* Definition of miscelaneous constants & macros */

#if defined(unix) || defined(_AIX)
#	define O_BINARY 0
#endif

#ifndef ERROR
#	define ERROR (-1)
#endif

#define NULLFP	(NULL)
#define DEFAULT (-1)

#ifndef __cplusplus
#	define private static
#endif

#ifndef forever
#	define forever for (;;)
#endif

#define IN_SET(a,b)	((int)(a) & (int) (b))

/*
 * Copy memory macro
 * Problems in some 286 machines that receive an int in 'n'.
 */
#if !CPU_I286
#	define MemSet(p,c,n) memset((p),(c),(n))
#endif

/* Some tricks to shut up lint */
#define INT(x) ((int)(x))

#define Alloc(n)(def_Alloc(n,__FILE__,__LINE__))
#define Free(p)(def_Free(p,__FILE__,__LINE__))
#define ReadEnv(n)(def_ReadEnv(n,__FILE__,__LINE__))

#ifndef MODULE_ID
#define MODULE_ID		PKG_RUNTIME
#endif

#ifndef WCMD_FUNCTION
#define WCMD_FUNCTION

#if !defined(HAVE_TTY) && !defined(HAVENT_UI)

/* Define the overlay cache buffer size for dos */

/*			int SetMenuBuiltIn(char *n, FP r) {return 0;} \
*/
#	ifndef OVR_SIZE
#		define OVR_SIZE  0x500
#	endif

#	ifndef NO_SERVICES
#		define DUMMY_FUNCTIONS
#	else
#		define DUMMY_FUNCTIONS	\
			void _w_service() {FreeMemWi();}   \
			void _print_scr() {}               \
			void w_calculator() {}             \
			void calculador(void) {}           \
			void _print_ichset() {}            \
			void servmov() {}                  \
			void wmmov() {}                    \
			void wimov() {}                    \
			void _WiAplHelp(void) {}           \
			void _WiSetAplHelp(void) {}        \
			void _WiKeyConf(void) {}           \
			void GoToShell(void) {}            \
			void _DoSpecialKey(UChar c) {}     \
			UChar AltKey(UChar c) {return c;}     \
			char *hlptxt(char *t, int n) { return t;}    \
			void parse_tfile(void){extern char*TermName;TermName=NULL;}
#	endif

#ifdef PACKED
#	define wcmd(prog, version)					\
	void prog##_x();							\
	void prog(int argc, char *argv[])			\
	{											\
		prog##_x(argc, argv);					\
	}											\
	void prog##_x(int argc, char *argv[])

#	define WindowCmd(system, prog, version)		\
	void UserMain_(int argc, char *argv[]);		\
	void UserMain__(int argc, char *argv[])		\
	{											\
		SetAppName(system, prog, version);		\
		UserMain_(argc, argv);					\
	}											\
	void UserMain_(int argc, char *argv[])

#elif defined(IDEAWIN)
#ifdef __cplusplus
extern "C" {
#endif

    extern int IdeaWinMain(int argc , char **argv);
    extern int WInitApp(void);
    extern short AppType;
 
#if defined (_MFC) || defined (_AFXDLL)
#	include <mswi/winidea.h>
#	undef OpenPrinter
#	undef GetMessage

int WINAPI InitIdeaWin(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow); //Ideafix with MFC Initialization Function.

#endif

#ifdef __cplusplus
}
#endif

/*
	Warning: !!!!
	The 'doform' is the only wcmd program that doesn't use this macro,
	if this macro is updated please update also the main of the doform
	utility.
*/

#  define wcmd(prog, version)							\
void prog(int, char **);								\
int IdeaWinMain(int argc, char ** argv)					\
	{													\
		AppType = 0;									\
		Start(&argc, argv, MODULE_ID);								\
		WiConnect(NULL, argv[0], #version);				\
		prog(argc, argv);								\
		Stop(0);										\
		return 0;                                       \
	}													\
	void prog(int argc, char **argv)

#define Wcmd(prog, version)	    						\
void prog(int, char **);								\
int IdeaWinMain(int argc, char ** argv)					\
	{													\
		AppType = 0;									\
		Start(&argc, argv, MODULE_ID);		 			\
		WiConnect(NULL, argv[0], #version);				\
		prog(argc, argv);								\
		Stop(0);										\
		return 0;                                       \
	}													\
	void prog(int argc, char **argv)

#  define WindowCmd(sys, prog, version)					\
void UserMain_(int, char **);							\
int IdeaWinMain(int argc, char ** argv)					\
	{													\
		AppType = 0;									\
		StartUp(sys, prog, version, &argc, argv);		\
		WiConnect(NULL, argv[0], version);				\
		UserMain_(argc, argv);							\
		Stop(0);										\
		return 0;										\
	}													\
	void UserMain_(int argc, char **argv)

#	define dwcmd(prog, version, apptype)				\
void prog(int, char **);								\
int IdeaWinMain(int argc, char **argv)					\
	{													\
		AppType = apptype;								\
		Start(&argc, argv, MODULE_ID);								\
		WiConnect(NULL, argv[0], #version);				\
		prog(argc, argv);								\
		Stop(0);										\
		return 0;										\
	}													\
	void prog(int argc, char **argv)

#define DWcmd(prog, version, apptype)   				\
void prog(int, char **);								\
int IdeaWinMain(int argc, char **argv)					\
	{													\
		AppType = apptype;								\
		Start(&argc, argv, MODULE_ID);					\
		WiConnect(NULL, argv[0], #version);				\
		prog(argc, argv);								\
		Stop(0);										\
		return 0;										\
	}													\
	void prog(int argc, char **argv)

#	define DWindowCmd(sys, prog, version, apptype)		\
void UserMain_(int, char **);							\
int IdeaWinMain(int argc, char **argv)					\
	{													\
		AppType = apptype;								\
		StartUp(sys, prog, version, &argc, argv);		\
		WiConnect(NULL, argv[0], version);				\
		UserMain_(argc, argv);							\
		Stop(0);										\
		return 0;										\
	}													\
	void UserMain_(int argc, char **argv)

#  define Program(prog, version) 						\
void prog(int, char **);								\
int IdeaWinMain(int argc, char ** argv)					\
	{													\
		AppType = 1;									\
		Start(&argc, argv, MODULE_ID);								\
		WiConnect(NULL, argv[0], #version);				\
		prog(argc, argv);								\
		Stop(0);										\
		return 0;										\
	}													\
	void prog(int argc, char **argv)

#else
#	define wcmd(prog, version)						\
		DUMMY_FUNCTIONS								\
		void (*_far _cdecl _controlc_handler) (void);\
		unsigned _ovrbuffer = OVR_SIZE;				\
		extern void _DosBackWinSetup(void);			\
		void prog(int, char *[]);					\
		int main(int argc, char *argv[])			\
		{											\
			Start(&argc, argv, MODULE_ID);						\
			WiConnect(NULL, argv[0], #version);		\
			_DosBackWinSetup();						\
			_controlc_handler = CallIntHandler;		\
			controlc_open();						\
			prog(argc, argv);						\
			controlc_close();						\
			Stop(0);								\
			return 0;								\
		}											\
		void prog(int argc, char *argv[])

#define Wcmd(prog, version)	   						\
		DUMMY_FUNCTIONS								\
		void (*_far _cdecl _controlc_handler) (void);\
		unsigned _ovrbuffer = OVR_SIZE;				\
		extern void _DosBackWinSetup(void);			\
		void prog(int, char *[]);					\
		int main(int argc, char *argv[])			\
		{											\
			Start(&argc, argv, MODULE_ID);			\
			WiConnect(NULL, argv[0], #version);		\
			_DosBackWinSetup();						\
			_controlc_handler = CallIntHandler;		\
			controlc_open();						\
			prog(argc, argv);						\
			controlc_close();						\
			Stop(0);								\
			return 0;								\
		}											\
		void prog(int argc, char *argv[])

#	define WindowCmd(sys, prog, version)			\
		DUMMY_FUNCTIONS								\
		void (*_far _cdecl _controlc_handler) (void);\
		unsigned _ovrbuffer = OVR_SIZE;				\
		extern void _DosBackWinSetup(void);			\
		void UserMain_(int, char *[]);				\
		int main(int argc, char *argv[])			\
		{											\
			StartUp(sys, prog, version, &argc, argv);\
			WiConnect(NULL, argv[0], version);		\
			_DosBackWinSetup();						\
			_controlc_handler = CallIntHandler;		\
			controlc_open();						\
			UserMain_(argc, argv);					\
			controlc_close();						\
			Stop(0);								\
			return 0;								\
		}											\
		void UserMain_(int argc, char *argv[])


#	define Program(prog, version)					\
		DUMMY_FUNCTIONS								\
		unsigned _ovrbuffer = OVR_SIZE;				\
		void prog(int argc, char *argv[]);			\
		int main(int argc, char *argv[])			\
		{											\
			Start(&argc,argv, MODULE_ID);						\
			prog(argc,argv);						\
			Stop(0);								\
			return 0;								\
		}											\
		void prog(int argc, char *argv[])

#	define SysProgram(sys, prog, version)			\
		DUMMY_FUNCTIONS								\
		unsigned _ovrbuffer = OVR_SIZE;				\
		void UserMain_(int argc, char *argv[]);	\
		int main(int argc, char *argv[])			\
		{											\
			StartUp(sys, prog, version, &argc, argv);\
			UserMain_(argc, argv);					\
			Stop(0);								\
			return 0;								\
		}											\
		void UserMain_(int argc, char *argv[])
#endif
#else

/* Macro to start a program Module							*/
/*
	Warning: !!!!
	The 'doform' is the only wcmd program that doesn't use this macro,
	if this macro is updated please update also the main of the doform
	utility.
*/

	/*	if (String("IxSCA_enabled").getEnv() == String("true")) { */  	
#ifdef IXSCA
#	define wcmd(prog, version)								\
		void prog(int, char *[]);							\
		int main(int argc, char *argv[])					\
		{													\
			Start(&argc, argv, MODULE_ID);					\
			WiConnect((char *) ProcTty(), argv[0],			\
					quote(version));						\
			if (getenv("IxSCA_enabled") != NULL) {			\
				if (strcmp("true", getenv("IxSCA_enabled")) == 0) {			\
		    		if (!crackerClient()) {                                	\
	                    if (!GetSession()) {                                \
    	                    Stop(0);                                        \
        	                return 0;                                       \
            	        }                                                   \
                	}                                                       \
	            }                                                           \
			}																\
			prog(argc, argv);												\
			if (getenv("IxSCA_enabled") != NULL) {							\
				if (strcmp("true", getenv("IxSCA_enabled")) == 0) {			\
		    		if (!crackerClient()) {                                	\
	                    EndProgram();										\
                	}                                                       \
	            }                                                           \
			}																\
			Stop(0);														\
			return 0;														\
		}																	\
		void prog(int argc, char *argv[])

#else

#	define wcmd(prog, version)								\
		void prog(int, char *[]);							\
		int main(int argc, char *argv[])					\
		{													\
			Start(&argc, argv, MODULE_ID);								\
			WiConnect((char *) ProcTty(), argv[0],			\
					quote(version));						\
			prog(argc, argv);								\
			Stop(0);										\
			return 0;										\
		}													\
		void prog(int argc, char *argv[])

#endif /* IXSCA */


#	define WindowCmd(sys, prog, version)					\
		void UserMain_(int, char *[]);						\
		int main(int argc, char *argv[])					\
		{													\
			StartUp(sys, prog, version, &argc, argv);		\
			WiConnect((char *) ProcTty(), argv[0],			\
					version);								\
			UserMain_(argc, argv);							\
			Stop(0);										\
			return 0;										\
		}													\
		void UserMain_(int argc, char *argv[])

/* Macro for a non-window manager client program */
#ifdef IXSCA
#	define Program(prog, version)							\
		void prog(int argc, char *argv[]);					\
		int main(int argc, char *argv[])					\
		{													\
			Start(&argc, argv, MODULE_ID);								\
			if (getenv("IxSCA_enabled") != NULL) {			\
				if (strcmp("true", (const char*)getenv("IxSCA_enabled")) == 0) {			\
		    		if (!crackerClient()) {                                	\
	                    if (!GetSession()) {                                \
    	                    Stop(0);                                        \
        	                return 0;                                       \
            	        }                                                   \
                	}                                                       \
	            }                                                           \
			}																\
			prog(argc, argv);								\
			if (getenv("IxSCA_enabled") != NULL) {							\
				if (strcmp("true", (const char*)getenv("IxSCA_enabled")) == 0) {			\
		    		if (!crackerClient()) {                                	\
	                    EndProgram();										\
                	}                                                       \
	            }                                                           \
			}																\
			Stop(0);										\
			return 0;										\
		}													\
		void prog(int argc, char *argv[])
#else
#	define Program(prog, version)							\
		void prog(int argc, char *argv[]);					\
		int main(int argc, char *argv[])					\
		{													\
			Start(&argc, argv, MODULE_ID);								\
			prog(argc, argv);								\
			Stop(0);										\
			return 0;										\
		}													\
		void prog(int argc, char *argv[])
#endif //IXSCA
#	define SysProgram(sys, prog, version)					\
		void UserMain_(int argc, char *argv[]);				\
		int main(int argc, char *argv[])					\
		{													\
			StartUp(sys, prog, version, &argc, argv);		\
			UserMain_(argc, argv);							\
			Stop(0);										\
			return 0;										\
		}													\
		void UserMain_(int argc, char *argv[])

#endif /* HAVE_TTY */

#endif /* WCMD_FUNCTION */

/* ++ Prototypes ++ */

#ifdef __cplusplus
extern "C" {
#endif

STRUCT ProcessInfo;

#if HAVE_CTRLC
extern void CallIntHandler(void);
#endif

#if HAVE_FREEMEM
extern long FreeMem(void);
extern void FreeMemWi(void);
#endif

extern void OnStop(FP fun);
extern void Stop(int ex_vl);
extern char COMMA(void);
extern char DECPOINT(void);

extern void SetUpdatePerm(bool v);
extern bool GetUpdatePerm(void);
extern void SetAddPerm(bool v);
extern bool GetAddPerm(void);
extern void SetDelPerm(bool v);
extern bool GetDelPerm(void);

#if HAVE_SAVEMEM
extern bool ProcSaveMem(void);
#endif

extern UShort ProcUserId(void);
extern UShort ProcUserGid(void);
extern Int ProcPid(void);
extern const char *ProcTtyName(void);
extern Int ProcNArgs(void);
extern char **ProcArgs(void);
extern TIME ProcInitTime(void);
extern DATE ProcInitDate(void);
extern Int ProcSig(void);
extern const char *ProcTty(void);
extern FPCPIU SetAllocHandler(FPCPIU f);
extern void SetAlloc(bool flag);
extern void *def_Alloc(unsigned int nbytes, char *filename, int lineno);
extern void def_Free(void *p, char *filename, int lineno);
extern void Background(void);
extern void Banner(char const *prog);
extern char *BaseName(char const *path);
extern void SetCheckFactor(long n);
extern int CheckDigit(long n);
extern FPCP SetConvHandler(FPCP f);
#if (!defined(__NT__) && !defined(hppa) && !defined(hpux) && !defined(AIX5) && !defined(Linux7) && !defined(LinuxGcc3) && !defined(LinuxGcc4) && !defined(SunSol)) 
extern IFPICCPVL SetAriMessager(IFPICCPVL handler);
#else
extern IFPICCPCCP SetAriMessager(IFPICCPCCP handler);
#endif
extern int AriError(const char *msg, ...);
extern IFPICCPCCP SetMessager(IFPICCPCCP f);
extern void Error(const char *msg, ...);
extern int Warning(const char *msg, ...);
extern int Notice(const char *msg, ...);
extern FILE *FopenPath(char *name, char *ext, char *mode);
extern void SetEnableIntHandler(FPI f);
extern void EnableInterrupts(FPI fp);

extern Int GetUid();
extern int UserId(char *name);
extern char *UserName(int uid);
extern char *FullUserName(int uid);
extern int GroupId(char *name);
extern char *GroupName(int gid);
extern int UserGid(int uid);

extern Int IxScaGetUid();
extern Int IxScaUserId(char *name);
extern char *IxScaUserName(Int uid);

extern FP SetSigInt(FP fp);
extern int SetIntChar(int c);
extern FILE *OpenPrinter(int ncopies, char *x);
extern void	ClosePrinter(FILE *p);
extern FPCPCPI SetReadEnvHandler(FPCPCPI f);
extern char *def_ReadEnv(char *x, char *filename, int	 lineno);
extern void Start(int *_argc, char **argv, int module);
extern void StartUp(const char *sys, const char *prg, const char *vers,
		int *_argc, char **argv);
extern long TestCheckDigit(char *s);

#ifdef __NT__
	#ifdef __cplusplus
		extern char *cuserid(char *s, int l = 0);
	#else
		extern char *cuserid(char *s, int l);
	#endif
#else
	extern char* cuserid(char *s);
#endif /* __NT__ */

extern void FindTask(void);
extern int KeyReady(void);
extern int SetIntChar(int c);
extern int KeyGetc(void);
extern void ClrKeybBuff(void);
extern void _ConvHandler(char *function, char *reason);
extern char *IdeaVersion(void);
extern int CheckPerm(int level);
extern void InitCrypt(char *pw, int sflag);
extern int CryptChar(int i);
extern void CryptString(char *str);
extern int Locate(char *datum, char *table, UShort nel, UShort sz, FP comp);

// IxSystem functions
extern int ModuleIsInstalled(int modid);
extern int ModuleExists(int modid, char *wsch);
extern bool ModuleOk(int modid);

extern bool CrackerClient(void);
extern const char *GetMessage(const char *module, const char *section, const char *msg);
extern void MessageDisplay(const char *module, const char *section, const char *label, const char *msg, ...);
extern void MessageDisplayError(const char *module, const char *section, const char *msg, ...);
extern void MessageFatalError(const char *module, const char *section, const char *msg, ...);


/* Image handling functions */

#ifdef __cplusplus
extern int ShowImage(char *,char *,int x=-1, int y=-1);
#else
extern int ShowImage(char *,char *,int x, int y);
#endif

extern void CloseImage(int);
extern void CloseAllImages(void);

/* Conversion functions */

#define StoL	ItoL
#define StoF	ItoF
extern long ItoL(short i);
extern double ItoF(short i);
extern double LtoF(long i);
extern short LtoI(long i);
extern short FtoI(double i);
extern long FtoL(double i);
extern double BtoF(char c);
extern long BtoL(char c);
extern short BtoI(char c);
extern signed char ItoB(short i);
extern signed char LtoB(long i);
extern signed char FtoB(double i);


/* Cracker specific functions */
#ifdef __cplusplus
extern int CrackerWiExecCmd(char *title, char *cmd,int wait=0);
#else
extern int CrackerWiExecCmd(char *title, char *cmd,int wait);
#endif
extern int CrackerExecShell(char *cmd);
extern int CrackerSendFile(char *localfile, char *remotefile);
extern int CrackerReceiveFile(char *localfile, char *remotefile);
extern int CrackerGetClipboardData(char *buf, int maxlen);

#ifdef IXSCA
extern bool crackerClient();
extern bool GetSession();
extern void EndProgram();
#endif

/* ANSI Functions */
// DALI-STANDALONE: On Linux, these are already declared by system headers.
// Re-declaring them causes exception specifier mismatches with glibc.
#ifndef linux
extern FILE *fdopen(int, FDOPENPARAM2TYPE);
extern DLLIMPORT FILE *popen(POPENPARAMTYPE, POPENPARAMTYPE);
extern int pclose(FILE *);
extern FILE *fopen(char const *, char const *);
extern int fprintf(FILE *, const char *, ...);
extern int fclose(FILE *);
extern int fflush(FILE *);
extern FTELLRETTYPE ftell(FTELLPARAM1TYPE);
extern int fseek(FILE *, long, int);
extern SizeT fread(FREADPARAM1TYPE, SizeT, SizeT, FILE *);
extern SizeT fwrite(FWRITEPARAM1TYPE, SizeT, SizeT, FILE *);
extern char *getpass(const char *prompt);
extern int execl(char const *path, char const *arg1, ...);
extern int rmdir(char const *p);
extern int access(char const *f, int a);
extern DLLIMPORT int rand(void);
extern DLLIMPORT SRANDRETTYPE srand(unsigned);
extern int fputc(FPUTCPARAM1TYPE, FILE *);
extern int fputs(char const *, FILE *);
extern char *fgets(char *, int , FILE *);
extern int fgetc(FILE *);
#endif /* linux */

#ifdef __cplusplus
}
#endif

/* -- Prototypes -- */

#if	HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif

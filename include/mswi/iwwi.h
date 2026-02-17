#ifndef _INC_IWWI
#define _INC_IWWI

#ifndef IDEAFIX
#   define IDEAFIX	1
#endif /* IDEAFIX */

#ifndef IDEAWIN
#   define IDEAWIN	1
#endif /* IDEAWIN */

#include <ixwi/wi.h>
#include <cfix/types.h>

#define putchar(c) fprintf(stdout,"%c",c)
#define fputs(s,f) fprintf(f,"%s",s)
#define fputc	 	__fputc
#define fgetc		__fgetc
#define getchar()	__fgetc(stdin)
#define getc     	__fgetc

#ifdef __NT__
#  ifndef CALLBACK
#    define CALLBACK    __stdcall
#  endif
#  ifndef WINAPI
#   define WINAPI      __stdcall
#  endif
#  ifndef PASCAL
#   define PASCAL      __stdcall
#  endif
#endif

#define 	MSG_YES_NO				0x0040
#define 	MSG_AB_RE_IG			0x0080
#define 	MSG_OK					MSG_WAIT
#define     MSG_OK_CANCEL   		0x0100
#define     MSG_YES_NO_CANCEL       0x0200
#define 	MSG_RETRY_CANCEL 		0x0400
#define 	MSG_CUSTOM 				0x8000

/* ----------- Window classes -------------------- */

#define 	ERROR_CLASS				(-1)	// Activ MDI Window not present;
#define     DEF_WND_CLASS           0
#define     FORM_WND_CLASS          1
#define     SCROLL_WND_CLASS        2
#define     CHART_WND_CLASS         3
#define     REPORT_WND_CLASS        4
#define     EDIT_WND_CLASS          5
#define     OUTPUT_WND_CLASS        6


#define     B_DEF_WND_CLASS      (((attr_type)DEF_WND_CLASS)    << 28)
#define     B_FORM_WND_CLASS     (((attr_type)FORM_WND_CLASS)   << 28)
#define     B_SCROLL_WND_CLASS   (((attr_type)SCROLL_WND_CLASS) << 28)
#define     B_CHART_WND_CLASS    (((attr_type)CHART_WND_CLASS)  << 28)
#define     B_REPORT_WND_CLASS   (((attr_type)REPORT_WND_CLASS) << 28)
#define     B_EDIT_WND_CLASS     (((attr_type)EDIT_WND_CLASS)   << 28)
#define     B_OUTPUT_WND_CLASS   (((attr_type)OUTPUT_WND_CLASS) << 28)

/* -------- Type of application ------------------ */
#define APP_DOFORM          0
#define APP_GENERAL         1
#define APP_MENU            2
#define APP_IQL             3
#define APP_SERVER          4
#define APP_CLIENT          5
#define APP_EXECFORM	    6

/* ----------- Frames -------------------- */

#define     B_NO_FRAME              0x00000000
#define     B_SINGLE_FRAME          0x01000000
#define     B_DOUBLE_FRAME          0x02000000
#define     B_THICK_FRAME           0x04000000     /* for sizeble windows */
#define     B_CAPTION               0x08000000
#define     B_FULL_SCREEN           0x0F000000

/* ---------- Color Attribute ------------ */
#define 	LT_COLOR			0x01000000
#define		LT_BKCOLOR			0x10000000

/* ---------- Key Codes for Map file ----- */

#define     KEY_VIRTUAL    (unsigned short)     0x000100
#define     KEY_ALT        (unsigned short)     0x000400
#define     KEY_CONTROL    (unsigned short)     0x000800
#define     KEY_SHIFT      (unsigned short)     0x001000

/* --------- Interface Type -------------- */
enum {
	MDI,
	SDI,
	IDI,
};

/* -------- Function prototypes ------------------ */
#ifdef __cplusplus
extern "C" {
#endif
extern char    ModuleName[9];
extern short   AppType;
extern bool    bClient;
extern char    ApplicName[16];

extern int PASCAL StartMessageLoop(void);
extern void PASCAL StopMessageLoop(unsigned char);
extern bool __WiCreateField( type ftype, short olength, void * );
	/* create filed in current window at current cursor position */
extern void __WiDeleteField( void );
	/* delete filed in current window at current cursor position */

extern void  SetProcessWait(void);
extern void  ResetProcessWait(void);

extern void _CloseErrorWindow(void);
extern void _ShowErrorWindow(void);
extern int RunArg(int *, char **, char *);
extern int  __fputc(FPUTCPARAM1TYPE, FILE *);
extern int  __fgetc(FILE *);
extern int InterfaceType(void);

#ifdef __cplusplus
  }   /* extern "C" { */
#endif

#ifdef __cplusplus
	class TerminalOutput;
	extern void  _IRScroll(UChar);
	extern void  _IRSetOutput(TerminalOutput *);
	extern void  _IRFixRange();
	extern bool  _IRChngSizeX(void);
	extern bool  _IRChngSizeY(void);
	extern void  _IRNoRedraw(UChar);
	extern void  _IRMessage(char*);
	extern int   _IRGetMaxYPage(void);
	extern int   _IRGetMaxXPage(void);
	extern int   _IRGetNewYPage(void);
	extern void  _IRClear(void);
#endif

/* ---End of Function prototypes------------------ */

#endif  /* _INC_IWWI */


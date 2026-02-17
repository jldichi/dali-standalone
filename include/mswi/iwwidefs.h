#ifndef _INC_IWWIDEFS
#define _INC_IWWIDEFS

#include <mswi/iwwi.h>

#include <mswi/winidea.h>

#include <mswi/iwwin32s.h>
#include <mswi/iwwm.h>
#include <mswi/iwres.h>		/* Resource ID */
#include <mswi/ideawin.h>

/* functions from C run-time libs are changing to Windows functions */

#define		vsprintf		wvsprintf

/* ---------- General defenitions ------------------------- */

#define	WM_IDEAWINMSG		(WM_USER + 0x0400)

#define WM_FUNCTION         WM_IDEAWINMSG+1
/* 	Commands from menu, toolbar, hotkeys sended by AppWnd
	Commamd = (WORD) wParam
	lParam	= 0L								*/

#define WM_STOPMSGLOOP		WM_IDEAWINMSG+2
/* 	Stop proccessing. Not Used now.
	wParam 	= 0
	lParam	= 0L								*/

#define WM_BAR				WM_IDEAWINMSG+3
/* 	Messages from ToolBar.
	action 	= (Int) lParam
	Command	= (WODR) wParam						*/

#define WM_HOTKEY			WM_IDEAWINMSG+4
/* HotKey translated messages sensded from Message processing
   routine (Start Message loop)
   Command	= (WORD) wParam
   lParam	= 0L								*/

#define WM_ADDSTRING_NOT_MY		WM_IDEAWINMSG+5
// not my model

#define WM_ADDSTRING			WM_IDEAWINMSG+6
/* Add string to output window
   winID	= (int) wParam  	for stderr = STDERR
   lpstr	= (LPSTR) lParam  	string to add	*/

#define WM_REGISTERCLIENT	WM_IDEAWINMSG+7
/* Register client application
	iClientInst	= (HINSTANCE) wParam
	lpstrClient = (LPSTR) lParam // Client module name
	return		- (HWND) of output window		*/

#define WM_CHECKCOMMAND		WM_IDEAWINMSG+8
/* Check menu items
	idMenu		= (short) wParam;
	return	 	- (UINT) menu checing type;				*/

#define WM_TASKEND             WM_IDEAWINMSG+9
/*  Child task terminated                               */

#define WM_MDIREFRESHTITLES             WM_IDEAWINMSG+10
/* Sended to MDI Client window to refresh names of child windows
	hWnd	= (HWND) wParam			- name of Window added or delited
	bApp	= (BOOL) lParam 		- TRUE, if hWnd is added */

#define WM_REGISTERCLIENT_NOT_MY 	WM_IDEAWINMSG+11
// not my model

#define WM_SETMYACTIVWND			WM_IDEAWINMSG+12

#define WM_IWAPPACTIVATE 			WM_IDEAWINMSG+13
/*  Sended to MDI Client window when App is activated/disactivated
	bActiv	= (BOOL) wParam						*/

#define     GCW_CLASSID         0
#define     CBCLASS_EXTRA	2

#define STDIN	0
#define STDOUT	1
#define STDERR  2

/* ---------- Color Attribute ------------ */
#define 	LT_COLOR			0x01000000
#define		LT_BKCOLOR			0x10000000

/* ---------- Colors --------------------------- */
#define COLOR_WHITE		GetSysColor(COLOR_BTNHIGHLIGHT)
#define COLOR_LTGRAY        GetSysColor(COLOR_BTNFACE)
#define COLOR_GRAY 			GetSysColor(COLOR_BTNSHADOW)
#define COLOR_BLACK			GetSysColor(COLOR_BTNTEXT)
#define CLR_RED             PALETTERGB(128,0,0)
#define CLR_BLUE			PALETTERGB(0,0,128)

/* ------------ Fonts -------------------------- */
#define	NORMAL		0x0000
#define BOLD		0x0001
#define	ITALIC		0x0002
#define UNDERLINE	0x0004

#define MAX_APPFONTS    26
#define ADEFBI_FONT     BOLD|ITALIC
#define ADEFU_FONT		UNDERLINE
#define ADEFUI_FONT		ITALIC | UNDERLINE
#define ADEFUB_FONT		BOLD|UNDERLINE
#define ADEFUIB_FONT	BOLD|ITALIC|UNDERLINE

#define VARIABLE_PITCH_FONT	0
#define FIXED_PITCH_FONT	8

#define DEF_FONT			16
#define ADEF_FONT           DEF_FONT
#define ADEFB_FONT      	17
#define ADEFI_FONT      	18
#define AFIXSR_FONT			19
#define ASB_FONT			20

#define ASYS_FONT       	21
#define AFIXS_FONT			22
#define AFIX_FONT       	23
#define MENU_FONT		24
#define PRINTER_FONT		25

/* Toolbar part ------------------------------------ */

#define		NOBAR	  (-1)
#define		TOPBAR		0
#define		LEFTBAR		1
#define		POPUPBAR	2

#define		BT_DOWN		0L
#define		BT_UP		1L
#define		BT_ACTION	2L

/* ----------- Keys ------------------------------ */

#define	K_CUT		(WORD)	0x0101
#define K_COPY		(WORD)	0x0102
#define K_PASTE		(WORD)	0x0103
#define K_DELETE	(WORD)	0x0104
#define K_TOGGLE_INS (WORD)  0x0105

#define K_WORD_LEFT  (WORD)	0x0106
#define K_WORD_RIGHT (WORD)	0x0107
#define K_LEFT_END	 (WORD)	0x0108
#define K_RIGHT_END  (WORD)	0x0109

#define K_S_CURS_LEFT	(WORD)	0x010a
#define K_S_CURS_RIGHT	(WORD)	0x010b
#define K_S_WORD_LEFT	(WORD)	0x010c
#define K_S_WORD_RIGHT	(WORD)	0x010d
#define K_S_LEFT_END	(WORD)	0x010e
#define K_S_RIGHT_END	(WORD)	0x010f

#define MF_ERROR		(UINT) 0xFFFF

typedef struct id {
	unsigned short	id_menu;
	unsigned short 	k_char;
} id;

/* --------------- Error messages --------------------------- */
/*
IDS_NO_MEM_TO_SAVE
IDS_CAN_NOT_SAVE
IDS_MAP_FILE_ERR
IDS_OPT_NOT_AVAIL
IDS_CANT_CREATE_WINDOW
IDS_CANT_REGISTER_CLASS
IDS_NOT_PRESENT_STD
IDS_NOT_OPEN_STD
IDS_NOT_STDIN_FILE
IDS_CONTINUE
IDS_CAN_NOT_READ
//IDS_CANT_CREATE_FLDWI
IDS_CANT_OPEN_STDIO_BUFF
IDS_CANT_FIND_TB_RES
IDS_EXENOTFOUND
IDS_WINEXEC
IDS_NO_MEMORY
IDS_WINDESC
IDS_NOT_VALIDPARENT
IDS_BADFORMAT
IDS_FIND_WINCLASS
IDS_CANT_FOUND_IDEADLL
IDS_CANT_RUN_SECOND_SERVER
IDS_NO_GLOBAL_MEM
*/
/* --------------- Arguments processing --------------------- */

#define MAXARGV             15
extern char *  argv[MAXARGV];
extern int     argc;
extern FILE*  std_file[2];

extern BOOL WINAPI ChechArguments(HINSTANCE hInst,char* wargv);
extern void WINAPI CloseStd(void);

/* -------- Externan variables ------------------- */

extern 	HWND 		hwndMDIClient;
extern 	HINSTANCE	hLangLib;
extern 	HWND    	__hAppWnd;
extern 	HINSTANCE  	__hAppInstance;
extern  HTASK       __hAppTask;
extern 	HINSTANCE  	hIdeaWinDLL;
extern 	HINSTANCE 	hCtl3d;
extern 	BOOL		InsMode;

#define LoadLangString(idResource, szBuffer) 		\
		LoadString(hLangLib, idResource, (char *)szBuffer, sizeof(szBuffer))
#define _LoadLangString(idResource, szBuffer, cbBuffer) 		\
		LoadString(hLangLib, idResource, (char *)szBuffer, cbBuffer)
#define LoadGenString(idResource, szBuffer) 		\
		LoadString(hIdeaWinDLL, idResource, (char *)szBuffer, sizeof(szBuffer))
#define _LoadGenString(idResource, szBuffer, cbBuffer) 		\
		LoadString(hIdeaWinDLL, idResource, (char *)szBuffer, cbBuffer)
#define GetAppInstance() 		((const HINSTANCE) __hAppInstance)
#define GetAppWnd() 			((const HWND) __hAppWnd)
#define GetMDIClientWnd()       ((const HWND) hwndMDIClient)
#define IniFName				IwGenMsg(IDS_INIFNAME)

#define IsMDI()					(InterfaceType() == MDI)
#define IsSDI()					(InterfaceType() == SDI)
#define IsIDI()					(InterfaceType() == IDI)

/* -------- Function prototypes ------------------ */
extern UINT WINAPI CheckByMenu(UINT MenuId);
extern void WINAPI ShowMenuStr(UINT MenuId);
extern BOOL WINAPI __FreeStd(int);
extern void WINAPI __TermChildTask(void);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef private
#	undef private
#endif

extern char * IwErrMsg( int Msg );
extern char * IwGenMsg( int Msg );
extern int PASCAL StartMessageLoop(void);
extern int PASCAL SingleMessageLoop(void);
extern void PASCAL StopMessageLoop(unsigned char);
extern int IdeaWinMain(int argc , char ** argv);
extern BOOL PASCAL _IsFormEdit();
extern void PASCAL _SetFormEdit( BOOL flag );
extern HWND PASCAL _CurrentDialog();
extern void PASCAL _SetCurrentDialog( HWND hWnd );
extern int 	_SearchRegexp(int, int, int, int, char *, CPFPCPI);
extern int _SearchRegexpWnd( HWND hWnd, BOOL Next );
extern const char * _GetSearchString( void );
extern void _SetSearchString( const char * );
extern void _CancelSearch( void );
extern void __WiDispFile(char*,int,int,int,int,char*,bool);

extern BOOL IsCtrlField();
extern BOOL IsKeyField();
extern BOOL IsMultiLineField();
extern BOOL FieldHasSelection();
extern BOOL FieldCanUndo();

/* from winfputc ---------------------- */

extern HWND _GetErrorWindow(void);

/* from C run-time lib ---------------- */

extern int dup2(int, int);
extern char * strdup(const char *);

/* --------- WinSock ------------------ */
extern int LoadWinsock(void);
extern void FreeWinsock(void);

void setWaitHook(void);
void setNormalHook(void);
/* ------------ Form functions -----------*/
extern int FmGetField( type ftype, UChar * data, UShort opt,
    short length, short olength, short ndec, char * tstmask, char * omask );
extern void FmCondCreateField(short ftype, short olength);

#ifdef __cplusplus
  }   /* extern "C" */
#endif

extern bool setSkipView(bool skip, int row, int column);

/* ---End of Function prototypes------------------ */


#endif  /* _INC_IWWIDEFS */


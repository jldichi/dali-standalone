#ifndef _INC_IDEAWIN
#define _INC_IDEAWIN

typedef int  (CALLBACK  * LMF) (char*, char***, int, int);
typedef struct s_app_data {
	HTASK			hTask;
	HINSTANCE		hAppInst;
	HWND			hAppWnd;
	HWND			hMDIClWnd;
	short			nAppType;
    HWND			_hServWnd;
    bool			bClient;
    char	*		pModuleName;
	short			DbmsType;
} s_app_data;

#ifdef __cplusplus
extern "C" {
#endif
/* ============ iwapp.cpp ========================================== */
extern HINSTANCE WINAPI  GetIdeaWinDLL(void);
extern BOOL WINAPI  _RegisterApp(s_app_data *);
extern void WINAPI  EndApp(void);
extern HINSTANCE WINAPI  LoadLanguageDLL(void);
extern void WINAPI  CheckClientSize(WORD, WORD);
extern HINSTANCE WINAPI  LoadCtl3d(HINSTANCE);
extern BOOL   ExecRegisterParent(HWND, char *);
extern void   ExecUnRegisterParent(HWND);
extern BOOL   ExecRegisterChild(HTASK, char *);
extern void  initialize(void);
extern void _GetClientRect(HWND, LPRECT);
extern void SetInterfaceType(BOOL);
extern HICON WINAPI  LoadIconsDLL(char *);
extern void SetApplicName(char *);

extern void WINAPI GetMDIMinMaxInfo( LPMINMAXINFO );
extern int dprintf( const char * Fmt, ... );
extern bool isWin3();

/* ============ Logo.cpp ========================================== */

extern void WINAPI  ShowLogo(HWND);
extern void WINAPI  ShowAbout(HWND);


/* ------------ Integr.cpp ---------------------------------------- */

extern int WINAPI  RunUtility(HWND, int);
extern int WINAPI  RunUtilityOptionDlg(HWND, int);
extern BOOL WINAPI  IsLogo(void);
extern int WINAPI  RunPreferencesDlg(HWND);

/* ------------ iwdirarg.cpp ---------------------------------------- */
extern void WINAPI  SetWorkDir(void);
extern int WINAPI  RunSetWorkDirDlg(HWND);
extern void WINAPI  RunDDListDlg(HWND);
extern int WINAPI  _RunArg(char *);

/* ============ Iwfonts.cpp ======================================= */

extern HFONT WINAPI  GetAppFont(int);
extern void WINAPI SelectFont(int);
extern HDC WINAPI createPrinterDC( void );

/* ========= iwbars.cpp ==================================== */

extern void WINAPI  StatusMsg(LPSTR);
extern int WINAPI  InitStatusBar(HWND, LPSTR);
extern void WINAPI  ChangeStatusStates(void);
extern BOOL WINAPI  ToggleInsertState(BOOL);

/* ========= iwmisc.cpp ==================================== */

extern COLORREF WINAPI  crColor(attr_type);
extern COLORREF WINAPI  crBkColor(attr_type);
extern COLORREF WINAPI  crBkFldColor(attr_type);
extern int WINAPI  _DelEscSeqTo(char * dst, const char * str);
extern void WINAPI  _DelSquareBrackets(char * dst, const char * str);
extern int WINAPI  GetSystemTextExtent( LPSTR str, int len );
extern int WINAPI  GetSystemTextHeight( LPSTR str, int len );
extern void WINAPI  _InternToExtern(char *, const char * );
extern CH_SET * WINAPI  _GetChSet(void);
extern attr_type * WINAPI  _GetOutTab(void);
extern ATTR_TAB	* WINAPI  _GetAttrTab(void);
extern KEY_TAB	* WINAPI  _GetKeyTab(void);
extern HWND WINAPI GetPopupParent( HWND );
extern HCURSOR WINAPI SetAppCursor( HCURSOR );
extern HCURSOR WINAPI SetAppCursorSys( LPCSTR );
extern HCURSOR WINAPI GetAppCursor( void );
extern BOOL WINAPI ShowAppCursor( void );

/* ========= Toolbar.cpp ==================================== */

extern int WINAPI  InitToolBar(HWND, int *, LPSTR);
extern void WINAPI  HideToolButton(int, BOOL);
extern int WINAPI  DesktopDlg(HWND, int *);

/* ========= iwmsg.cpp ======================================= */
extern int  WiExMsg(int, char *, char *, ...);
extern int  WiVExMsg(int, char *, char *, va_list);

extern void WINAPI DisableTaskWindows(HWND hBase);
extern void WINAPI EnableTaskWindows(HWND hBase);

/*=========== winfputc ========================================= */
extern HWND WINAPI CreateOutWindow(HINSTANCE, LPCSTR);
extern BOOL WINAPI ExistOutputWindow( void );
extern void WINAPI CloseOutputWindows( void );

/*=========== mdiprocs ========================================= */

extern void WINAPI  CheckWindowMenu(HWND, HMENU);
extern LRESULT CALLBACK _MDIClientProc (HWND, UINT, WPARAM, LPARAM);

/*=========== idiprocs ========================================= */
extern HWND WINAPI FindIDIChildWindow( WORD Class );
extern LRESULT CALLBACK _IDIClientProc (HWND, UINT, WPARAM, LPARAM);
extern LRESULT CALLBACK DefAppChildProc(HWND, UINT, WPARAM, LPARAM);
extern BOOL CALLBACK DefAppDlgProc(HWND, UINT, WPARAM, LPARAM);
extern UINT CALLBACK DefAppDlgHook(HWND, UINT, WPARAM, LPARAM);

/* -------------------------------------------------------------- */
#ifdef __cplusplus
  }   /* extern "C" */
#endif

#endif /* _INC_IDEAWIN */



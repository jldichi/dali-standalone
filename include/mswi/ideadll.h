#ifndef _INC_IDEADLL
#define _INC_IDEADLL
#ifndef IDEADLL
#   define IDEADLL      1
#endif
#include <mswi/iwwidefs.h>

#define SYSNAMELEN	20
typedef struct s_bar {
        RECT            rc;
    HBITMAP     hBMP;
    short                       state;
    short                       iBarId;
        short           iShiftBarId;
        short           iCtrlBarId;
        } s_bar;

struct sDLL;

typedef struct sToolBar {
        s_bar   *aBar;
        struct sDLL     *sDLLData;
        int     iBUTTONHEIGHT;
        int     iBUTTONWIDTH;
        int     __nToolSize;
        short   nButton;
        int     BarType;
        short   iActivBut;
        short   iCurrBut;
        short   iCmdBut;
        char    ResName[20];
        } sToolBar;

struct WFILE;
struct TSearch;

typedef struct sDLL {
    HTASK                   hTask;
    struct sDLL     		*pNext, *pPrev;
    HINSTANCE               hAppInst;
    HWND                    hAppWnd;
    HWND                    hMDIClWnd;
    short                   nAppType;
    HWND                    hStatusWnd;
    HWND            		hToolBarWnd;
    sToolBar 				* pTool;
    HWND                    _hServWnd;
    bool                    bClient;
    char                    ModuleName[9];
    Int                    ponderator;
    struct WFILE    		* wstdouta;
    struct WFILE    		* wstderra;
    void                    * pdlg;
    IFPICPVL                error_msg;
    short                   DbmsType;
    HWND                    hActiveIDIChild;
    short                   interfaceType;
    HWND 					* IDIChildList;
    short                   IDIChildCount, IDIChildListSize;
    HICON                   hAppIcon;
    char                    * ApplicName;
    HINSTANCE               hIconLib;
	HCURSOR 				hAppCursor;
	struct TSearch					*CurrentSearch;
	char					sysName[SYSNAMELEN+1];
	char dec_point;
	char thousand_sep;
	void * searchTxt;
} sDLL;

#define WFM_SAVE 0x1001
enum WFILE_Type
  {
        STD = 0, // wstdouta or wstderra in MY MDI
        SERVER,  // wstdouta or wstderra connected to Server (do not close it)
        CLIENT,  // Client output window in Server
        FREE     // Disconnected client
  };
typedef struct WFILE {
        HWND hWnd;
        HWND hWndEdit;
        sDLL * pDLL;
        BOOL Registered;
        WORD Class_ID, Title_ID, SaveName_ID, Icon_ID;
        int buffsize;
        int buffpos;
        char * buff;
        char windname[48];
        enum WFILE_Type Type;
        HINSTANCE hClientInst;
        struct WFILE * next;
        struct WFILE * previos;
  } WFILE;


extern struct WFILE _wstdouta;
extern struct WFILE _wstderra;

extern int                              __nStatusHeight;
extern WNDPROC                  PrevMDIClientProc;
extern HBITMAP          hScreenBMP;
extern BOOL                     initialized;


#define Stop(ex_cod)    StopApp(ex_cod)
#define stop(ex_cod)    StopApp(ex_cod)

#ifdef GetAppInstance()
#       undef GetAppInstance()
#endif
#define GetAppInstance()        ((const HINSTANCE ) hDLLInst)

#ifdef GetAppWnd()
#       undef GetAppWnd()
#endif
#ifdef Alloc
#       undef Alloc
#endif
#define Alloc tr_malloc
#ifdef Free
#       undef Free
#endif
#define Free tr_free

#define LoadLangString(idResource, szBuffer)        \
                LoadString(hDLLLangLib, idResource, (char *)szBuffer, sizeof(szBuffer))
#define _LoadLangString(idResource, szBuffer, cbBuffer)                 \
                LoadString(hDLLLangLib, idResource, (char *)szBuffer, cbBuffer)

#define LoadGenString(idResource, szBuffer)         \
        LoadString(GetAppInstance(), idResource, (char *)szBuffer, sizeof(szBuffer))
#define _LoadGenString(idResource, szBuffer, cbBuffer)          \
        LoadString(GetAppInstance(), idResource, (char *)szBuffer, cbBuffer)

#ifdef __cplusplus
extern "C" {
#endif

extern CH_SET chset[];
extern  KEY_TAB  key_tab[];
extern  ATTR_TAB attr_tab[];
extern attr_type out_tab[];

extern HINSTANCE                hDLLInst;
extern HINSTANCE                hDLLLangLib;
extern BOOL                                     CallFromDLL;

extern int WINAPI StartMessageLoop(void);
extern void WINAPI StopMessageLoop(unsigned char);
extern sDLL * WINAPI GetDLLData(void);
extern sDLL * WINAPI _GetDLLData(HTASK);
extern sDLL * WINAPI _GetServerDLLData(void);
extern HWND WINAPI GetAppWnd(void);
extern void WINAPI StopApp(int);
extern void * _dalloc(unsigned int, char*, unsigned );
extern void * _dglalloc(unsigned int, char*, unsigned );
extern void _dglfree(void *, char*, unsigned );
extern void * glalloc(unsigned int);
extern BOOL WINAPI BrowseFile(HWND, LPSTR, int);
#define dalloc(n)(_dalloc(n,__FILE__,__LINE__))
#define dglalloc(n)(_dglalloc(n,__FILE__,__LINE__))
#define dglfree(n)(_dglfree(n,__FILE__,__LINE__))

void  _memd(char *, char *, unsigned);
#define memd(sz)(_memd(sz,__FILE__,__LINE__))

#ifdef __cplusplus
  }   /* extern "C" */
#endif

extern void WINAPI __DeleteAppFonts(void);
extern void WINAPI __MDIClientSize(void);
extern void WINAPI MoveStatusBar(int, int);
extern void WINAPI MoveToolBar(int, int);
extern HBITMAP WINAPI   CreateScreenBitmap(void);
extern void WINAPI      InitAppFonts(void);
extern void WINAPI InitAllUtils(void);
extern void WINAPI SaveOptionsIniFile(short);
extern BOOL WINAPI IsOptionSavedIniFile(void);
extern void WINAPI CloseOutWindows( sDLL * );
extern WFILE * WINAPI CreateStdWFILE( int Type, sDLL * );
extern void WINAPI DeleteStdWFILE( WFILE * );
extern void WINAPI FlushAllWFILEs( void );
extern void WINAPI ToolBarSize(LPSIZE);
extern void WINAPI StatusBarSize(LPSIZE);
void WINAPI SetSDIAppWndSize( LPRECT prc );
void WINAPI GetSDIAppWndSizePos( LPRECT prc, LPSIZE size, LPPOINT pos );
HWND WINAPI GetSDIForm( void );

#endif /*_INC_IDEADLL */


#ifndef _INC_IWSEARCH
#define _INC_IWSEARCH

#ifndef __cplusplus
#error C++ mode required
#endif

#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTERROPS
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NONLS
#define NOMB
#define NOMETAFILE
#define NOOPENFILE
#define NOSERVICE
#define NOSOUND
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEREFWINDOWPOS

class TSearch
  {
  public:
    TSearch( int org_row, int org_col, int ncols, int line,
		char *garg, CPFPCPI get_optx);

    ~TSearch();

    int Search();

	static const char * GetSearchString( void ) { return Str; };
	static void SetSearchString( const char * Str );

	BOOL Cancelled;

  private:
    int nCols, Line;
    BOOL Next;
    CPFPCPI GetOptx;
    char *gArg;
    HWND Parent;
    HWND hWnd;
	BOOL inSearch;

    static BOOL CALLBACK DialogProc( HWND, UINT, WPARAM, LPARAM );

	static char * Str;
	static int MaxCol;
	static BOOL Up;
	static BOOL Case;

    void PASCAL OnClose();

    BOOL PASCAL OnInitDialog( HWND hWndDialog, HWND hWndFocus, LPARAM data );
    BOOL PASCAL OnCommand( HWND hWndCtl, int CtlID, UINT Msg );

    void PASCAL OnSearch();
    void PASCAL OnCancel();
    void PASCAL OnHelp();

    int PASCAL DoSearch();
	void PASCAL EnableWindows( BOOL Enable );
  };

#endif


#ifndef _INC_IWPOPUP
#define _INC_IWPOPUP

#include <mswi/iwwin.h>

#ifndef __cplusplus
#error C++ mode required
#endif

class TControl
  {
  public:
    POINT p;
    SIZE s;
    HWND hWnd;
    WORD ID;

    void Create( char * Class, char * Name, HWND Parent, DWORD Style );
    ~TControl() {}
    operator HWND() { return hWnd; }
    operator WPARAM() { return WPARAM(hWnd); }
  };

class TButton : public TControl
  {
  public:
    char * Name;
    void Init( HDC hDC, WORD IDS, WORD IDM );
    void Create( HWND Parent, DWORD Style );
    ~TButton() { tr_delete_vec(Name); }
  };

class TPopUpMenu
  {
  public:
    TPopUpMenu( int nrows, int ncols, LPSTR Title,
                CPFPCPI get_optx, char *garg,
                FPCPPI execf, char *earg[],
                int attrib );

    ~TPopUpMenu();

    int Select();

  private:
    char * Title;
    int nRows, nCols;
    CPFPCPI GetOptx;
    char *gArg;
    FPCPPI ExecF;
    char **eArg;
    int Attribs;
    HWND Parent;
    HFONT hItemsFont, hButtonFont;
    int FontHeight, FontWidth;

#ifdef __NT__
    LPDLGTEMPLATE Template;
#else
    HGLOBAL Template;
#endif

    HWND hWnd;

    enum
      {
        ID_OK       = IDOK,
        ID_CANCEL   = IDCANCEL,
        ID_LISTBOX  = 101,
        ID_SEARCH,
        ID_NEXT
      };

    TControl ListBox;
    TButton Ok, Cancel, Search, Next;

    int NumButtons;

    int NumItems;
    BOOL EndOfItems;
	BOOL inSearch;

	const char **SaveRows;
	int	SaveFirstRow;
	static const char NoString[];

	int 	BufferSize() { return (nRows<<2)+2; };
	int 	ReadBefore() { return (nRows>>1)+1; };
	int 	ReadAhead() { return nRows+(nRows>>1)+1; };
	int		MaxInvisibleLines() { return nRows*5; }
	void 	_FreeSaveString( int row );
	void 	_MoveSaveString( int dstrow, int srcrow );
	const char *GetString( int index );  // with moving buffer window
	const char *_GetString( int index ); // without moving buffer window
	const char *_GetOptx( int index );	// call GetOptx and
										// validate and strdup str if Ok

    static BOOL CALLBACK DialogProc( HWND, UINT, WPARAM, LPARAM );

    void OnClose();

	void CreateControl( char * Class, char * Name,
			TControl &TPopUpMenu::ctl, DWORD Style );
	void CreateButton( TButton &TPopUpMenu::btn, DWORD Style );

    BOOL OnInitDialog( HWND hWndDialog, HWND hWndFocus, LPARAM data );
    BOOL OnMeasureItem( int CtlID, LPMEASUREITEMSTRUCT mi );
    BOOL OnDrawItem( LPDRAWITEMSTRUCT di );
    BOOL OnCommand( HWND hWndCtl, int CtlID, UINT Msg );
    int OnCharToItem( WORD key, HWND hWndCtl, int Index );

    void DrawListItem( LPDRAWITEMSTRUCT di );

    int NextTokenLength( const char *str, const char *&next_str,
    					int scr_pos, int &next_scr_pos );
    void DrawListItemText( LPDRAWITEMSTRUCT di );
    int ListItemTextWidth( const char *str );

    void DrawListItemSelect( LPDRAWITEMSTRUCT di );
    void DrawListItemFocus( LPDRAWITEMSTRUCT di );

	// added by Eugen to allow virtual listbox

	enum { WM_TOPLINECHANGED = WM_USER+100 };

	int GetLine( int index );
	int GetCurrentLine();
	int GetTopLine();
	void SetCurrentLine( int line );
	void SetTopLine( int line );
	void UpdateTopLine();		// modify before&after lines
	void UpdateLength();

	int FirstLine;		// first line that is in listbox now
	int TopLine;		// last top line (first on page) for which we
						// update number of lines before and after

	void SetUpdate();
	void OnChangeTopLine();

	void SetListBoxLen();
    int InitAddListItems(); // returns max width of lines

    BOOL HandleListBox( HWND hWndCtl, int CtlsID, UINT Msg );

    void OnOk();
    void OnCancel();
    void OnSearch();
    void OnNext();

    void EndMenuSelect();
    void EndMenuCancel();
    void DoSearch( int org_row );
	void RedrawWait( void );
	void EnableWindows( BOOL Enable );

	static TPopUpMenu * MPtr( HWND hWnd )
	  {	return (TPopUpMenu*)GetWindowLong(hWnd,DWL_USER); }
  };

#endif


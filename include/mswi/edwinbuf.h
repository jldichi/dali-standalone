#include <mswi/wedwin.h>
#ifndef INC_EDWINBUF_H
#define INC_EDWINBUF_H

class WINDOW;
class BUFFER;

class WINDOW
  {
  public:
  	friend class BUFFER;

	WINDOW( char * name, int t_name );
	WINDOW( BUFFER * bp );
	~WINDOW();

	WINDOW *next, *prev;

	BUFFER *bufp;
	enum
	  {
	  	WIN_CHANGED = 1,
	  	WIN_PROCESSING = 2,
	  	WIN_DISABLED = 4
	  } Flags;

	HWND hWnd, hWndEdit;

	void ShowTitle();
	void SelectBuf( BUFFER * bp );
	void Enable();
	void Disable();

  private:
	HFONT hFont;
	COLORREF TextColor, BackColor;
	HBRUSH hBrush;

	enum
	  {
	  	GWL_EDIT_OBJECT  = 0,
		GW_EDIT_TOTAL    = 4
	  };
	enum
	  {
	  	GCW_EDIT_CLASSID = 0,
		GC_EDIT_TOTAL    = 2
	  };
	enum
	  {
		IEW_EDIT   = 1000,
		IEW_SAVE,
	  };

  	void Link();
  	void Create();
  	void SetCurrent();

	void OnCreate();
	void OnDestroy();
	void OnSize();
	BOOL OnSetFocus( HWND hWndFocus );
	void OnKillFocus();
	HBRUSH OnCtlColorEdit( HDC hDC, HWND hWndCtl );

	void OnSave();
	void OnLoad();
	void OnExit();
	void OnProcess();
	void OnPrevBuffer();
	void OnNextBuffer();
	void OnCut();
	void OnCopy();
	void OnPaste();
	void OnUndo();
	void OnSearch();
	void OnSearchNext();
	void OnChangeCase( BOOL toupper );
	void OnFindError( int Direction );

	BOOL FindError();

	BOOL LineColShown;
	DWORD LastLineCol;
	void ShowLineCol();
	void SetStatusMsg( char * );

	BOOL OnCommand( UINT Cmd, HWND hWndCtl, UINT NCode );
	BOOL OnFunction( WPARAM wParam, LPARAM lParam );
	BOOL OnChar( WPARAM wParam, LPARAM lParam );
	BOOL OnKeyDown( WPARAM wParam, LPARAM lParam );
	BOOL OnSysKeyDown( WPARAM wParam, LPARAM lParam );

	int Execute( EHAND f, int n = 1 );

  	static BOOL Register();
  	static BOOL Registered;
  	static LRESULT CALLBACK WndProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
  	static LRESULT CALLBACK EditWndProc( HWND hWndEdit, UINT Msg, WPARAM wParam, LPARAM lParam );

  	WNDPROC PrevProc;
  	static WINDOW * ToWINDOW( HWND hWnd )
  		{ return (WINDOW*)GetWindowLong(hWnd,GWL_EDIT_OBJECT); };

  };

class BUFFER
  {
  public:
  	friend class WINDOW;

  	BUFFER();
  	BUFFER( char * name, int bflag );
  	~BUFFER();

  	BUFFER * next, * prev;

  	char * data;
  	unsigned datasize, readpos;
  	unsigned selstart, selend;

	WINDOW * winp;

	enum
	  {
	  	IS_READING	= 1,
	  	IS_NULL		= 2
	  };
	int flags;

	char  fname[NFILEN];  	/* File name                    */
	char  bname[NBUFN];   	/* Buffer name                  */
	char  ideamode;			/*buffer mode in the IDEA soft	*/

	void SetScan();
	void UnSetScan();
	void Update();
	void Show();
	int getc();
	void ungetc(int c);
	bool Set( BUFFER * bp );
	void FreeData();
	bool NewData( unsigned size );
	void SetCurrent();
  };

#endif

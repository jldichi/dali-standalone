/********************************************************************
* ORGLAND Ltd. IDEAFIX for MS Windows.
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* MODULE        : @(#)iwflddef.h
* VERSION       : 1.0 alpha
* DATE          : 93/11/20
*
* DESCRIPTION   : Definition of object TField (for WiGetField)
*
* LAST CHANGE   :
*
*********************************************************************/

#ifndef _INC_IWFLDDEF
#define _INC_IWFLDDEF

#ifndef __cplusplus
#error C++ mode required
#endif

#include <ideafix.h>
#include <ifound.h>
#include <ideafix/priv/opers.h>
#include <ideafix/priv/formrep.h>
#include <mswi/iwwidefs.h>
#include <mswi/iwwin.h>


class TField
  {
  public:
        enum
          {
                CE_FLD_SIZE   = 0,
          };
        enum
          {
                WE_FLD_PTR    = 0,
                WE_FLD_SIZE   = 4,
          };

    TField * next, *prev;       // Window fields chain
    window WindowDesc;          // Window descriptor
    IWForm * Window;
    int Row, Col;                       // Field position inside window
    type FType;           // Type of field ( if no -> IERROR )
    HWND hWnd;                          // This window

    TField( type FType = (type)ERR );
                                                // At cursor position at current window
    virtual ~TField();

    virtual void ShowField( char * data ) {};
    virtual int InputField( unsigned char * data, unsigned short opt,
                      short length, short olength, short ndec,
                      char * tstmask, char * omask ) = 0;

  protected:
    void Check();                       // Check for HWND is not NULL

    virtual BOOL OnPaint() { return FALSE; };
    virtual BOOL OnSetFocus() { return FALSE; };
    virtual BOOL OnKillFocus() { return FALSE; };
    virtual BOOL OnButtonDown( UINT state, int x ) { return FALSE; };
    virtual BOOL OnButtonUp( UINT state, int x ) { return FALSE; };
    virtual BOOL OnMouseMove( UINT state, int x ) { return FALSE; };
    virtual BOOL OnButtonDblClk( UINT state, int x ) { return FALSE; };
    virtual BOOL OnChar( WORD Char ) { return FALSE; };
    virtual BOOL OnKeyDown( WORD VKey ) { return FALSE; };
    virtual BOOL OnFunction( WORD Cmd, Int param ) { return FALSE; };
    virtual BOOL OnTimer() { return FALSE; };

  public:

	void setSkip()		{FType |= 	TY_SKIP	;};
	void unsetSkip()	{FType &= (~TY_SKIP);};
	virtual int olength() {return -1 ;}
};

class TFieldArrows : public TField
  {
  public:
    TFieldArrows( type ftype, short olength );
    ~TFieldArrows();

    virtual int InputField( unsigned char * data, unsigned short opt,
                      short length, short olength, short ndec,
                      char * tstmask, char * omask );
  protected:
        static TFieldArrows * _WndToField( HWND hWnd )
                { return (TFieldArrows*)GetWindowLong(hWnd,WE_FLD_PTR); };

        static void Register();
        static LRESULT CALLBACK FieldWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    BOOL InsideMult();

    BOOL OnVScroll( WORD Code, HWND hWndCtl, WORD Pos );
    void ScrollBy( short Delta );
    void ScrollTo( unsigned short Pos );
    void SetNewScrollPos( unsigned short NewMaxPos, unsigned short NewPos );

    void EmuKey( unsigned char Key );
    void SetNewPos( unsigned short Pos );

        HWND hWndScroll;  // HWND of scroll bar
        int MinCol, MaxCol, MinRow, MaxRow;
        unsigned short MaxPos, CurPos;
        FmFieldRep * FPtr;
  };

class TFieldInput : public TField
  {
  public:
        enum
          {
                IWFLD_SH_START          = -1,
                IWFLD_SH_HOME           = -2,
                IWFLD_SH_END            = -3,
                IWFLD_SH_WORD_LEFT      = -4,
                IWFLD_SH_WORD_RIGHT     = -5
          };

    TFieldInput( type ftype, short olength );
        TFieldInput( type ftype, int XPos, int YPos, int wWidth, int wHeight,
                                HWND Parent, attr_type at, HFONT font );
    ~TFieldInput();

    virtual void ShowField( char * data );
    virtual int InputField( unsigned char * data, unsigned short opt,
                      short length, short olength, short ndec,
                      char * tstmask, char * omask );
	int olength() {return _oLength ;}

    static int SelStart, SelEnd;        // Current selection (if == -> no selection)
    BOOL Editing() { return Cursor>=0; };       // Field is being editing
    BOOL HasSelection() { return !Editing() || SelStart!=SelEnd; };
                                                                        // Field has selection

        static TFieldInput * Current;
        static TFieldInput * NextKeyField;
        static int NextKey;

        enum { MAX_SAVE = 20 };

        static TFieldInput * SaveField;
    typedef struct
      {
        int Cursor;
        char Data[1];
      } SaveStruct;
    static SaveStruct * SaveData[MAX_SAVE];     // Save buffer for changing Data
        static int SaveCnt;

protected:
    virtual BOOL OnPaint();
    virtual BOOL OnSetFocus();
    virtual BOOL OnKillFocus();
    virtual BOOL OnButtonDown( UINT state, int x );
    virtual BOOL OnButtonUp( UINT state, int x );
    virtual BOOL OnMouseMove( UINT state, int x );
    virtual BOOL OnButtonDblClk( UINT state, int x );
    virtual BOOL OnChar( WORD Char );
    virtual BOOL OnKeyDown( WORD VKey );
    virtual BOOL OnFunction( WORD Cmd, Int param );
    virtual BOOL OnTimer( int TimerID );

        static TFieldInput * _WndToField( HWND hWnd )
                { return (TFieldInput*)GetWindowLong(hWnd,WE_FLD_PTR); };

        static void Register();
        static LRESULT CALLBACK FieldWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HFONT Font;                         // Font to use
    COLORREF Color, BackGr; // Colors

    RECT Rect;                          // Visible rectangle of window
    RECT CRect;                         // Clipping rect (in client coords)
    POINT Org;                          // Current screen origin

    static unsigned short Opt;                  // Options from call WiGetField
    static unsigned char * InOutData;   // Buffer from call WiGetField
    static char * TstMask, * oMask;       // Masks form call WiGetField
    static int oLength, Length, nDec; // Parameters from call WiGetField

    int Len;                            // Current length of Data & oData
    int oDataLen;                       // Length of oData
    char * oData;                       // Output buffer for WM_PAINT
	int _oLength;
    static int bLength;                 // Max length of next two buffers
    static char * Data;                 // Work buffer
    static int DelStart, DelCount;      // Deleted chars form PrevData

    static int Cursor;                          // Cursor position (-1 = no cursor)
    static int ScreenCursor;            // Current cursor pos on screen

    static BOOL MouseSel;                       // Processing select by mouse

    static BOOL DataChanged;            // Data is changed while editing

    virtual char FillChar();     // Filling char : default ' '

    static HFONT OldFont;       // Old font of device context
    static HDC hDC;                             // Window device context
    static int hDC_Use;                 // Use count of hDC

        void GetDC();                   // Allocate hDC from Windows
        void FreeDC();                  // Free hDC to Windows
        void _FreeDC();                 // Unconditional free hDC to Windows
        void BeginPaint( PAINTSTRUCT * ps ); // Call Windows's BeginPaint
        void EndPaint( PAINTSTRUCT * ps );       // Call Windows's EndPaint

        void SetClipping();             // Set clip region for output text
        void ClearClipping();   // Clear clip region for output text

    void Refresh();                     // Refresh window on screen

    static BOOL UpdData;        // oData needs to be updated

        BOOL TryStop( int key );        // Try to return from editing with return value
        BOOL TryStopCurrent( int key ); // Try to return Current from editing
                                                                        // than switch to this and return value
    virtual BOOL StartEdit( BOOL select = FALSE );      // Start editing of field
    void EndEdit();                                             // End editing of field
    void EndSelection();                                // End selection of chars
    virtual void SelectWord();

    void UpdateCursor();        // Update state of caret on screen
    void SetNewCursorPos( BOOL smooth = FALSE ); // Update caret position
    void DoShiftCursor( BOOL expand, int delta, int cmd = 0 ); // Process cursor
                                                                                          // movement keys

    void DrawChangeFocus(); // Display that field has input focus
    void DrawChangeInput(); // Display that field is editing
    void InvertSelection();             // Invert selection
    void InvertChars( int c1, int c2 ); // Invert chars from c1 to c2

    void ShowArea( BOOL smooth, int pos, int size = 1 ); // Make sure
                                                                // that area is visible inside window
                                                                // if smooth then don't jump

    void DelData();                     // Free all buffers
    void NewData( int blength );  // Allocate buffers with length
    void NewData( const char * data ); // Allocate buffers from string

        virtual void InitData();          // Init data when is null
        virtual BOOL NullData();          // Data is null
    virtual void InData() = 0;    // Input string from InOutData to Data
    virtual BOOL OutData() = 0;   // Output string from Data to InOutData
        virtual BOOL ValidData() = 0; // Check for Data is valid
    virtual void Refresh_oData(); // Convert Data to oData for showing
        virtual BOOL InputDone();     // It is time to Auto Enter

        virtual int StartCursor();        // Where is cursor when start editing

        virtual void PaintText() = 0; // Draw text in hDC
    virtual int CharPos( int curs ) = 0; // Position of character
    int FindPoint( int pos ); // find character which contains point
    virtual void ValidateRect();                // Make sure that window shift
                                                                        // is valid
    virtual void SetInitRect() = 0;

        BOOL ShiftWindow( int dx, int dy ); // Shift visible portion of field
    void UpdateShift();                                 // Update on screen

    BOOL CopyFromClipboard();   // Copy data from clipboard
    BOOL CopyToClipboard();             // Copy selection to clipboard

        virtual BOOL DecreaseBy( int Count ) = 0;
                        // Try to decrease length of Data by Count chars
                        // Changing Cursor if OK
        BOOL ShiftCursor( int delta, int cmd = 0 );
                        // Shift cursor by delta
        virtual BOOL ShiftCursorTo( int NewCursor );
                // Shift cursor to position

        virtual BOOL Insert( const char * s, BOOL tobegin = FALSE );
                        // Try to replace selection by s
                        // End set cursor to end/beginning of inserted
        virtual BOOL Insert( char c, BOOL tobegin = FALSE );
                        // Try to replace selection by c
                        // End set cursor to end/beginning of inserted
        virtual BOOL Delete( BOOL back = FALSE );
                                                        // Try to delete char from Data
        virtual BOOL InsertChar( char c ); // Try to insert char at cursor
        virtual BOOL ReplaceChar( char c ); // Try to replace char at cursor
        virtual BOOL DeleteChar(); // Try to remove char at cursor
        virtual BOOL BackSpace();  // Try to remove char before cursor

        void SaveUndoBuffer();
        void RestoreUndoBuffer();
  };

class TFieldLeft : public TFieldInput
  {
  public:
    TFieldLeft( type ftype, short olength ) :
                TFieldInput(ftype,olength) {};
        TFieldLeft( type ftype, int XPos, int YPos, int wWidth, int wHeight,
                                HWND Parent, attr_type at, HFONT font ) :
                TFieldInput(ftype,XPos,YPos,wWidth,wHeight,Parent,at,font ) {};

  protected:
        virtual void PaintText();
    virtual int CharPos( int curs );

    virtual void ValidateRect();
    virtual void SetInitRect();

        virtual BOOL DecreaseBy( int Count );
  };

class TFieldRight : public TFieldInput
  {
  public:
    TFieldRight( type ftype, short olength ) :
                TFieldInput(ftype,olength) {};
        TFieldRight( type ftype, int XPos, int YPos, int wWidth, int wHeight,
                                HWND Parent, attr_type at, HFONT font ) :
                TFieldInput(ftype,XPos,YPos,wWidth,wHeight,Parent,at,font ) {};

  protected:
        virtual void PaintText();
    virtual int CharPos( int curs );

    virtual void ValidateRect();
    virtual void SetInitRect();

        virtual BOOL DecreaseBy( int Count );
  };

class TFieldString : public TFieldLeft
  {
  public:
    TFieldString( type ftype, short olength ) :
                TFieldLeft(ftype,olength) {};
        TFieldString( type ftype, int XPos, int YPos, int wWidth, int wHeight,
                                HWND Parent, attr_type at, HFONT font ) :
                TFieldLeft(ftype,XPos,YPos,wWidth,wHeight,Parent,at,font) {};

  protected:
    virtual void InData();
    virtual BOOL OutData();
        virtual BOOL ValidData();
    virtual void Refresh_oData();
  };

class TFieldNumeric : public TFieldRight
  {
  public:
    TFieldNumeric( type ftype, short olength );
        TFieldNumeric( type ftype, int XPos, int YPos, int wWidth, int wHeight,
                                HWND Parent, attr_type at, HFONT font );

  protected:
    virtual char FillChar();
    virtual void InData();
    virtual BOOL OutData();
        virtual void InitData();
        virtual BOOL NullData();
        virtual BOOL ValidData();
        virtual BOOL InputDone();
        virtual int StartCursor();
        virtual BOOL ShiftCursorTo( int NewCursor );
        BOOL ValidDot();
        BOOL DeleteCommas();
        BOOL InsertCommas();
        BOOL SkipZeroes();
        BOOL AddZeroes();

        virtual BOOL InsertChar( char c ); // Try to insert char at cursor
        virtual BOOL ReplaceChar( char c ); // Try to replace char at cursor
        virtual BOOL DeleteChar(); // Try to remove char at cursor
        virtual BOOL BackSpace();  // Try to remove char before cursor

    virtual void Refresh_oData();
        static char Comma, DecPoint;
  };

class TFieldDateTime : public TFieldLeft
  {
  public:
    TFieldDateTime( type ftype, short olength, char delim );
        TFieldDateTime( type ftype, int XPos, int YPos, int wWidth, int wHeight,
                                HWND Parent, attr_type at, HFONT font, char delim );

  protected:
    int DigitSize, DelimiterSize; // Width of digits & delimiter ('/' or ':')
    char Delimiter;                                     // Delimiter

        virtual void PaintText();
    virtual int CharPos( int curs );

    virtual void InData();
    virtual BOOL OutData();
        virtual BOOL InputDone();
        virtual BOOL NullData();
        virtual void InitData();
        virtual BOOL ValidData();
        virtual ValidNumbers( int len );
        virtual ValidNumbers( int n1, int len1, int n2, int len2, int n3, int len3 ) = 0;
        virtual BOOL ShiftCursorTo( int NewCursor );

        virtual int StartCursor();

        virtual BOOL InsertChar( char c ); // Try to insert char at cursor
        virtual BOOL ReplaceChar( char c ); // Try to replace char at cursor
        virtual BOOL DeleteChar(); // Try to remove char at cursor
        virtual BOOL BackSpace();  // Try to remove char before cursor
  };

class TFieldTime : public TFieldDateTime
  {
  public:
    TFieldTime( type ftype, short olength ) :
                TFieldDateTime(ftype,olength,':') {};
        TFieldTime( type ftype, int XPos, int YPos, int wWidth, int wHeight,
                                HWND Parent, attr_type at, HFONT font ) :
                TFieldDateTime(ftype,XPos,YPos,wWidth,wHeight,Parent,at,font,':') {};

  protected:
        virtual ValidNumbers( int n1, int len1, int n2, int len2, int n3, int len3 );
  };

class TFieldDate : public TFieldDateTime
  {
  public:
    TFieldDate( type ftype, short olength ) :
                TFieldDateTime(ftype,olength,'/') {};
        TFieldDate( type ftype, int XPos, int YPos, int wWidth, int wHeight,
                                HWND Parent, attr_type at, HFONT font ) :
                TFieldDateTime(ftype,XPos,YPos,wWidth,wHeight,Parent,at,font,'/') {};

  protected:
        virtual ValidNumbers( int n1, int len1, int n2, int len2, int n3, int len3 );
  };

class TFieldBool : public TFieldLeft
  {
  public:
    TFieldBool( type ftype, short olength );
        TFieldBool( type ftype, int XPos, int YPos, int wWidth, int wHeight,
                                HWND Parent, attr_type at, HFONT font );

  protected:
        virtual BOOL OnChar( WORD c );
        virtual BOOL OnKeyDown( WORD c );
    virtual BOOL OnFunction( WORD Cmd, Int param );

        static char *Yes, *No;

    virtual void InData();
    virtual void Refresh_oData();
    virtual BOOL OutData();
        virtual BOOL NullData();
        virtual BOOL ValidData();
        virtual BOOL ShiftCursorTo( int NewCursor );
        virtual int StartCursor();
        virtual int StartEdit( BOOL select = FALSE );

        virtual BOOL InsertChar( char c ); // Try to insert char at cursor
        virtual BOOL Delete( BOOL back = FALSE );
  };

extern "C" BOOL WiFindFormField( TFieldInput * Ptr,
                                        form * fmd, fmfield * fld, int * MultiRow );

#endif /* _INC_IWFLDDEF */



#ifndef _INC_IWWINDOW
#define _INC_IWWINDOW


#define     GWL_WNDOBJ          0                     // Offset to pointer
#define     LINESPACEFACTOR     1.2


const ClsNameSize = 30;                             // Size class name
/* ------------------------------------------------- */


class TField;
class IWDispatcher;
class TFieldInput;
class IWSave;
class IWPaintSave;

// Abstract class window for MICROSOFT WINDOW
class MSWindow{
friend  TField;
friend  TFieldInput;
        BOOL DoRegister(const char*);
protected:
virtual void      DoPaint(const PAINTSTRUCT*);
        int       ptWidth;               // Width  in pixels
        int       ptHeight;              // Height in pixels
        POINT     ptOrig;                // Origin in pixels
        HWND      hWnd;                  // Handle of MS window
        int       baseFont;              // Base font of window
        HFONT     font;                  // Font   of MS window
        HCURSOR   RgCursor;              // Cursor for register
        HBRUSH    RgBackGr;              // Background for register
        HICON     RgIcon;                // Icon for register
        short     ClassID;               // Class identificator
        char*     label;                 // Label MS window
        DWORD     dwStyle;               // Style MS window
        DWORD     dwExStyle;             // Extended style MS window
virtual WNDPROC   GetWndProc()   = 0 ;   // WndProc for register
virtual BOOL      IsRegister()   = 0 ;   // ==TRUE is register
virtual void      SetRegister()  = 0 ;
static  MSWindow* PutWindowObj(HWND,LPARAM);

public:
                  MSWindow() ;
virtual           ~MSWindow() ;

           BOOL   CreateMSWindow(BOOL);
           BOOL   CreateMSWindow(BOOL,MSWindow * Parent);
           HWND   GetHWnd()      { return hWnd;     }
           short  GetClassID()   { return ClassID;  }
           HFONT  GetFont()      { return font;     }
           POINT  GetOrig()      { return ptOrig;   }
           int    GetWidth()     { return ptWidth;  }
           int    GetHeight()    { return ptHeight; }
};

// Class window of IDEAFIX

class IWWindow:public MSWindow {
friend  IWDispatcher;
friend  IWSave;
friend  IWPaintSave;


    IWWindow * parent;           // Parent window
    IWWindow * child;            // Child window
    IWWindow * next;             // proximo en la lista de hermanos
    IWWindow * prev;             // anterior en la lista de hermanos
    window     sub;              // Index in table
    attr_type  CheckFrame(attr_type border);
    void       SetLabel(char* label);
    void       SetSize(int nRowOrg,int nColOrg,int nHeight,int nWidth);
    void       DetermCharPos(int row, int col,int& y, int& x);
protected:
    short      nfils;            // Number of rows
    short      ncols;            // Number of columns
    IWPaintSave* pSave;          // Text image of window
    int        nLnSpace;
    int        nChWidth;         // Width character
    int        nChHeight;        // Height character
virtual void   DoPaint(const PAINTSTRUCT*);  // Paint IWWindow
    void       SetMSWindow();
public:
     attr_type backgr;             // Background of window
     attr_type border;             // Border type
     unsigned char     flags;
     short     f_org;              // Row of origin in text screen
     short     c_org;              // Column of origin in text screen
     attr_type attrib;             // Current text attribute
     short     c_fil;              // Row  cursor
     short     c_col;              // Col. cursor
     short     dc;
public:
     IWWindow(int nRowOrg,int nColOrg,int nHeight,int nWidth,
            attr_type AtrBorder,char* Label,attr_type AtrBackGr);
     ~IWWindow();

      void      SetWd(window new_wd)          { sub    = new_wd;   }
      void      SetParent(IWWindow* lpParent) { parent = lpParent; }
      window    GetSub()              const   { return sub;        }
      IWWindow* GetParent()           const   { return parent;     }
      IWWindow* GetChild()            const   { return child;      }
      IWWindow* GetNext()             const   { return next;       }
      IWWindow* GetPrev()             const   { return prev;       }
      int       GetNRows()            const   { return nfils;      }
      int       GetNCols()            const   { return ncols;      }
      attr_type GetBackGr()           const   { return backgr;     }
      void      SetBackGr(attr_type bgr)      { backgr = bgr;      }
      short     Get_c_fil()           const   { return c_fil;      }
      void      Set_c_fil(short i)            { c_fil = i;         }
      short     Get_c_col()           const   { return c_col;      }
      void      Set_c_col(short i)            { c_col = i;         }
      short     Get_c_org()           const   { return c_org;      }
      short     Get_f_org()           const   { return f_org;      }
      attr_type GetAttr()                     { return attrib;     }
      void      SetAttr(attr_type a)          { attrib = a;        }
      void      MoveTo(int,int);
      void      RMoveTo(int,int);
      void      SetFlag(BOOL,unsigned char);
     IWPaintSave*   GetSave()         const   { return pSave;      }
};


// Class form window

class IWForm: public IWWindow {
static BOOL    bRegister      ;
static LRESULT CALLBACK FormIWWndProc (HWND, UINT,WPARAM,LPARAM );
       void    SetRegister()  ;
       BOOL    IsRegister()   ;
       WNDPROC GetWndProc()   ;
public:
       HWND     hwndFocus;
       TField * fields;
public:
         IWForm(int nRowOrg,int nColOrg,int nHeight,int nWidth,
                attr_type AtrBorder,char* Label,attr_type AtrBackGr);
         IWForm(int nRowOrg,int nColOrg,int nHeight,int nWidth,
                attr_type AtrBorder,char* Label,attr_type AtrBackGr,int par_wd);
        ~IWForm();

         TField*  GetField()             { return fields;   }
         void     SetField(TField* fld)  { fields = fld;    }
         HWND     GetHWndFocus()         { return hwndFocus;}
         void     SetHWndFocus(HWND fcs) { hwndFocus = fcs; }
};

// class default window

class IWDefault: public IWWindow {
static BOOL bRegister;
static LRESULT CALLBACK DefIWWndProc (HWND,UINT,WPARAM, LPARAM);
       void SetRegister();
       BOOL IsRegister()    const;
       WNDPROC GetWndProc() ;
public:
         IWDefault(int nRowOrg,int nColOrg,int nHeight,int nWidth,
                attr_type AtrBorder,char* Label,attr_type AtrBackGr);
        ~IWDefault();
};

//extern void PASCAL DetermCharPos(int row, int col, MSWindow *lpw, int& y, int& x);
typedef IWWindow * PIWWindow ;


extern IWWindow *  wcurr;          /* Pointer to current window        */

inline MSWindow *GetWindowObj(HWND hWnd) {
          return (MSWindow *) GetWindowLong(hWnd, GWL_WNDOBJ);
  }

extern BOOL WINAPI  __GetIWWindow(window wd, IWWindow * & lpIWWnd);
extern IWForm*     __GetIWForm(window wd);

/* ----------- Other --------------- */
extern void WINAPI DetermMetrics(int& nLineSpace, int& nOptCharWidth, int& nCharHeight, HFONT font);
extern void WINAPI DetermCharMetrics(int& nLineSpace, int& nOptCharWidth,
                          HFONT font);
extern void WINAPI DetermFullCharMetrics(int& nLineSpace, int& nOptCharWidth,
                          HFONT font);
extern void WINAPI DetermCharPos(int row, int col, MSWindow *lpw, int& y, int& x);
extern int  WINAPI GetOffsetX(MSWindow *lpw);
/*extern LRESULT CALLBACK GenDefWndProc (HWND, UINT, WPARAM, LPARAM);*/

extern void w_deleteField( TField * Ptr );

#endif  /* _INC_IWWINDOW */



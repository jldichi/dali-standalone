/********************************************************************
* ORGLAND Ltd. IDEAFIX for MS Windows.
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* MODULE        : @(#)iwdispat.h
* VERSION       : 1.0 alpha
* DATE          : 2-15-94
* DESCRIPTION   : Scroll Window for IdeaFix text
*
********************************************************************/
#ifndef _INC_IWSCROLL
#define _INC_IWSCROLL

#include <mswi/iwwin.h>

#define BSIZE 512		// Line buffer size
#define MAXLINES 1024		 // Max. number of lines in file
#define SIZE_BUF_SEARCH 80   // Max. search buff size

class IWScrollModel{
protected:
         int   nLines;
         int   nCol;
public:
         IWScrollModel() {  nLines = 0; nCol = 0;}
virtual  ~IWScrollModel() {};

virtual  char * GetPText(int)     = 0;
         int    GetHeight(void){ return nLines;   }
         int    GetWidth(void) { return nCol;   } 

};


//Test class for ScrollModel
class IWScrText: IWScrollModel{
	char** pStr;
public:
      IWScrText( char** p,int w,int h) { pStr = p; nCol=w;nLines=h;}
      ~IWScrText() {};
      char * GetPText(int i)  { return pStr[i];}
};


class IWScrollFile: public IWScrollModel {
       char* szBuf;
       WORD* wOffset;
       FILE* fp;
       void  SetOffset();
public:
    IWScrollFile();
    ~IWScrollFile() ;

      BOOL   Open( const   char*);
      BOOL   OpenPath(const char*);
      char * GetPText(int) ;
};

class IWScrollFunc: public IWScrollModel {
     char*   offset;
     CPFPCPI GetTxt;
     void    SetSize(void);
public:
    IWScrollFunc(CPFPCPI,char*);
    ~IWScrollFunc();

     char * GetPText(int);
};


class IWScrollText: public IWWindow {
static BOOL    bRegister;
static LRESULT CALLBACK ScrollWndProc (HWND , UINT ,WPARAM ,LPARAM );
static char*          GetOptx(IWScrollModel*,int);
       BOOL           IsRegister()    ;
       WNDPROC        GetWndProc()    ;
       HBRUSH         GetBackGr()     const;
       HICON          GetIcon()       const;
       void           SetRegister();
       void           DoPaint(const PAINTSTRUCT*);
       void           Size(HWND);
       int            nXPos,nYPos;
       int            nXRange,nYRange;
       int            nXPage,nYPage;
       int            nXLine,nYLine;
       IWScrollModel* pModel;
public:

       IWScrollText(IWScrollModel* model,int nRowOrg,int nColOrg,
                    int nHeight,int nWidth, attr_type AtrBorder,
                    char* Label,attr_type AtrBackGr);
        ~IWScrollText();


         void SetSBarRange();
         void ScrollTo(int nX, int nY);
         void ScrollBy(int Dx, int Dy);
         void VScroll(WORD ScrollEvent, int nThumbPos);
         void HScroll(WORD ScrollEvent, int nThumbPos);
         void EndView();
         void KeyScroll(WORD);
         void SearchDialog(void);
         void DoSearch(void);
};

#endif


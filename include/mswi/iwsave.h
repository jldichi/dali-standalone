/********************************************************************
* ORGLAND Ltd. IDEAFIX for MS Windows.
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* MODULE        : @(#)iwdispat.h
* VERSION       : 1.0 alpha
* DATE          : 20-15-94
* DESCRIPTION   : Convert IdeaFix text image to graphic image
*
********************************************************************/

#ifndef _INC_IWSAVE
#define _INC_IWSAVE

#include <mswi/iwwin.h>

typedef unsigned long   VS_CELL;
#define VS_CHAR(x)     ((unsigned char) (INT((Int)x) & 0xFF))
#define VS_ATTR(x)     ((x) & (VS_CELL) 0xFFFFFF00)
#define VS_BACKGR(x)   ((x) & (VS_CELL) \
				       (A_INVISIBLE|A_RED_BG|A_BLUE_BG|A_GREEN_BG|A_REVERSE))
#define VS_COLOR(x)    ((x & A_WHITE) >> 16)
#define VS_BG_COLOR(x) ((x & A_WHITE_BG) >> 20)

#define TL_SINGLE  0
#define TL_DOUBLE  1

const int nMaxText=130;

class DrawItem {
protected:
     attr_type  dwPAttr;         // Previos attribute
     attr_type  dwCAttr;         // Current attribute
     BOOL       bState;          // Current state, = True - draw
     POINT      p1;              // Origin  item
     int        nYOffset;
public:
     DrawItem()  { bState = FALSE; }
     ~DrawItem() {}

virtual  void Draw(HDC) = 0;
         void SetState(BOOL bst)    { bState = bst; }
         BOOL IsDraw(void)    const { return bState;}
};

class GText: public DrawItem {
     int       defFont;         // Default font
     char      szBuf[nMaxText];  // Buffer of string
     int       nChar;            // Count characters
     HFONT     SelectFont(attr_type);
     HFONT     ChangeAttr(HDC);
public:
     GText()  {}
     ~GText() {}

     void   InitText(attr_type,int,int,BYTE);
     void   Add(BYTE);
     void   Draw(HDC);
     BOOL   IsPrevSpace()         const;
     int    GetEndText(HDC hDC)   const  { SIZE s;
		GetTextExtentPoint(hDC,szBuf,nChar,&s);return p1.x+s.cx;}
     HFONT  InitAttr(HDC,int,int,attr_type);
};

class GLine: public DrawItem {
      POINT  p2;
      int    nTypLine;                        // Current type of line
      int    nPTypLine;                       // Previus type of line
      int    nXOffset;
      HPEN   hPen;                            // Current pen
      HPEN   ChangeAttr(HDC);
      void   SetArrowPen(HDC);
public:
     GLine()  { nTypLine = TL_SINGLE; }
     ~GLine() {}

     void   Draw(HDC);
     void   InitHLine(attr_type,int,int,int,int);
     void   InitVLine(attr_type,int,int,int,int);
     int    GetTypLine()      const { return nTypLine;}
     void   SetX2(int x2)           { p2.x = x2;      }
     void   SetY2(int y2)           { p2.y = y2;      }
     void   DrawLArrow(attr_type,HDC,int,int);
     void   DrawRArrow(attr_type,HDC,int,int);
     void   DrawUArrow(attr_type,HDC,int,int);
     void   DrawDArrow(attr_type,HDC,int,int);
     void   DrawBullet(attr_type,HDC,int,int);
     void   DrawBlock(attr_type,HDC,int,int,int,int);
     HPEN   InitAttr(HDC,int,int,attr_type);  
     HPEN   GetPen()          const { return hPen;   }
};


class IWSave {
      BOOL      in_sec;
      void      DelLine(int,int,int,int);
      void      InsLine(int,int,int,int);
      void      PutCell(int,int,VS_CELL);
      void      PutChar(unsigned char);
      void      Down(void);
      void      Up(void);
      void      BackSpace(void);
      BOOL      PutSec(unsigned char);
      void      RawPutC(unsigned char);

public:
      short     top_scrl;           // Top of scroll area
      short     bot_scrl;           // Bottom of scroll area
      short     left_scrl;          // Left of scroll area
      short     right_scrl;         // Right of scroll area
      int       nCurXScroll;
      int       nPosXScroll;
protected:
      VS_CELL  *Save;
      IWWindow *pWin;
public:
      IWSave(IWWindow*);
      ~IWSave();

      void       InsChar(int,int,int);
      void       DelChar(int,int,int);
      void       FillCell(attr_type,short,short,short,short);
      void       EraseCell(int,int,int,int);
      void       DelLine(int);
      void       InsLine(int);
      void       Scroll(int);
      void       Update(void);
      void       PutC(unsigned char);
      void       PutString(const unsigned char*);
      void       Erase(void);
      void       InsChar(int,int);
      void       DelChar(int,int);
      void       SetScroll(int,int,int,int);
      VS_CELL    GetCell(int,int);
      void       NewLine(void);
      void       CreateSave(void);
      VS_CELL*   GetSave()   { return Save;}
};
enum State {eSkip,ePGraph,eText};

class IWPaintSave: public IWSave {
    HDC         hDC;
    HWND        hWnd;
    GText*      gt;
    GLine*      gl;
    attr_type  dwPAttr;
    attr_type  dwCAttr;
    int        nCurPxlRow,nCurPxlCol;
    int        nCurTxtRow,nCurTxtCol;

    void       PaintBackGr(int,int,int,int,BOOL);
    void       H_StatSkip(State);
    void       H_StatText(State,BYTE);
    void       H_StatPGraph(State,BYTE);
    void       H_GenPGraph(BYTE);
    void       H_Paint(int,int,int,int);
    void       H_LChar(int);
    void       H_RChar(int);
    void       H_FChar(int);
    void       H_LArrow();
    void       H_RArrow();
    void       H_Bullet();
    void       V_Block();
    void       V_HChar(int);
    void       V_LChar(int);
    void       V_FChar(int);
    void       V_DArrow();
    void       V_UArrow();
    void       V_StatSkip(State);
    void       V_StatPGraph(State,BYTE);
    void       V_GenPGraph(BYTE);
    void       V_Paint(int,int,int,int);
    State      GetState(BYTE ch);
    BOOL       boSepSpace;
public:
    IWPaintSave(IWWindow*,BOOL=TRUE);
    ~IWPaintSave();

    void    DoPaint(HDC,int,int,int,int,BOOL=TRUE);
};

#endif

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
* DESCRIPTION   : MS Window for Ideafix report
*
********************************************************************/
#ifndef _INC_IWREPORT
#define _INC_IWREPORT
#include <mswi/iwwin.h>

class IWReport: public IWWindow {
static BOOL    	      bRegister;
static LRESULT CALLBACK ReportWndProc (HWND , UINT ,WPARAM ,LPARAM );
    BOOL           IsRegister()    ;
    WNDPROC        GetWndProc()    ;
    HBRUSH         GetBackGr()     const;
    HICON          GetIcon()       const;
    void           SetRegister();
    void           DoPaint(const PAINTSTRUCT*);
    void           DoSize(HWND);
    void           TestWidth(void);
    void           ChangeYScroll(int);
    void           ChangeXScroll(void);
	void 	      SetDownScroll(void);
	void          ChngSizeX(int);
	void	      ChngSizeY(int);
	void	      DoGetMinMax(MINMAXINFO*);
	BOOL 		   AlignClientArea(HWND);
    BOOL           bChngSizeX;          // = TRUE, if change X window size
    BOOL           bChngSizeY;          // = TRUE, if change Y window size
    int			   nAddX;
    int			   nAddY;
	int            nYPos;               // Current y position in report
    int            nYRange;             // Range y of report
    BOOL           bBackFill;           // = TRUE,  if fill all client area
    BOOL           bSetRange;           // = TRUE, if set end range
    RECT           ImgRect;             // Current image rectangle
	TerminalOutput* out;	// Link with IdeaFix report out
	int			   nOldOffset;
public:

       IWReport(int,int,int,int,attr_type,char*,attr_type);
       ~IWReport();

       void ScrollByY(int);
       void VScroll(WORD,int);
       void HScroll(WORD,int);
       void DoScroll(unsigned char);
       void SetOutput(TerminalOutput*);
	void FixRange(void);
	BOOL GetbChngSizeX(void);
	BOOL GetbChngSizeY(void);
	void FixScrollPos(unsigned char);
	void DoClear(void);
	void SaveOffset()  { nOldOffset = out->offset; }
};

class IWChart: public IWWindow {
static BOOL    bRegister;
static LRESULT CALLBACK IWChartWndProc (HWND , UINT ,WPARAM ,LPARAM );
       BOOL           IsRegister()    ;
       WNDPROC        GetWndProc()    ;
       HBRUSH         GetBackGr()     const;
       HICON          GetIcon()       const;
       void           SetRegister();
public:

       IWChart(int,int,int,int,attr_type,char* ,attr_type);
        ~IWChart();
};

#endif


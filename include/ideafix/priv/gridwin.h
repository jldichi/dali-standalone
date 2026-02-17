// -------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: gridwin.h,v 1.1.1.1 1996/04/16 18:51:42 gustavof Exp $
//
// Description:
// -------------------------------------------------------------------------

#ifndef GRIDWIN_H
#define GRIDWIN_H

#define gridSMALL_FONT   0
#define gridMEDIUM_FONT  1
#define gridLARGE_FONT   2

#include <ideafix/priv/gridwx.h>

class Grid;
class GridRep;
class GridDev;
class WxGridDev;
class GridColumn;
class wxFrame;
class wxItem;
class wxBrush;
class wxPen;
class wxFont;
class wxColour;
class wxKeyEvent;
class wxMouseEvent;

class FontBox;
class ShowBox;


class GCanvas : public wxCanvas
{

public:

	enum COLORS {
		WHITE = 0,
		FORE  = 1,
		HALF  = 2,
		BACK  = 3,
		DEEP  = 4,
		BLACK = 5
	};

	enum FONTS  {
		NO_FONT     = -1,
		SMALL_FONT  = 0,
		MEDIUM_FONT = 1,
		LARGE_FONT  = 2
	};

protected:

	WxGridDev  *gdev_;
	wxPen      *pen_;
	wxBrush    *brush_;
	wxColour   *pen_colour_;
	wxColour   *brush_colour_;
	wxColour   *text_colour_;
	wxFont     *font_;

	wxCursor *size_c_;
	wxCursor *norm_c_;
	wxCursor *curr_c_;
	wxCursor *wait_c_;

	unsigned long fore_;
	unsigned long half_;
	unsigned long back_;
	unsigned long deep_;
	unsigned long black_;
	unsigned long white_;
	enum FONTS font_size_;
	Bool bold_;
	Int char_width;
	Int char_height;
	Int char_descent;
	Int interLines_;

	Int start_x_;
	Int last_x_;
	Int selected_column_;

	Int type__;

public:

	/*explicit*/ GCanvas(WxGridDev *table);
	virtual ~GCanvas(void);

	void SetClippingRegion(Int x,Int y,Int w,Int h);

	virtual void makeColors(void);
	void SetPen(enum GCanvas::COLORS color = BLACK,int style = wxSOLID,int width = 0);
	void SetBrush(enum GCanvas::COLORS color = BACK,int style = wxSOLID);
	wxCursor *SetCursor(wxCursor *cursor);
	virtual void setText(enum GCanvas::COLORS color = BLACK);
	Bool Show(Bool show);
	virtual void Clear(void);
	wxFont* font();

	virtual void setFontAttributes(GCanvas::FONTS font = SMALL_FONT,Bool bold = FALSE);
	virtual void getFontAttributes(GCanvas::FONTS *,int *);

	virtual Int addOffset(void) const = 0;

	virtual void drawButton(int x,int y,int w,int h,bool press = false,bool fill = false);

	virtual	bool checkDelimiter(Int c,Int x,Int w);
	virtual	bool checkColumn(Int c,Int x,Int w);
};

class GridFrame : public wxFrame
{

private:

	WxGridDev *gdev_;
	FontBox *fontBox_;
	ShowBox *showBox_;
	Bool fontChanged_;
	wxMenuBar *menuBar_;

public:

	GridFrame(WxGridDev *table, wxFrame *parent, char *title, int x=-1, int y=-1,
			  int width=-1, int height=-1, int style=wxSDI|wxDEFAULT_FRAME,
			  char *name = "gridframe");

	virtual ~GridFrame(void);

	void SetSizeHints(int minW, int minH,int maxW, int maxH, int incW, int incH);
	virtual Bool OnClose(void);	
	virtual void OnSize(int w, int h);
	void updateSizeHints();
	void updateNumbersMenu();
	void updateShowBox();
	void printGrid();
	void saveGrid();
	
	void OnMenuCommand(int choise);

	virtual Bool isFontChanged(void);
	virtual void setFontAttributes(GCanvas::FONTS hfont = GCanvas::SMALL_FONT,
								   Bool hbold = TRUE,
								   GCanvas::FONTS gfont = GCanvas::SMALL_FONT,
								   Bool gbold = FALSE,
								   Bool mode = FALSE);
};


class GridCanvas : public GCanvas
{

protected:

	Int select_row_;
	Int select_col_;
	Int start_row_;
	Int last_row_;
	Int start_col_;

public:

	/*explicit*/ GridCanvas(WxGridDev *table);
	virtual ~GridCanvas(void);

	void OnPaint(void);
	void OnChar(wxKeyEvent &);
	void OnEvent(wxMouseEvent &);

	void SetClippingRegion(Int x, Int y, Int w, Int h);
	virtual void Clear(void);
	virtual void Clear(Int, Int, Int, Int, GCanvas::COLORS color = WHITE,
					   int style = wxCOPY);
	void Scroll(int, int);

	virtual Int addOffset(void) const { return 0; };
	Int sbarWidth(void);

	Int charWidth(void) const	{ return char_width;	};
	Int charHeight(void) const	{ return char_height;	};
	Int charDescent(void) const	{ return char_descent;	};

	virtual Int xCharToPixel(Int xpos=1)const;
	virtual Int yCharToPixel(Int ypos=1)const;
	virtual Int xPixelToChar(Int xpos)const;
	virtual Int yPixelToChar(Int ypos)const;
	virtual void where(Int *,Int *,Int *);

	virtual void drawColumn(Int col,Int x,Int from,Int w,Int to);
	virtual void drawColumns(Int from,Int to);
	virtual void drawColumns(Int x,Int y,Int w,Int h);
	virtual void drawDelimiter(Int col,Int from,Int qn);
	virtual void drawCell(Int col,Int row);
	virtual void drawCellSelection(void);
	virtual void scrollText(Int col,Int row);
	virtual void drawSelectedLine(Int row);

	virtual void drawLeadings(Int col,Int from,Int qn);
	virtual Int stringOffset(char *str,Int col,Int c_off);

	virtual void drawBoundCursor(Int x);
	virtual void drawTriangle(Int x,Int y,Int dir = 0);

	Int interLines(void) const	{ return interLines_; };
	void interLines(Int i)		{ interLines_ = i; };

};

class LGridCanvas : public GridCanvas
{

public:

	/*explicit*/ LGridCanvas(WxGridDev *table);
	~LGridCanvas(void);
	virtual void updateSize(void);
	virtual void scroll(Int n);
	virtual void OnPaint(void);
	void OnEvent(wxMouseEvent &);
	void Scroll(int, int);
	Bool Show(Bool show);

	virtual void drawColumns(Int x, Int y, Int w, Int h);
	virtual void drawCell(Int row, Int col);
	virtual void drawBoundCursor(Int x);

	virtual Int width(void);
	virtual Int addOffset(void) const;
};

class HeadCanvas : public GCanvas
{

	Int popupColumn_d;
	wxMenu* popupMenu_;
	
public:

	/*explicit*/ HeadCanvas(WxGridDev *table);
	virtual ~HeadCanvas(void);

	void OnPaint(void);
	void OnChar(wxKeyEvent &);
	void OnEvent(wxMouseEvent &);
	void popupMenu(float x,float y);
	void popupFunction(Int command);
	void SetClippingRegion(Int x, Int w);
	void Clear(void);
	virtual void Clear(Int, Int, GCanvas::COLORS color = BACK,
					   int style = wxCOPY);

	virtual Int addOffset(void) const;

	virtual void drawDelimiter(Int col,bool inverse=false,bool fill=true);
	virtual void drawHeader(Int col,Int x,Int w);
	virtual void drawHeaders(Int x,Int w);
	virtual void moveArea(Int x1,Int w,Int x2);

	virtual void where(Int *,Int *);
	virtual void drawBoundCursor(Int x);

	Int	interLines(void) const	{ return interLines_; };
	void interLines(Int i)	    { interLines_ = i; };

	virtual Int height(void);
	
};

class LHeadCanvas : public HeadCanvas
{

public:

	/*explicit*/ LHeadCanvas(WxGridDev *table);
	virtual ~LHeadCanvas(void);

	void OnPaint(void);
	virtual void updateSize(void);
	virtual void drawHeaders(Int x, Int w);
	virtual void drawBoundCursor(Int x);
	virtual Int addOffset(void) const { return 0; };

};

#endif

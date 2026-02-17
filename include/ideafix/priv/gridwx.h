// -------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: gridwx.h,v 1.1.1.1 1996/04/16 18:51:42 gustavof Exp $
//
// Description:
// Declaration of the Grid object and related functions, for
// creating grid filled with text strings.  
// -------------------------------------------------------------------------

#ifndef GRIDWX_H
#define GRIDWX_H

#include <ideafix/priv/griddev.h>

class wxCanvas;
class wxFrame;
class String;
class GridRep;
class GridDev;
class PSGridDev;
class GridFrame;
class GridCanvas;
class LGridCanvas;
class HeadCanvas;
class LHeadCanvas;
class IconCache;
class GridIcon;

#ifdef wx_xview 
#define WxDalta 2
#else
#define WxDalta 0
#endif

class WxGridDev : public GridDev
{

protected:

	wxFrame     *wp_;
	GridFrame   *gridFrame_;
	GridCanvas  *gridCanvas_;
	LGridCanvas *lGridCanvas_;
	HeadCanvas  *headCanvas_;
	LHeadCanvas *lHeadCanvas_;

	Int select_col_;
	Int select_row_;
	Int curr_c_off_;
	
	bool messageLoopOn_;

	IconCache *icon_cache_;
	Int icon_c_qn_;

public:

	WxGridDev(GridRep *grep, const String &name, Int x, Int y);
	~WxGridDev(void);

	virtual void build();
	void endGrid(void);

	virtual void refreshAll(void);
	void refreshGrid();
	void refreshLGrid();

	virtual bool selectedCell(Int *col = NULL,Int *row = NULL);
	virtual void selectCell(Int col,Int row);
	virtual void unSelectCell(void);
	virtual void stringOffset(Int c_off = 0);
	virtual Int  stringOffset(void);
          
	virtual void drawColumns(Int x,Int y,Int w,Int h);
	virtual void drawColumns(Int from,Int to);
	virtual void drawCell(Int col,Int row);
	virtual void drawSelectedLine(Int row);

	virtual void updateVirtualSize(void);

	void showColumn(Int n , bool sh);
	void lockColumn(Int c);
	Int columnWidth(Int n) const;
	void columnWidth(Int n,Int w);

	virtual void xOffset(Int offset);
	virtual void yOffset(Int offset);
	virtual Int xOffset();
	virtual Int yOffset();

	virtual Int xCharToPixel(Int xpos = 1) const;
	virtual Int yCharToPixel(Int ypos = 1) const;
	virtual Int xPixelToChar(Int xpos = 1) const;
	virtual Int yPixelToChar(Int ypos = 1) const;

	Int sbarWidth(void) const;

	void startMessageLoop(void);
	void stopMessageLoop(void);

	virtual void showNumbers(bool show);

	inline GridFrame*   gridFrame(void);
	inline GridCanvas*  gridCanvas();
	inline LGridCanvas* lGridCanvas();
	inline HeadCanvas*  headCanvas();
	inline LHeadCanvas* lHeadCanvas();
	
	PSGridDev* printDev();
	
	void setLines(Int x,Int y);
	void feedIconCache(Int from,Int to);
	GridIcon *icon(const String &name);
};

#include <ideafix/priv/gridwx.icc>

#endif

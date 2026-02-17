// -------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: gridrep.h,v 1.1.1.1 1996/04/16 18:51:42 gustavof Exp $
//
// Description:
// Declaration of the Grid object and related functions, for
// creating grid filled with text strings.  
// -------------------------------------------------------------------------
                                           
#ifndef GRIDREP_H
#define GRIDREP_H

#define OUT_PIPE		(UnShort) 0
#define OUT_FILE		(UnShort) 1
#define OUT_PRINTER		(UnShort) 2
#define OUT_TERMINAL	(UnShort) 3
#define OUT_DEFAULT		(UnShort) 4
#define OUT_WX			(UnShort) 5
#define OUT_PS			(UnShort) 6
#define OUT_DUMP		(UnShort) 7
#define OUT_DELIMITED	(UnShort) 8
#define OUT_FIXED		(UnShort) 9

class GridColumn;
class GridDev;
class WxGridDev;
class Grid;
class String;
class GridIcon;
typedef String *StringArray;
class gtype(PtrArray,StringArray);
class gtype(PtrArray,GridColumn);
class gtype(PtrArray,Int);

class GridRep
{
	useIdCheck(GridRep_id);

protected:

	Grid *grid_;
	String name_;

	gtype(PtrArray,StringArray) *lines__;    // Buffer for all
	StringArray* pool__;                     // added array
	gtype(PtrArray,GridColumn) *columns__;   // Columns
	gtype(PtrArray,Int) *slines__;           // Selected lines

	FILE *fp_;
	FILE *fp_index_;

	Int       currCol_;          // Current output column
	Int       currRow_;          // Current output row
	Int       poolRow_;          // Row in pool
	Int       lastLocked_d;      // Last locked column
	Int       clickOnRow_d;      // 

	Int       pageHeight_;       // page Height
	Int       pageWidth_;        // page Width
	Int       virtualWidth_;     // virtual Width
	Int       virtualHeight_;    // virtual Height

	Int       base_;
	Int       items_;            // Number of items listed
	Int       offset_;           // Current offset

	bool gridIsBuild_;           // flag of grid ending
	bool gridIsDone_;            // flag of grid ending
	bool gridIsAbort_;           // flag of grid aborting
	bool showNumbers_;           // flag shows numbers
	
	GridDev *currDev_;           // current device
	void* clickedOn_d;

public:

	GridRep(Grid *grid,const String &name);
	virtual ~GridRep(void);

///////////////////// ALL ABOUT GRIDREP //////////////////

	Int pageWidth(void) const;
	Int pageHeight(void) const;
	Int virtualWidth(void);
	Int virtualHeight(void);
	bool isNumbers(void) const;
	Int numbersWidth(void);
	Int base(void) const;
	Int items(void) const;
	inline bool gridIsDone(void) const;
	inline bool gridIsAbort(void) const;
	inline void endGrid(void);

	virtual void build(void);
	virtual void execute(void);
	virtual void restart(void);
	virtual void abortGrid(void);
	virtual void showNumbers(bool show);
	virtual Int scroll(Int n = 1);
	void setLines(Int y=ERR,Int x=ERR);
	void base(Int newbase);

	virtual void onClick(Int col, Int row);
	bool clickedOn(void*) const ;
	Int  idOf(void*) const;
	bool row(Int r);
	Int row() const;
	const String& value(Int row,Int col);
	const String& value(Int colId) const;
	void value(Int colId, const String &v);

///////////////////// ALL ABOUT LINES   //////////////////

	virtual void freeLines(void);
	virtual void selectLine(Int row);
	virtual void unSelectLine(Int row);
	virtual bool lineSelected(Int row);
	virtual bool isSelection(void);
	virtual Int  nLinesSelected(void);

///////////////////// ALL ABOUT COLUMNS //////////////////

	Int addColumn(GridColumn *);

	Int addCol(String &theString, const String &title, Int width,
			Int flags = 0, const String &mask = NULL_STRING);
	Int addCol(Date &theDate, const String &title, Int width, Int flags = 0, 
			const String &mask = NULL_STRING);
	Int addCol(Time &theTime, const String &title, Int width,
			Int flags = 0, const String &mask = NULL_STRING);
	Int addCol(Boolean &theBoolean, const String &title, Int width,
			Int flags = 0, const String &mask = NULL_STRING);
	Int addCol(Num &theNum, const String &title, Int width,
			Int flags = 0, const String &mask = NULL_STRING);
	Int addCol(Num &theNum, const String &title, Int width, Int flags, 
			Int ndig, Int ndec = 0, bool separ = true,
			Int sign = SIGN_LEADING, char filler = ' ');
	Int addCol(Int &theInt, const String &title, Int width,
			Int flags = 0, const String &mask = NULL_STRING);
	Int addCol(GridIcon &theIcon, const String &title, Int width,
			Int flags=0 , const String &mask = NULL_STRING);

	Int addTypedCol(Int ty, const String &title, Int width, Int flags=0,
			const String &mask = NULL_STRING);
	Int addTypedCol(Int ty, const String &title, Int width, Int flags,
			Int ndig, Int ndec = 0, bool separ = true,
			Int sign = SIGN_LEADING, char filler = ' ');

	void flags(Int column, Int flags);

	Int columnWidth(Int n) const;
	Int columnOffset(Int n) const;
	bool columnLocked(Int n) const;
	Int columnType(Int n) const;
	GridColumn *gridColumn(Int n) const;

	Int columnNdec(Int n);
	Int nCols() const;
	inline Int currCol(void) const;
	void showColumn(Int c,bool sh);
	bool columnShowed(Int c) const;
	bool columnClickable(Int c) const;
	bool columnSizeable(Int c) const;
	bool columnLockable(Int c) const;
	bool columnPrintable(Int c) const;
	virtual void lockColumn(Int c);
	virtual Int lastLocked(void);
	virtual void columnWidth(Int col, Int width);

///////////////////// ALL ABOUT OUTPUT  //////////////////

	virtual	void outputLine();
	void readFromFile(Int n, Int qn = 1);
	virtual	void logInFile(StringArray*);
	String formattedValue(Int, Int);
	virtual bool onNextLine();
	virtual	void getLinesFromUser(Int startRow, Int n = 1);

//////////////////// ALL ABOUT DEVICES ///////////////////

	GridDev *currDev(void) const;
	void currDev(GridDev*);
	virtual bool openWxGridDev(Int x=ERR, Int y=ERR, Int w=ERR,Int h=ERR);
};

#include <ideafix/priv/gridrep.icc>

#endif


#if 0
#define OUT_PIPE 		(UnShort) 0
#define OUT_FILE		(UnShort) 1
#define OUT_PRINTER		(UnShort) 2
#define OUT_TERMINAL	(UnShort) 3
#define OUT_DEFAULT		(UnShort) 4
	virtual TerminalOutput *getTerminalOutput();
	virtual StreamOutput *getStreamOutput();
	virtual GraphOutput *getGraphOutput();
	virtual ReportOutput *getReportOutput();
	virtual DelimitedOutput *getDelimitedOutput();
	virtual RemoteOutput *getRemoteOutput();
	virtual GridOutput *getGridOutput();
	virtual Grid *getGrid();
	virtual CGrid *getCGrid();
#endif

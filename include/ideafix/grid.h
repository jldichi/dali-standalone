// -------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: grid.h,v 1.1.1.1 1996/04/16 18:51:37 gustavof Exp $
//
// Description:
// -------------------------------------------------------------------------

#ifndef GRID_H
#define GRID_H

#include <ifound/itypes.h>

class GridRep;
class String;
class GridIcon;

class Grid 
{

	GridRep *grep_d;

public:

	enum {
		clickable		= 1,
		fixed_width		= 1 << 1,
		fixed			= 1 << 2,
		hidden			= 1 << 3,
		no_print		= 1 << 4
	};

	Grid(void);
	Grid(const String &header, Int x=10, Int y=5, Int w=80, Int h=25);
	virtual ~Grid(void);

	// 2 phase constructor
	bool create(const String &header, Int x, Int y, Int w, Int h);

	// should return false either if it was not created or it was
	// created with some errors
	operator bool() const;

	bool clickedOn(String& d);
	bool clickedOn(Num& d);
	bool clickedOn(Boolean& d);
	bool clickedOn(Int& d);
	bool clickedOn(Date& d);
	bool clickedOn(Time& d);
	bool clickedOn(GridIcon& d);

	Int idOf(String& d);
	Int idOf(Num& d);
	Int idOf(Boolean& d);
	Int idOf(Int& d);
	Int idOf(Date& d);
	Int idOf(Time& d);
	Int idOf(GridIcon& d);

	// Column creation methods
	Int addCol(String &theString, const String &title, Int width,
				Int flags = 0, const String &mask = NULL_STRING);
	Int addCol(Date &theDate, const String &title, Int width, Int flags = 0, 
				const String &mask = NULL_STRING);
	Int addCol(Time &theTime, const String &title, Int width,
				Int flags = 0, const String &mask = NULL_STRING);
	Int addCol(Boolean &theBoolean, const String &title, Int width,
				Int flags = 0, const String &mask = NULL_STRING);
	Int addCol(Boolean &theBoolean, const String &title,
				const GridIcon &trueIcon /* = defaultTrueIcon */,
				const GridIcon &falseIcon /* = defaultFalseIcon */,
				Int flags = 0);
	Int addCol(Num &theNum, const String &title, Int width,
				Int flags = 0, const String &mask = NULL_STRING);
	Int addCol(Num &theNum, const String &title, Int width, Int flags, 
				Int ndig, Int ndec = 0, bool separ = true,
				Int sign = SIGN_LEADING, char filler = ' ');
	Int addCol(Int &theInt, const String &title, Int width,
				Int flags = 0, const String &mask = NULL_STRING);
	Int addCol(GridIcon &theIcon, const String &title, Int width,
				Int flags = 0,const String& mask=NULL_STRING);

	Int addTypedCol(Int ty, const String &title, Int width, Int flags=0,
					const String &mask = NULL_STRING);
	Int addTypedCol(Int ty, const String &title, Int width, Int flags,
			Int ndig, Int ndec = 0, bool separ = true,
			Int sign = SIGN_LEADING, char filler = ' ');


	void execute();
	void restart();
	void stop();

	virtual bool onNextLine() = 0;
	virtual void onClick(Int row, Int col){}
	virtual bool onRestart();
	virtual void onStop(){}


	// get/set current row for getting values.
	// (normally used from within onClick())
	// For the case of static grids (when user used addCol() to create
	// columns), it copy all data to user objects (Strings, Nums, etc.),
	// so that the user can access them
	Int row() const;
	bool row(Int r);

	// the following are only needed when the user used addTypedCol() to
	// create columns... but they might be used if the user prefers them.

	// get value of current row and specified column
	// (normally used from within onClick())
	const String &operator()(Int colId) const;
	const String &value(Int colId) const;

	// set value of current row and specified column
	// (normally used from within onNextLine())
	void operator()(Int colId, const String &v);
	void value(Int colId, const String &v);

	// get value of specified row and column
	// (normally used from within onClick()... but might also be
	// called from onNextLine()... as long as the accessed row
	// is <= than the current one)
	// this is *not* equivalent to "row(r); value(col);"
	// because the current row is not changed by these calls...
	const String &operator()(Int row, Int colId) const;
	const String &value(Int row, Int colId) const;

};

#endif // GRID_H

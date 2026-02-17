

#ifndef FMPOPUP_H
#define FMPOPUP_H

#pragma interface

#include <ifound.h>

class FmValueObj;

class MenuLines {
public:
	// indexes are from 0
	virtual String GetLine(Int i);
	virtual void Exec(Int i);
};

Int FmPopUpMenu(FmValueObj *obj, Int row, Int nrows, Int ncols,
		const String &label, MenuLines &lines, Int attrib = 0);

#endif

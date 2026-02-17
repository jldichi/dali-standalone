/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: formtype.h,v 1.1.1.1 1996/04/16 18:51:37 gustavof Exp $
*
* DESCRIPTION
* Form enums
*********************************************************************/

#ifndef FORMTYPE_H
#define FORMTYPE_H

class FmField;

struct Fm {

	enum Flag {
		NORMAL		= 0,
		NODISPLAY	= 1 << 0,
		EABORT		= 1 << 1,
		SYMBOLS		= 1 << 2,
		FDUMP		= 1 << 3,
		PRSERVER	= 1 << 4,
		DOCALLBKS	= 1 << 5,
	};

	enum Event {
		add,
		update,
		del,
		ignore,
		read,
		exit,
		read_next,
		read_prev,
		read_first,
		read_last,
	};

	enum Action {
		ok,
		skip,
		redo,
	};
	
	enum Status {
		locked,
		error,
		added,
		used,
		page_down,
		page_up,
		eof,
		init,
		top,
		bottom,
	};

	typedef Action (*Fp)(FmField &f);
};

#endif

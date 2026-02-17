/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: repdict.h,v 1.1.1.1 1996/04/16 18:51:39 gustavof Exp $
*
* DESCRIPTION:
*********************************************************************/

#ifndef REPDICT_H
#define REPDICT_H

/*
	toDo:
	These includes should be removed once the whole library is converted
	to C++.
*/
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/opers.h>

// Report field data

struct RpAccumDict {

	enum {
		sum,
		avg,
		count,
		min,
		max,
		runsum,
		runavg,
		runcount,
		runmin,
		runmax,
	};

	short			acctype;			// accumulator type
	bool			f_noreset;
	Int				argument;			/*
											Function arg. number
											ERROR ==> No argument
										*/
};

struct RpFieldDict {

	bool			f_nullz;  			// flag for null zero
	Int             length;				// Used in char. fields
	short			offset;				// Offset in buffer
	dbfield			vdbfield;			/* 
											Data base field descrip.
											ERROR ==> No DB field
										*/
	value_type		name;				// Field name
};

struct RpZoneExprDict {

	itype rp_itype;						// Internal field type
	bool f_cmp_mask;					// compile mask
	Int f_chk_digit;					// check digit format
	bool f_fillz;	  					// fill with zeros
	UnShort	ndec;						// number of decimal places
	value_type mask;					// field mask
	value_type fmt;						// format for the field
	UnShort exprno;						// expression number

	RpZoneExprDict(int exprNo = ERROR)
	:	rp_itype(ERROR), f_cmp_mask(false),
		f_chk_digit(RP_CDIGIT_UNDEF),
		f_fillz(false), ndec(0),
		exprno(exprNo)
	{ mask.strval = NULL; fmt.strval = NULL }
};

struct RpZoneDict {

	UnShort		lines;					// Lines occupied by this zone
	short		need_lines,				// Lines needed in page to print z.
				at_line;				// Line to write the zone at
	UnInt 		nexprs:16,				// Number of expressions in this zone
				f_eject:1,				// Eject a page after printing zone
				f_before:1, 			// True if must be ejected before
				f_noprint:1;			// Don't print this zone
	value_type	name,					// zone name
				aft_zone;				// Image following field formats
	
	struct condition { 					// Condition data
		Report::When when;				// always, after or before
		Report::WhenType vtype;			// page, field, report
		short nflds;					// number of fields in condition
		value_type field;				// used for after or before field
		value_type ifexpr;				// if expression
	} cond;

};

struct ReportDict {

	short magic;						// rpo magic number
	UnShort nflds,						// Number of fields
			naccums,					// Number of accumulators
			nexprs,						// Number of expressions
			nzones,						// Number of zones
			nschemas,					// Number of schemas used
			buflen,						// Form record buffer length
			valtablen,					// Value table length
			max_lin_aft;				// Maximun lines for after pages
	char ident[32];						// SCCS identification string
	Int		valtab_org,					// Offset in file of value table
			fldtab_org;					// Offset in file of field table
	value_type	module,					// Report module name
				flength,				// Form length (may be env. var.)
				topmarg,				// Top margin (may be env. var.)
				botmarg,				// Bottom margin (may be env. var.)
				leftmarg,				// Left margin (may be env. var.)
				width,					// Width in characters
				inp,					// Input source
				out,					// Output destination
				schemas,				// Tables related
				exprs;					// Expressions used in zones
	UnInt	f_inp_order:1,				// true if input order defined
			f_inp_from:2,				// PRINTER, FILE or PIPE
			f_out_to:4,					// PRINTER, FILE or PIPE
			f_no_ffeed:1,				// true if no use ffeed '\f'
			f_do_eject:1,				// Flag a pending eject page
			pad_1:7,
			pad_2:16;

};

#endif

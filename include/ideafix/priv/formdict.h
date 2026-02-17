/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: formdict.h,v 1.2 2003/06/04 19:15:29 albertoi Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef FORMDICT_H
#define FORMDICT_H

/*
	toDo:
	These includes should be removed once the whole library is converted
	to C++.
*/
#include <cfix.h>
#include <ideafix/priv/gndefs.h>

// Max length of form name
#define MAX_FNAME 64

// MAX length of identification
#define IDENT_LEN	22

#define SIMPLE_FLD(t)	((t) != FormRep::mult && (t) != FormRep::sform && \
						 (t) != FormRep::grouped && (t) != FormRep::ref)
#define VIRTUAL_FLD(d)	((d)->vtype == FormRep::sform || (d)->f_internal)

struct FormDict {

	UnShort magic;				// fmo magic number
	UnShort pad;				// pad for dos
	char name[MAX_FNAME+1],		// form name
		 ident[IDENT_LEN+1];	// SCCS identification string
	UnShort	nkeys,				// Number of keys
			nflds,				// Number of fields
			nrows,				// Number of rows
			ncolumns,			// Number of columns
			nmessages,			// Number of Error messages
			nschemas,			// Number of schemas used
			nformal_args,		// Number of formal arguments
			imglen,				// Form image length
			valtablen,			// Value table length
			nmsgtables;			// Number of Message Tables
	value_type	label,			// Window label
				messages,		/*
									messages string, it has all messages
									separated with ascii-zero.
								*/
				msgtables,		/* 
									usr msgs table, it has all messages
									tables names separated with ascii-zero.
								*/
				schemas,		/*
									schemas string, it has all schemas
									separated with ascii-zero.
								*/
				formal_args;	/*
									formal argument list for
									this form.
								*/
	Int	wborder;				// Border of the window
	Int	background;				// Background color
	short		f_org,			// origin row of the window
				c_org;			// origin column of the window

	UnShort first_fld;			// first field inside the key
	
	// Flags to ignore functions
	BITFIELDSTYPE f_i_add:1,	// flag to ignore add function
				f_i_del:1,		// flag to ignore del function
				f_i_update:1,	// flag to ignore update function
				f_i_ignore:1,	// flag to ignore ignore function
				f_i_end:1,		// flag to ignore end function
				f_i_next:1,		// flag to ignore next function
				f_i_prev:1,		// flag to ignore prev function
				f_i_commands:1,	// flag to ignore commands function

	// Flags to confirm functions
				f_c_add:1,		// flags to confirm add function
				f_c_del:1,		// flags to confirm del function
				f_c_update:1,	// flags to confirm update function
				f_c_ignore:1,	// flags to confirm ignore function
				f_c_end:1,		// flags to confirm end function
				f_full_screen:1,// true if form has full screen win.
				f_no_control:1,	// true if form has not 'control field'
				f_recalcis:1,	// true if there is a 'is field' that has to be updated
				f_autowrite:1,	// Process when form is full
				f_dispstatus:1,	// true if I want to see operation's status
				f_recalcmask:1, // true if there is a mask that has to be updated
				pad_2:13;

	void store(FILE *f);
	bool load(FILE *f);
	Time timeStamp();
	static Time timeStamp(FILE *in);
	static Time timeStamp(const String &ts);
};

struct FmFieldDict {
	/*
		Field type.
		For example FormRep::NUM or FormRep::BOOL.
	*/
	short vtype;
//		FormRep::FldType vtype;

	/*
		Internal field type.  This has to do with C representation
		for the field this is why the two kind of types.
		For example for FM_NUM you can have IT_SHORT, IT_LONG, etc.
	*/
	itype			fm_itype;

	/*
		Now a union because some information changes with the field
		type (In C++ we would have used inheritance).
	*/
	union {
		struct SimpleField {
			/*
				Attribute type for the field (BOLD, UNDERLINE, etc.)
			*/
			attr_type	attr;
			/*
				The data base field this form field is
				related to.  ERR ==> no db field.
			*/
			dbfield vdbfield;

			/* 
				Table and field name if the field belongs to the data base,
				used by the library to correct the table and field 
				descriptors (cracker)
			*/
			value_type table_name;
			value_type field_name;
		} s;
		struct MultipleField {
			/*
				The position in the window for the
				square brackets that the user drew in the .fm file
				this position is maintained because it is used to
				put in this place the scroll arrows.
			*/
			UnShort lpos, lchr, rpos, rchr;
		} m;
		struct SubFormField {
			/*
			* for subforms a flag that indicates
			* if I haven't visited the subform yet.
			* because in this case I must assign the default
			* values.
			*/
			char is_new;		// mark if the subform is empty
		} sf;
		struct ReferenceField {
			fmfield	from;
			fmfield to;			
		} rf;
		void *tmp;
	} u;
	/*
		The data below has sometimes different meanings
		depending on the field type, this is indicated in each field.
	*/
	UnShort			dec,		// Number of decimal places	(for num's)
					row,		// row of field beginning
								// MULT: Rows per page
					column;		/*
									NORMAL FLD:field beginning column
									SUBFORM	: Number of subforms
									GROUPED	: Number of fields.
									MULT	: Number of fields.
								*/
	Int				length,		// field length
								// MULT: Total num. of rows
					olength;	/*
									Output length (some fields can have
									less output lenght than data length)
								*/

	short			nerr_msg,	// Err. msg when check fail
					nhlp_msg,	// Help message number
					ndsc_msg;	// Description message
					
	/*
		Max. relation depth (when using the fld->fld notation
		for description fields in 'in table's)
	*/
	UnShort indepth;

	// Flags
	BITFIELDSTYPE 	f_not_null:1,	// true if null is not accepted	*/
					f_dsp_only:1,	// true if you can not modify it */
					f_skip:1,		// true if the field is skipped on entry */
					f_signed:1,		// true in a numeric field if not unsigned */
					f_comma:1,		/*
										true if in numeric fields you must
										show them formated with commas separating
										the thousands (1,999,999,999.99)
									*/
					/*
						the next three are flags
						to avoid certain operation in multiple fields
					*/
					f_i_add:1,		// Add a new row
					f_i_del:1,		// Delete a row
					f_i_ins:1,		// Insert a row

					f_in_mult:1,	/*
										true if this field is inside
										a multiple one.
									*/
					f_recalcmask:1,	// Recalc mask when modified
					f_manual:1,		// true For manual subforms
					f_in_table:2,	/*
										1 if in table validation
										2 not in table validation
									*/
					f_check_always:1,/*
										Check always not only when
										the field changes
									*/
					f_do_in:1,		/*
										Execute the in table validation
										when the DbToFm procedure is called
									*/
					f_unique:1,		/*
										The field can't be more than
										one time inside a multiple field
									*/
					f_recalcis:1,	// Recalc is when modified
					f_nochk_in:1,	// Don't check IN cond.
					f_auto_enter:1,	// Auto enter
					f_is:1,			// Has an 'IS' expression
					f_alias:1,		/*
										For recursive validations I must
										create a table alias, this flag
										marks that.
									*/
					f_man_help:1,	/*
										The user provides the
										help procedure
									*/
					f_chk_digit:2,	/*
										Check digit validation
										as defined above
									*/
					f_lazy_dflt:1,	/*
										Lazy Default
										default that can not be 
										calculated at the beginning
										of the input (because it references]
										another field),
										and therefore must be delayed
									*/
					f_internal:1,	// Internal field (no image)
					f_arg_type:2,	// Arg. type (in, out, inout)
					NOTUSED1:1,		// it was Default MIN
					NOTUSED2:1,		// it was Default MAX

					f_auto_dup:1,	// field autoduplication 

					f_has_before:1;	// If I have before handler

	BITFIELDSTYPE	f_has_after:1,	// If I have after handler
					f_referenced:1,	// If I'm being referenced
					f_intab_desc:1,	// true if in-table should be descending
					pad_2:29;		// Pad to 32 bits.


	// this type is short instead of Int to maintain 
	// the dictionary length
	short			father,			/*
										Father's number.
										ERR if Adan
									*/
					check_after,	/*
										Field number after which perform
										the validatiom
										ERR if this field
									*/
					next_field,		/*
										Next field (default traversal).
										ERR means physical next.
									*/
					prev_field;		/*
										Prev field (default traversal).
										ERR means physical prev.
									*/

	value_type		defexpr, 		// 'default' compiled expression
					isexpr,			// 'is' compiled expression
					name,			// Field name
					incr,			// Autoincr value
					mask,			// compiled expression mask
					subform,		// subform
					relcheck,		/*
										Relational check compiled
										expression.
									*/
					incheck,		// In Check
					buf,			// Ptr.to rec.buffer
					hasbefore,		// has before compiled expression
					hasafter,		// has after compiled expression
					cmask,			/* buffer that contains :
									   mask + omask + tstmask
									   it is already resolved when mask
									   is a simple expresion
									*/
					onhelp,			// on help expression
					descr,			// descr expression
					onerror;		// on error expression

	void store(FILE *f);
	void load(FILE *f);
};

#endif


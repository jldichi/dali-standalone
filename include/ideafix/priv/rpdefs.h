/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rpdefs.h,v 1.1 1998/07/30 13:29:05 damian Exp $
*
* DESCRIPTION:
* Report general definitions for Library routines & utilities
*
* Files that must be included before this:
* ----------------------------------------
* ideafix.h
* opers.h
*********************************************************************/

#ifndef RPDEFS_H
#define RPDEFS_H

#include <ideafix/priv/gndefs.h>

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

/*++++++++ CONFIG +++++++*/
#define MAX_ZONE_FLDS  256
/*-------- CONFIG -------*/

/* 
		      .rpo file structure
		      ===================

			+-------------------+
			|  report_header    | ---> .rpo file header
			+-------------------+ \
			|     zone data     |  |
			| zone fields       |  <- Variable length array (nflds).
			+-------------------+  | 
			|     zone data     |  | Report zones
			| zone fields       |  <- Variable length array (nflds).
			+-------------------+  | 
			 ...................   | 
								   /
			+-------------------+  \
			|     field data    |  |
			+-------------------+  | 
			|     field data    |  | Report fields
			+-------------------+  | 
			 ...................   | 
								   /
			+-------------------+  \
			| | | | | | | | | | |  | Input field order (nrflds elems)
			+-------------------+  /
			+-------------------+  \
			| Value             |  |
			|        Table      |  |   Value table
			+-------------------+  /
*/	

/* Constants definition										*/
#define RPO_MAGIC 		0411


/* Default values for pages dimentions						*/
#define DEF_LEFTMARG	 0
#define DEF_BOTMARG		 2
#define DEF_TOPMARG		 2
#define DEF_FLENGTH		66
#define DEF_WIDTH		80

/* Field types */
typedef short rp_fld_type;

/* A report real report field */
#define RP_FIELD  	 	 (rp_fld_type) 0 

/* Functions */
#define RP_SUM 		 	 (rp_fld_type) 1
#define RP_AVG 		 	 (rp_fld_type) 2
#define RP_COUNT 		 (rp_fld_type) 3
#define RP_MIN 		 	 (rp_fld_type) 4
#define RP_MAX 		 	 (rp_fld_type) 5
#define RP_RUNSUM 		 (rp_fld_type) 6
#define RP_RUNAVG 		 (rp_fld_type) 7
#define RP_RUNCOUNT 	 (rp_fld_type) 8
#define RP_RUNMIN 		 (rp_fld_type) 9
#define RP_RUNMAX 		 (rp_fld_type) 10
#define RP_DAY 		 	 (rp_fld_type) 11
#define RP_MONTH 		 (rp_fld_type) 12
#define RP_MONTHNAME 	 (rp_fld_type) 13
#define RP_YEAR 		 (rp_fld_type) 14
#define RP_DAYNAME 		 (rp_fld_type) 15
#define RP_DATE 		 (rp_fld_type) 16
#define RP_TIME 		 (rp_fld_type) 17

/* Variables */
#define RP_PAGE 		 (rp_fld_type) 18
#define RP_LINE 		 (rp_fld_type) 19
#define RP_MODULE 		 (rp_fld_type) 20
#define RP_TODAY 		 (rp_fld_type) 21
#define RP_HOUR 		 (rp_fld_type) 22
#define RP_FLENGTH 		 (rp_fld_type) 23
#define RP_BOTMARG 		 (rp_fld_type) 24
#define RP_TOPMARG 		 (rp_fld_type) 25
#define RP_LEFTMARG 	 (rp_fld_type) 26
#define RP_WIDTH 		 (rp_fld_type) 27

/* A constant */
#define RP_CONST 		 (rp_fld_type) 28

/* Check digit codifiers */
#define RP_NO_CDIGIT	0
#define RP_CDIGIT		1
#define RP_CDIGIT_DASH	2
#define RP_CDIGIT_SLASH	3
#define RP_CDIGIT_UNDEF	4

#ifdef HAVE_PRAGMA_ALIGN
#	pragma SC align 4
#endif

/* Report field data */
struct rpfld_dict {
	rp_fld_type		vtype;		/* field type				*/
	itype			rp_itype;	/* Internal field type		*/
	UnInt			f_nullz:1,  /* flag for null zero       */
					f_noreset:1,/* flag to not reset Acc.   */
					f_cmp_mask:1,/* flag to compile mask    */
					f_chk_digit:3,
					/*	check digit codifier (although
						we have only 4 values we need
						3 bits 'cause we need an "undefined"
						value */
					f_fillz:1,  /* flag for fill with zeros */
					length:16,	/* Used in char. fields     */
					f_pad:9;
	short			offset;		/* Offset in buffer      	*/
								/* or function arg. number	*/
								/* ERROR ==> No arg. func.  */
	UShort			ndec;		/* Number of decimal places	*/
	dbfield			vdbfield;	/* Data base field descrip.	*/
								/* ERROR ==> No DB field    */
	value_type		name,		/* Field name				*/
								/* Ptr. to value if RP_CONST*/
					mask;		/* Field mask				*/
};

/* Representation of a field in memory	*/
struct rp_fld {
	rp_fld_type		vtype;		/* field type				*/
	itype			rp_itype;	/* Internal field type		*/
	UnInt			f_nullz:1,  /* flag for null zero       */
					f_noreset:1, /* flag to not reset Acc.   */
					f_chk_digit:2, /* check digit attrib.   */
					ndec:16,	/* Number of decimal places	*/
					pad:12;
	union {
		struct {				/* Data for Real field		*/
			char	*value,		/* Actual value				*/
					*prev_val,	/* Previous value			*/
					*mask;		/* Compiled output mask		*/
			dbfield	vdbfield;	/* Data base field descrip.	*/
								/* ERROR ==> No DB field    */
			UShort  length;		/* Used for char. fields    */
			UShort	f_chg;		/* If field has changed		*/
		} rf;
		struct {				/* Data for virtual field	*/
			struct rp_fld *arg; /* Field arg. to function	*/
			NUM			acc;	/* Function accumulator		*/
			Int		  count;/* Number of times printed  */
		} vf;
		union  {				/* Data for constant field	*/
								/* Can hold any const. type	*/
			char	*C;
			short	S;
			Int	L;
			double	F;
			DATE	D;
			TIME	T;
			NUM   N;
		} cf;
	} u;
};

/* When condition */
typedef short rp_when;
#define ALWAYS (rp_when) 0
#define AFTER  (rp_when) 1
#define BEFORE (rp_when) 2 

/* When condition types */
typedef short rp_cndtp;
#define PAGE   (rp_cndtp) 1
#define FIELD  (rp_cndtp) 2
#define REPORT (rp_cndtp) 4

/* Data of a report zone									*/
struct rp_zones {
	struct rpzone_dict {
		UShort		lines;		/* Lines ocupied by this zone		*/
		short		need_lines,	/* Lines needed in page to print z.	*/
					at_line;	/* Line to write the zone			*/
		UnInt		nflds:16,	/* Number of fields in this zone	*/
					f_eject:1,	/* Eject a page when print zone		*/
					f_before:1, /* True if must be ejected before	*/
					f_noprint:1;/* Don't print this zone			*/
		value_type	name,
					fld_fmts,	/* Format for each field			*/
					aft_zone;	/* Print after field has been prted */

		struct condition { 		/* Data of a condition				*/
			rp_when  when;		/* ALWAYS , AFTER o BEFORE			*/
			rp_cndtp vtype;		/* PAGE | FIELD | REPORT			*/
			value_type field;	/* used for after or before field	*/
			short nflds;		/* number of fields in condition	*/

			short oper1;			/* First operand of IF.        	*/
									/* ERROR ==> inconditional      */
			relop_type relop; 		/* The operator					*/
			short oper2;			/* IF second operand         */
			short pld;				/* Pading */
		} cond;
	}d;
	/*---Follows an array of nflds shorts with the fields numbers---*/
	struct rp_fld	**flds;		/* Array with pointer to fields		*/
};


/* General data of a report			*/
struct s_rep {
	struct rp_dict {
		short magic;		/* rpo magic number				*/
		UShort  nflds,		/* Number of fields				*/
				nrflds,		/* Number of real fields		*/
				nzones,		/* Number of zones				*/
				nschemas,	/* Number of schemas used   	*/
				buflen,		/* Form record buffer length    */
				valtablen,	/* Value table length 			*/
				max_lin_aft;/* Maximun lines for after pages*/
		char ident[32];		/* SCCS identification string	*/
		Int	valtab_org, /* Offset in file of value tab. */
				fldtab_org; /* "   "   "   "   " field  "   */
		value_type	module, /* Report module name			*/
					flength,/* Form length (may be env.var.)*/
					topmarg,/* Top margin    "  "   "   "   */
					botmarg,/* Bottom marg.  "  "   "   "   */
					leftmarg,/* Left margin					*/
					width,  /* Width in characters			*/
					inp, 	/* Input source					*/
					out, 	/* Output destination			*/
					schemas;/* Tables related				*/
		UnInt	f_inp_order:1,/* TRUE if input order defined*/
				f_inp_from:2,/* PRINTER, FILE or PIPE		*/
				f_out_to:2,	/* PRINTER, FILE or PIPE		*/
				f_no_ffeed:1, /* TRUE if no use ffeed '\f'  */
				f_do_eject:1, /* Flag a pending eject page  */
				pad_1:9,
				pad_2:16;
	} d;
	struct rp_zones	*z;		/* Pointer to zones array		*/
	char			*valtab,/* Pointer to value table		*/
					*buf;	/* Pointer to buffer 			*/
	struct rp_fld	*f,		/* Pointer to fields array		*/
					**chkf; /* Fields with conditions		*/
	FILE			*out;	/* Output file descriptor		*/
	UShort			curr_line,/* Line actually in print		*/
					curr_page;/* Page  "    "   "   "       */
	UnInt			lst_p_zone:16,/* Last printed zone		*/
					f_rep_body:1,
					f_started :1,
					f_eabort:1,/* If RP_EABORT used on open  */
					f_bef_report:1; /* Before Report Flag	*/
	DATE today;			/* Set to the current date		*/
	TIME hour;			/* Set to current hour			*/
	UShort *forder;		/* Fields order for input read	*/
	char *tbuff,/* If RP_IO_TERM buffer for OutputColumn (MAX_RP_BUFF)*/
		 *pbuff;/* Idem but dinamic pointer		*/
	Output tout;/* For OpenTermOutput			*/
	bool exit_by_user;	/* In OutputColumn: if user press K_END		*/
};

#ifdef HAVE_PRAGMA_ALIGN
#	pragma SC align __DEFALIGN
#endif

/* Constants										*/
#define MAX_INP_OUT_STRING	256
#define MAX_RP_BUFF			1024

/* ++ Prototypes ++ */

#ifdef __cplusplus
extern "C" {
#endif

extern struct s_rep *_CheckReport(report rpd);

#ifdef __cplusplus
}
#endif

/* -- Prototypes -- */

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif

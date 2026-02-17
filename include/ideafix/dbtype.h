/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbtype.h,v 1.1.1.1 1996/04/16 18:51:37 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef DBTYPE_H
#define DBTYPE_H

#define CURRENT_VERSION		(0L)
#define CURSOR_PAGE_SIZE	(1024L)

enum {
	io_not_lock		=		 0,
	default_lock	=	io_not_lock,
	io_default		=		 0,
	io_lock			=	1 << 0,
	io_rlock		=	1 << 2,
	io_test			=	1 << 3,
	io_notify		=	1 << 4,

	io_eabort		=	1 << 5,
	io_key_fields	=	1 << 6,
	io_symbols		=	1 << 7,
	io_not_renv		=	1 << 8,
	io_all_index	=	1 << 9,
	io_clear_index	=	1 << 10,
	io_verbose		=	1 << 11,
	io_inc_rlock	=	1 << 12,
	io_dec_rlock	=	1 << 13,	
	io_force_open	=	1 << 14,
	io_blocked		=	1 << 15,	
	io_modify		=	1 << 16,
	io_control_break=	1 << 17,

	this_key		=	1 << 18,
	next_key		=	1 << 19,
	prev_key		=	1 << 20,
	first_key		=	1 << 21,
	last_key		=	1 << 22,
	partial_key		=	1 << 23,
	mode_seq		=	1 << 24,
	next_seq		=	next_key|mode_seq,
	prev_seq		=	prev_key|mode_seq,
};

#endif

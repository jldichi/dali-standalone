/*
 * Copyright (c) 1995 InterSoft Co.  All Rights Reserved
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
 * The copyright notice above does not evidence any
 * actual or intended publication of such source code.
 *
 * $Id: fmopname.h,v 1.1.1.1 1996/04/16 18:51:42 gustavof Exp $
 *
 */

#ifndef FMOPNAME_H
#define FMOPNAME_H

struct FmOpt {
	enum Name {
		none,
		label,
		geometry,
		foreground,
		background,
		style,
		unique,
		errmsg,
		hlpmsg,
		descrmsg,
		autowrite,
		dspstatus,
		horizontal,
		vertical,
		boxcolor,
		bordercolor,
		boxmask,
		shadow,
		layout,
		orientation,
		decimal,
		title,
		descr,
		pushedbitmap,
		ignoreinkey,
		ignoreindata,
		confirm,
		resetafter,
		validatebefore,
		body,
		header,
		footer,
		check,
		is,
		ignore,
		execute,
		member,
		defaultopt,
		isopt,
		bitmapfile,
		minopt,
		maxopt,
		not_null,
		length,
		dbfield,
		subform,
		subforms,
		internal,
		in_list,
		in_table,
		displayonly,
		skip,
		skip_when,
		manual_subform,
		manual_help,
		ignore_in_list,
		signedopt,
		use_comma,
		ydelta,
		fmfield,
		schema,
		formal_arg,
		keyzone,
		datazone,
		mask,
		datazone_,
		in_enum,
		not_in_enum
	};
};

#endif


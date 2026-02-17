/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: report.h,v 1.1.1.1 1996/04/16 18:51:37 gustavof Exp $
*
* DESCRIPTION
* Report API
*********************************************************************/

#ifndef REPORT_H
#define REPORT_H

class ReportRep;

class Report {

	ReportRep *rep;

public:

	enum When {
		always,
		after,
		before,
	};

	enum WhenType {
		page	= (1 << 0),
		field	= (1 << 1),
		report	= (1 << 2),
	};

	enum OutputType {
		io_printer,
		io_default = io_printer,
		io_file,
		io_pipe,
		io_term,
		io_report,
		io_graph,
	};

	enum Flag {
		normal		= (1 << 0),
		eabort		= (1 << 1),
		nodb		= (1 << 2),
		copies		= (1 << 3),
		nobegin		= (1 << 4),
		symbols		= (1 << 5),
		option		= (1 << 6),
	};

};

#endif


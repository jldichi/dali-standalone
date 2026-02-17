/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: creprep.h,v 1.3 2003/02/04 15:16:09 albertoi Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef CREPREP_H
#define CREPREP_H

#include <ideafix/priv/reprep.h>

class String;
class CReportRep;
class RpFieldRep;

extern "C" {

	// C interface
	CReportRep *_CheckReport(Int rpd);
	RpFieldRep *_checkField(report rpd, rpfield fn);
	report OpenReport(char *nm, int flgs, ...);
	int CloseReport(form fmd);
	void CloseAllReports(void);
	void RpClearZone(report rpd, int zone);
	int DoReport(report rpd, UShort zone);
	int BeginReport(report rpd, int ncopies, char *fmt);
	int EndReport(report rpd);
	void RpEjectPage(report rp);
	void RpSkipLines(report rp, int nlines);
	void RpDrawZ(report rpd, UChar c);
	void DbToRp(report rpd, rpfield from, rpfield to, ...);
	long RpCheckSum(report rpd);
	void RpVerifyCheckSum(report rpd, long chksum);
	void RpSetOutput(report rpd, rp_output out_to, char *desp);

	void RpSetFld(report rpd, rpfield fn, char *val);
	void RpSetIFld(report rpd, rpfield fn, int val);
	void RpSetLFld(report rpd, rpfield fn, long val);
	void RpSetFFld(report rpd, rpfield fn, double val);
	void RpSetDFld(report rpd, rpfield fn, DATE val);
	void RpSetTFld(report rpd, rpfield fn, TIME val);
	void RpSetNFld(report rpd, rpfield fn, NUM *val);
	long RpLFld(report rpd, rpfield fn);

	int RpFldLen(report rpd, rpfield fn);

	void setCap(char *cap);
	void setPort(char *port);
}

class gtype(PtrArray, RpZoneExpr);

class CReportRep : public ReportRep {

	// C interface
	friend CReportRep *_CheckReport(Int rpd);
	friend form OpenReport(char *nm, int flgs, ...);
	friend int CloseReport(form fmd);
	friend void CloseAllReports(void);
	friend void RpClearZone(report rpd, int zone);
	friend int DoReport(report rpd, UShort zone);
	friend int BeginReport(report rpd, int ncopies, char *fmt);
	friend int EndReport(report rpd);
	friend void RpEjectPage(report rp);
	friend void RpSkipLines(report rp, int nlines);
	friend void RpDrawZ(report rpd, UChar c);
	friend void DbToRp(report rpd, rpfield from, rpfield to, ...);
	friend long RpCheckSum(report rpd);
	friend void RpVerifyCheckSum(report rpd, long chksum);
	friend void RpSetOutput(report rpd, rp_output out_to, char *desp);

	friend void _RpSetNFld(report rpd, rpfield fn, Num *val, const char *func);
	friend long RpLFld(report rpd, rpfield fn);

	friend int RpFldLen(report rpd, rpfield fn);

	// doreport Interface
	friend int doReport(report rpd, char **argv, int ncopies);
	friend int load_rec(report rpd, CReportRep *p, char *rbuf);
	
	static CReportRep *reports[RP_MAX_OPEN];

	Int id;
	
public:

	CReportRep();
	~CReportRep();

	CReportRep *getCReportRep();

	static Int open(const String &name, Report::Flag flags, va_list ap);
	static Int close(Int rpd);
	static void closeAll();
	static CReportRep *getReport(Int rpd);
};

#endif

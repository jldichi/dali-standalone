/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: rp.h,v 1.2 1997/05/28 13:02:21 eduardoc Exp $
*
*
* DESCRIPTION:
*      Report library interface header. 
*
* Files that must be included before this:
* ----------------------------------------
* ideafix.h
*
*
*********************************************************************/

#ifndef RP_H
#define RP_H

/* extern */
STRUCT BaseOutput;
	
struct NUM;

typedef short report;
typedef short rpfield;

/* Special report field descriptors						*/
#define PAGENO	10000
#define LINENO  10001
#define FLENGTH 10002
#define BOTMARG 10003
#define TOPMARG 10004
#define LEFTMARG 10005
#define WIDTH	10006

/* Flags for Openreport	*/
#define RP_NORMAL		0
#define RP_EABORT		1
#define RP_INP_ORDER	2
#define RP_NODB			4
#define RP_COPIES		8
#define RP_NOBEGIN		16
#define RP_SYMBOLS		32
#define RP_OPTION		64

/* Output sub-file character */

#define EOSF	(char) 1		/* End of Sub-file (for remote
									outputs only). */

/* Output destinations */
typedef STRUCT BaseOutput *Output;
                               
typedef short rp_output;
#define RP_IO_DEFAULT	00  /* Output according to $printer */
#define RP_IO_PRINTER	00  /* Output according to $printer */
#define RP_IO_FILE		01
#define RP_IO_PIPE		02
#define RP_IO_TERM      03
#define RP_IO_REPORT	04
#define RP_IO_GRAPH		05	/* Output to graph */

/* Limits				*/
#define RP_MAX_OPEN		8

/* ++ Prototypes ++ */

#ifdef __cplusplus
extern "C" {
#endif

extern void RpClearZone(report rpd, int zone);
extern int DoReport(report rpd, UShort zone);
extern int BeginReport(report rpd, int ncopies, char *fmt);
extern int EndReport(report rpd);
extern void RpEjectPage(report rp);
extern void RpSkipLines(report rp, int nlines);
extern void RpDrawZ(report rpd, UChar c);
extern int CloseReport(report rpd);
extern void CloseAllReports(void);
extern void RpSetFld(report rpd, rpfield fn, char *val);
extern void RpSetIFld(report rpd, rpfield fn, int val);
extern void RpSetLFld(report rpd, rpfield fn, long val);
extern void RpSetFFld(report rpd, rpfield fn, double val);
extern void RpSetDFld(report rpd, rpfield fn, DATE val);
extern void RpSetTFld(report rpd, rpfield fn, TIME val);
extern void RpSetNFld(report rpd, rpfield fn, NUM *val);
extern int RpIFld(report rpd, rpfield fn);
extern long RpLFld(report rpd, rpfield fn);
extern double RpFFld(report rpd, rpfield fn);
extern NUM RpNFld(report rpd, rpfield fn);
extern void DbToRp(report rpd, rpfield from, rpfield to, ...);
extern report OpenReport(char *name, int flags, ...);
extern long RpCheckSum(report rpd);
extern void RpVerifyCheckSum(report rpd, long chksum);
extern void RpSetOutput(report rpd, rp_output out_to, char *desp);
extern Output OpenTermOutput(int f_org, int c_org, int height, int width, attr_type backgr, IFP getcmd);
extern Output OpenRpOutput(char *rpname, int ncopies, char *arg);
extern Output OpenDelimOutput(int dest, char *arg, char *sep, int ncopies);
extern Output OpenFmtOutput(int dest, UChar *arg, UChar *heading, UChar *footing, int fwidth, int flen, int topm, int botm, int leftm, int ncopies);
extern Output OpenRemoteOutput(int dest, char *arg, char *sep);
extern int SetOutputColumn(Output out, char *head, int minwidth, type t, ...);
extern int ColumnWidth(Output out, int n);
extern void SetList(Output out);
extern int OutputColumn(Output out, char *bp);
extern int CloseOutput(Output out);
extern void SetRpOutput(Output out, rp_output out_to, char *dest);
extern Output OpenGraphOutput(int Type,UChar *gtitle,UChar *gstitle,UChar *glabelx,UChar *glabely,bool direc, bool toprinter);
extern void SetGraphOutput(Output out,int gwidth, int glength, int fwidth, int flength);
extern bool IsGraphOutput(Output out);
extern int ColumnNdec(Output out, int n);
extern UShort NColsOutput(Output out);
extern UShort CurrColOutput(Output out);
extern UShort PageWidthOutput(Output out);
extern bool IsNullOutput(Output out);
extern void SetNullOutput(Output out);
extern int RpFldLen(report rpd, rpfield fld);
extern rpfield FindRpField(report rp, const char *fname);

#ifdef __cplusplus
}
#endif

/* -- Prototypes -- */

#endif

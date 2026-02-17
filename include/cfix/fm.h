/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: fm.h,v 1.1.1.1 1996/04/16 18:51:46 gustavof Exp $
*
* DESCRIPTION
* Form library interface header. 
*
* Files that must be included before this:
* ----------------------------------------
* ideafix.h
*********************************************************************/

#ifndef FM_H
#define FM_H

#include <ixwi/wi.h>

typedef short fm_status;

struct NUM;		/* extern */

#define	FM_OK			(fm_status) 0
#define	FM_SKIP			(fm_status) 1
#define	FM_REDO			(fm_status) 2
#define	FM_LOCKED		(fm_status) 3
#define	FM_ERROR		(fm_status) 4
#define	FM_NEW			(fm_status) 5
#define	FM_USED			(fm_status) 6
#define	FM_PAGE_DOWN	(fm_status) 7
#define	FM_PAGE_UP		(fm_status) 8
#define	FM_EOF			(fm_status) 9
#define	FM_INIT			(fm_status) 10
#define	FM_TOP			(fm_status) 11
#define	FM_BOTTOM		(fm_status) 12

typedef short fm_cmd;

#define FM_ADD			(fm_cmd) 0
#define FM_UPDATE		(fm_cmd) 1
#define FM_DELETE		(fm_cmd) 2
#define FM_IGNORE		(fm_cmd) 3
#define FM_READ			(fm_cmd) 4
#define FM_EXIT			(fm_cmd) 5
#define FM_READ_NEXT	(fm_cmd) 6
#define FM_READ_PREV	(fm_cmd) 7
#define FM_READ_FIRST	(fm_cmd) 8
#define FM_READ_LAST	(fm_cmd) 9


typedef short form;
typedef short fmfield;


/* A type for the after and before field functions	*/

/* ++ Prototypes ++ */
typedef fm_status (*fm_status_fp)(form f, fmfield field, int row);
/* -- Prototypes -- */

/* A special field number for the Control form field			*/
#define CONTROL_FLD		(fmfield)9999

/* A tab in a form is always of this length						*/
#define FMTAB	4

/* Flags for Openform											*/
#define FM_NORMAL		0
#define FM_NODISPLAY	1
#define FM_EABORT		2
#define FM_SYMBOLS		4
#define FM_FDUMP		8
#define FM_PRSERVER		16
#define FM_DOCALLBKS	32

/* ++ Prototypes ++ */

#ifdef __cplusplus
extern "C" {
#endif

extern form FmCallBacks(form fmd, fmfield fno, const char *sfname, fm_status_fp before_fp, fm_status_fp after_fp);
extern void FmOnKey(form fmd, Int key, fm_status_fp callBack, fmfield from, fmfield to);
extern void FmClearFlds(form fmd, fmfield from, fmfield to, ...);
extern void FmSetDisplayOnly(form fmd, fmfield from, fmfield to, bool value);
extern void FmClearAllFlds(form fmd);
extern void DisplayForm(form fmd);
extern void ClearForm(form fmd);
extern fm_status FmErrMsg(form fm, int msg, ...);
extern const char *FmMsg(form fm, int msg);
extern void FmRecalc(form fmd, UShort curr_row);
extern void FmRecalcFld(form fmd, fmfield fld, UShort curr_row);
extern form OpenForm(char *name, int flags);
extern long FmCheckSum(form fmd);
extern void FmVerifyCheckSum(form fmd, long chksum);
extern char *FmSFld(form fmd, fmfield fn, ...);
extern bool FmIsNull(form fmd, fmfield fn, ...);
extern fmfield FmInMult(form fmd, fmfield fn);
extern void FmGetFld(char *buf, form fmd, fmfield fn ,...);
extern int FmNextFld(form fmd, fmfield fno, ...);
extern int FmIFld(form fmd, fmfield fn, ...);
extern long FmLFld(form fmd, fmfield fn, ...);
extern double FmFFld(form fmd, fmfield fn, ...);
extern DATE FmDFld(form fmd, fmfield fn, ...);
extern TIME FmTFld(form fmd, fmfield fn, ...);
extern NUM FmNFld(form fmd, fmfield fn, ...);
extern UShort FmInOffset(form fmd);
extern void FmSetInOffset(form fmd, UShort n);
extern char *FmInValue(form fmd, fmfield fn, UShort ndx);
extern char *FmInDescr(form fmd, fmfield fn, UShort ndx);
extern void FmSetFld(form fmd, fmfield fn, char *buf, ...);
extern void FmSetIFld(form fmd, fmfield fn, int val, ...);
extern void FmSetLFld(form fmd, fmfield fn, long val, ...);
extern void FmSetFFld(form fmd, fmfield fn, double val, ...);
extern fmfield FmRefFld(form fmd, fmfield ref, ...);
extern void FmShowFlds(form fmd, fmfield from, fmfield to, ...);
extern void FmShowAllFlds(form fmd);
extern void FmSetStatus(form fmd, fm_status status);
extern void FmSetError(form fm, fmfield fld, ...);
extern fm_status FmStatus(form fmd);
extern form UseSubform(form fmd, fmfield fld, UShort subf_n, ...);
extern fm_cmd DoSubform(form fmd, fm_status_fp precond, fm_status_fp postcond, fmfield fld, UShort subf_n, ...);
extern int FmSubformId(form fmd);
extern form FmFather(form fmd);
extern fmfield FmAssocFld(form fmd);
extern UShort FmSubfRow(form fmd);
extern void FmToDb(form fmd, fmfield from, fmfield to, ...);
extern void FmToTable(form fmd, dbtable tab, ...);
extern void DbToFm(form fmd, fmfield from, fmfield to, ...);
extern void TableToFm(dbtable tab, form fmd, ...);
extern char *FmFldPrev(form fmd);
extern int FmFldLen(form fmd, fmfield fn);
extern int FmNKeys(form fmd);
extern bool FmIsDisplayOnly(form fmd, fmfield fld, ...);
extern bool FmChgFld(form fmd);
extern void FmSetKeyCode(form fmd, UChar code);
extern UChar FmKeyCode(form fmd);
extern fmfield FindFmField(form fmd, const char *s);
extern fm_cmd DoForm(form fmd, fm_status_fp precond, fm_status_fp postcond);
extern void CloseAllForms(void);
extern void CloseForm(form fmd);
extern void FmSetMask(form fm, fmfield fld, char *mask);
extern void FmSetDFld(form fmd, fmfield fn, DATE val, ...);
extern void FmSetTFld(form fmd, fmfield fn, TIME val, ...);
extern void FmSetNFld(form fmd, fmfield fn, NUM* val, ...);
extern form FmFindForm( window wd );
extern fmfield FmFindField( form fmd, int row, int col, int * mult_row );
extern int FmNextFld(form fmd, fmfield fno, ...);
extern int FmNextFldRowCol( form fmd, int row, int col );
extern bool FmIsKeyField( form fmd, fmfield fld );
extern bool FmIsControlField( form fmd, fmfield fld );
extern const char *FmLastMessage();
extern const char *FmErroneous();
extern void FmCheckMods(form fmd, bool rec);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

// routines that are only can be used from C++ programs

void FmSetErrorFld(form fmd, fmfield fld, int rowno = 0);

#endif

/* -- Prototypes -- */

#endif

/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: cformrep.h,v 1.2 2002/02/05 21:07:31 albertoi Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef CFORMREP_H
#define CFORMREP_H

#include <ideafix/priv/formrep.h>

class String;
class OnKeyCallBack;

class NetConnection;
class PrServerConnection;

#define SUBFORM		(-2L)
#define ENDFORM		(ERR)
#define ENDMULT		(-3L)

extern "C" {

	// C interface
	form FmCallBacks(form fmd, fmfield fno, const char *sfname,
			fm_status_fp before_fp, fm_status_fp after_fp);
	form OpenForm(char *nm, int flags);
	void CloseForm(form fmd);
	void CloseAllForms(void);
	fm_cmd DoForm(form fmd, fm_status_fp beforeIx, fm_status_fp afterIx);
	void FmClearFlds(form fmd, fmfield from, fmfield to, ...);
	void FmSetDisplayOnly(form fmd, fmfield from, fmfield to, bool value);
	void FmClearAllFlds(form fmd);
	void DisplayForm(form fmd);
	void ClearForm(form fmd);
	fm_status FmErrMsg(form fm, int m, ...);
	char* FmErrorMsg(form fmd, char* msgid);
	void FmRecalc(form fmd, UShort curr_row);
	long FmCheckSum(form fmd);
	void FmVerifyCheckSum(form fmd, long chksum);
	int FmNextFld(form fmd, fmfield fno, ...);
	void FmShowFlds(form fmd, fmfield from, fmfield to, ...);
	void FmShowAllFlds(form fmd);
	void FmSetStatus(form fmd, fm_status sts);
	void FmSetError(form fmd, fmfield fld, ...);
	fm_status FmStatus(form fmd);
	form UseSubform(form fmd, fmfield fld, UShort subf_n, ...);
	fm_cmd DoSubform(form fmd, fm_status_fp beforeIx,
							fm_status_fp afterIx, fmfield fld, UShort subf_n, ...);
	int FmSubformId(form fmd);
	form FmFather(form fmd);
	fmfield FmAsocFld(form fmd);
	UShort FmSubfRow(form fmd);
	void FmToDb(form fmd, fmfield from, fmfield to, ...);
	void FmToTable(form fmd, dbtable tab, ...);
	void DbToFm(form fmd, fmfield from, fmfield to, ...);
	void TableToFm(dbtable tab, form fmd, ...);
	char *FmFldPrev(form fmd);
	int FmFldLen(form fmd, fmfield fn);
	int FmNKeys(form fmd);
	void FmAbortRead(form fmd);
	bool FmChgFld(form fmd);
	void FmSetKeyCode(form fmd, UChar code);
	UChar FmKeyCode(form fmd);
	fmfield FindFmField(form fmd, const char *s);
	fmfield FmFindField(form fmd, int rowno, int col, int *mult_row);
	bool FmIsKeyField(form fmd, fmfield fld);
	bool FmIsControlField(form fmd, fmfield fld);
	FmFieldRep *_checkField(form fmd, fmfield fn, char *fun);
	char *Detail(FmFieldRep *f, char *funct);
	char *FmSFld(form fmd, fmfield fn, ...);
	bool FmIsNull(form fmd, fmfield fn, ...);
	fmfield FmInMult(form fmd, fmfield fn);
	void FmGetFld(char *buf, form fmd, fmfield fn, ...);
	int FmIFld(form fmd, fmfield fn, ...);
	long FmLFld(form fmd, fmfield fn, ...);
	double FmFFld(form fmd, fmfield fn, ...);
	DATE FmDFld(form fmd, fmfield fn, ...);
	TIME FmTFld(form fmd, fmfield fn, ...);
	NUM FmNFld(form fmd, fmfield fn, ...);
	UShort FmInOffset(form fmd);
	void FmSetInOffset(form fmd, UShort n);
	char *FmInValue(form fmd, fmfield fn, UShort ndx);
	char *FmInDescr(form fmd, fmfield fn, UShort ndx);
	void FmSetFld(form fmd, fmfield fn, char *buf, ...);
	void FmSetIFld(form fmd, fmfield fn, int val, ...);
	void FmSetLFld(form fmd, fmfield fn, long val, ...);
	void FmSetFFld(form fmd, fmfield fn, double val, ...);
	void FmSetDFld(form fmd, fmfield fn, DATE val, ...);
	void FmSetTFld(form fmd, fmfield fn, TIME val, ...);
	void FmSetNFld(form fmd, fmfield fn, NUM *val, ...);
	void FmSetMask(form fm, fmfield fld, char *mask);
	form FmFindForm(window wd);
	bool FmIsSubForm(form fmd);
	void _setLFld(FmFieldRep *f, char *msg, long val, Int rowno);
	CFormRep *_CheckForm(Int fmd);
	bool  _ValidForm(Int fmd);
	int FmNKeys(form fmd);
	void FmOnKey(form fmd, Int key, fm_status_fp callBack, fmfield from, fmfield to);
}

//albertoi: Y esto ??
//void FmFieldRep::setFld(const char *buf, Int rowno);
//const char *FmFieldRep::getFld(char *funct, Int rowno);

class CFormRep : public FormRep {

	// C interface
	friend form FmCallBacks(form fmd, fmfield fno, const char *sfname,
			fm_status_fp before_fp, fm_status_fp after_fp);
	friend form OpenForm(char *nm, int flags);
	friend void CloseForm(form fmd);
	friend void CloseAllForms(void);
	friend fm_cmd DoForm(form fmd, fm_status_fp beforeIx,
							 fm_status_fp afterIx);
	friend void FmClearFlds(form fmd, fmfield from, fmfield to, ...);
	friend void FmSetDisplayOnly(form fmd, fmfield from, fmfield to,
									 bool value);
	friend void FmClearAllFlds(form fmd);
	friend void DisplayForm(form fmd);
	friend void ClearForm(form fmd);
	friend fm_status FmErrMsg(form fm, int m, ...);
	friend void FmRecalc(form fmd, UShort curr_row);
	friend long FmCheckSum(form fmd);
	friend void FmVerifyCheckSum(form fmd, long chksum);
	friend int FmNextFld(form fmd, fmfield fno, ...);
	friend void FmShowFlds(form fmd, fmfield from, fmfield to, ...);
	friend void FmShowAllFlds(form fmd);
	friend char* FmErrorMsg(form fmd, char* msgid);
	friend void FmSetStatus(form fmd, fm_status sts);
	friend void FmSetError(form fmd, fmfield fld, ...);
	friend fm_status FmStatus(form fmd);
	friend form UseSubform(form fmd, fmfield fld, UShort subf_n, ...);
	friend fm_cmd DoSubform(form fmd, fm_status_fp beforeIx,
							fm_status_fp afterIx, fmfield fld, UShort subf_n, ...);
	friend int FmSubformId(form fmd);
	friend form FmFather(form fmd);
	friend UShort FmSubfRow(form fmd);
	friend void FmToDb(form fmd, fmfield from, fmfield to, ...);
	friend void FmToTable(form fmd, dbtable tab, ...);
	friend void DbToFm(form fmd, fmfield from, fmfield to, ...);
	friend void TableToFm(dbtable tab, form fmd, ...);
	friend char *FmFldPrev(form fmd);
	friend int FmFldLen(form fmd, fmfield fn);
	friend int FmNKeys(form fmd);
	friend void FmAbortRead(form fmd);
	friend bool FmChgFld(form fmd);
	friend void FmSetKeyCode(form fmd, UChar code);
	friend UChar FmKeyCode(form fmd);
	friend fmfield FindFmField(form fmd, const char *s);
	friend fmfield FmFindField(form fmd, int rowno, int col, int *mult_row);
	friend bool FmIsKeyField(form fmd, fmfield fld);
	friend bool FmIsControlField(form fmd, fmfield fld);
	friend FmFieldRep *_checkField(form fmd, fmfield fn, char *fun);
	friend char *Detail(FmFieldRep *f, char *funct);
	friend void FmFieldRep::setFld(const char *buf, Int rowno);
	friend const char *FmFieldRep::getFld(char *funct, Int rowno);
	friend char *FmSFld(form fmd, fmfield fn, ...);
	friend bool FmIsNull(form fmd, fmfield fn, ...);
	friend fmfield FmInMult(form fmd, fmfield fn);
	friend void FmGetFld(char *buf, form fmd, fmfield fn, ...);
	friend int FmIFld(form fmd, fmfield fn, ...);
	friend long FmLFld(form fmd, fmfield fn, ...);
	friend double FmFFld(form fmd, fmfield fn, ...);
	friend DATE FmDFld(form fmd, fmfield fn, ...);
	friend TIME FmTFld(form fmd, fmfield fn, ...);
	friend NUM FmNFld(form fmd, fmfield fn, ...);
	friend UShort FmInOffset(form fmd);
	friend void FmSetInOffset(form fmd, UShort n);
	friend char *FmInValue(form fmd, fmfield fn, UShort ndx);
	friend char *FmInDescr(form fmd, fmfield fn, UShort ndx);
	friend void FmSetFld(form fmd, fmfield fn, char *buf, ...);
	friend void FmSetIFld(form fmd, fmfield fn, int val, ...);
	friend void FmSetLFld(form fmd, fmfield fn, long val, ...);
	friend void FmSetFFld(form fmd, fmfield fn, double val, ...);
	friend void FmSetDFld(form fmd, fmfield fn, DATE val, ...);
	friend void FmSetTFld(form fmd, fmfield fn, TIME val, ...);
	friend void FmSetNFld(form fmd, fmfield fn, NUM *val, ...);
	friend void FmSetMask(form fm, fmfield fld, char *mask);
	friend void _setLFld(FmFieldRep *f, char *msg, long val, Int rowno);
	friend form FmFindForm(window wd);
	friend bool FmIsSubForm(form fmd);
	friend CFormRep *_CheckForm(Int fmd);

	// gencf interface
	friend void gencf(char *Path, char *FileName);
	friend void NumberSubf(FILE *out, CFormRep *fp, int nivel);
	friend void AllFields(CFormRep *fp);
	friend void CheckFields(CFormRep *pp, dbindex idx,
						   int nivel);
	friend void AllSubforms(FILE *out, CFormRep *fp);
	friend void PreYPos(FILE *out, char nom[20], int cual,
							   CFormRep *fp);
	friend void Abm(FILE *out, CFormRep *fp);
	friend void UpDate(FILE *out, CFormRep *fp);
	friend dbindex ChooseKey(dbtable xtab);
	friend int BuscaMulti(int match, dbindex idx);

	// imail interface
	friend void SetMode(form fm, char op);

	// dbdes interface
	friend char *AnyToStr(form fm, fmfield fld, char *what);
	friend void SetFmFieldType(form fm, fmfield fld,
			FormRep::FldType ty, int len, int ndec);

	// menu interface
	friend char *execform(char *form_name, char *action, int options);

	// fdump interface
	friend void showFormSize(int &argc, char **&argv);

	// doform interface
	friend void extractValues(form fmd);
	friend dbtable ChooseTab(CFormRep *fp);
	friend void CheckFields(CFormRep *pp, dbindex idx);
	friend void PassToFm(form fm);
	friend void PassToDb(form fm);
	friend FmFieldRep *_check_field(form fm, fmfield fn);

	// execform interface
	friend int execForm(form fmd, char *procedure, int exmode,
						int pnrows, int pncols, bool restart);

	friend Fm::Action before(FmField &fld);
	friend Fm::Action after(FmField &fld);
	friend CFormRep *FmFieldRep::setCallBacks(const char *sfname,
									fm_status_fp before_fp,
									fm_status_fp after_fp);

	static CFormRep *forms[FM_MAX_OPEN];

	CFormRep *adan();
	void assignCallBacks(fm_status_fp beforeIx, fm_status_fp afterIx);
	Int id;
	fm_status_fp beforeFunc;
	fm_status_fp afterFunc;
	OnKeyCallBack *onKeyCallBacks;

	friend class IdeApp;

public:

	CFormRep();
	~CFormRep();

	FormRep *createSubForm(const String &name, int subf_id,
						   Fm::Flag flags, FormRep *father,
						   FmFieldRep *srcFld, schema *first_sch);

	Fm::Event execSubForm(FormRep *sf, Int fldno, Int subf_n, Int row);
	CFormRep *getCFormRep();

	bool isSubForm(Int &fid, Int &fld, Int &subf_n, Int &r);

	fm_cmd remExecForm();
	fm_cmd remExecSubForm(bool haveBefore, bool haveAfter,
						  Int fmd, Int fld, Int subf_n, Int rowno);
	fm_cmd remExecute();

	void sendKey(NetConnection &);
	void sendData(NetConnection &);
	void sendFields(NetConnection &, Int from, Int to, Int rowno = 0);
	void sendAllFields(NetConnection &conn);
	void receiveAllFields(NetConnection &conn);
	void _sendFields(NetConnection &, Int from, Int to,
					 Int rowno = 0, Int to_rowno = ERR);
	void receiveFields(NetConnection &conn);
	void sendStamps(PrServerConnection &conn);
	void _sendStamps(PrServerConnection &conn);

	Int fmId() 		{ idCheck(); return id; }
	CFormRep *parent();


	static Int open(const String &name, Fm::Flag flags);
	static void close(Int fmd);
	static void closeAll(void);
	static CFormRep *getForm(Int fmd);
	void setOnKeyCallBack(Int fkey, fm_status_fp fp,
						  fmfield to = ERR, fmfield from = ERR);
	fm_status invokeOnKeyCallBack(Int fkey, Int fldno, Int row = 0);
	bool haveOnKeyCallBack(Int fkey, Int fldNo);
	static bool valid(Int fmd);
	void sendMsgTables(PrServerConnection &conn);
};

typedef UnChar (*fpRemoteHelpInTable)(FmFieldRep *, char *, UnShort);
typedef bool (*fpRemoteInTableValidate)(FmFieldRep *, UnShort);
typedef void (*fpRemoteUseSubform)(FormRep *, Int, Int, Int);
typedef void (*fpRemoteInsMult)(FormRep *, FmFieldRep *, Int);
typedef void (*fpRemoteDelMult)(FormRep *, FmFieldRep *, Int);

extern void setRemoteHelpInTable(fpRemoteHelpInTable fp);
extern void setRemoteInTableValidate(fpRemoteInTableValidate fp);
extern void setRemoteUseSubform(fpRemoteUseSubform fp);
extern void setRemoteInsMult(fpRemoteInsMult fp);
extern void setRemoteDelMult(fpRemoteDelMult fp);

#endif

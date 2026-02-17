/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: formrep.h,v 1.6 2010/07/02 21:32:45 leonardol Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef FORMREP_H
#define FORMREP_H

#include <ideafix/formtype.h>
#include <ideafix/priv/formdict.h>
#include <ixwi/wi.h>

class FormExpr;
class FmFieldRep;
class CFormRep;
class FormMsgTable;
class MsgTable;
class Time;

class gtype(PtrArray, FmFieldRep);
class gtype(PtrArray, FormMsgTable);
class gtype(Array, String);

class NetConnection;

enum StatusMsg {
	DS_NONE,
	DS_ADD,
	DS_UPDATE,
	DS_PROCESS_ADD,
	DS_PROCESS_UPDATE,
	DS_PROCESS_DELETE,
	DS_PROCESS
};

enum CLRMODE { CLRSTD, CLRAUTOATTR};

// Maximun number of values in an IN condition
#define MAX_IN_VALS		64

// Number of command keys in the control field
#define FM_NUM_CMD_KEYS  8

// How many levels you can nest grouped fields
#define MAX_GROUP_DEPTH 16

// Field Overhead one for the '\0' and one for mark
// if the field was modified

#define FLD_OVERHEAD	2

#define FM_COMP_EXT	".fmo"

struct sform_info {
	UnChar subfNo_;
	UnChar isMod_;
	long  nModFlds_;
	UnChar firstTime_;
	UnChar dummy1_;
	short  dummy2_;
};

struct s_rvalue {
	rvaltype vtype;
	union {
		struct db_field {
			dbfield	vdbfield;
		    const char *table_name;
		    const char *fld_name;
		} db;
		struct fm_field {
			fmfield	vfmfield;
		} fm;
		struct const_field {
			char	Const[1];	/* The real size depends on the const 0.*/
			char	*cvalue;
		} c;
	} v;
};

struct dsp_fld {
	dbfield dbfld;
	const char *table_name;
	const char *fld_name;
};

class FormRep : public Resource {

	useIdCheck(FormRep_id);

	friend class FmFieldRep;

public:

	// message table
	static MsgTable *fmsg;

	FormDict d;						// form dictionary

	UnInt buflen;					// Form record buffer length
	Fm::Status	st;					/*
										current form status,
										see Fm::Status type in fm.h
									*/
	UnShort		act_field;			// current field number
	bool		overlap;			// true if higher than std window
	Fm::Fp pre, post;				// ptrs to before/after functions
	bool usingDadCallBacks;			/*
										if I called setCallBacks I don't have
										to inherit the pre and post from 
										my dad
									*/
	// field array
	gtype(PtrArray, FmFieldRep) &flds;

	char		*valtab;			// Value table org. (need to free)
	char		*img;				// form image
	UnChar		fkey;				/*
										last pressed function key,
										returned by FmKeyCode
									*/
	char		*prev_val;			// previous value of current input field
	window		wd;					// window's form descriptor
	short		subform;			// subform identification
	char		**msgs;				/*
										user defined messages,
										it's built from 'd.messages'
									*/
	short		selected_in;		// index of 'in list' selection
	FormRep		*father;			/*
										father form pointer or, 
										NULL when Adan
									*/
	FmFieldRep	*srcFld;			/*
										field where the subform is or NULL
										if it's not a subform
									*/
	UnShort		row;				/*
										if I'm subform this is the
										multiline row number of my father.
									*/
	UnShort		display_row;		/*
										it tells which was the father's row
										used to execute the display form.
									*/
	bool		changed;			/*
										true if current field changed
										its value
									*/
	bool		errflag;			// error mark, set by FmSetError
	int			next_field;			/*
										If ERR, the form
										follows the normal sequence,
										otherwise it is the next field to
										be processed.  Set by FmNextFld().
									*/
	UnShort		next_row;			/*
										Same as next_field, indicating row
										for multiple fields.
										Also set by FmNextFld().
									*/
	bool status_;					// status when the form was opened

	Int nModFlds_;					// number of fields that were changed
	
	bool values_sent;				// if have been sent some fields
	
	gtype(PtrArray, FormMsgTable) &msgtables;// user message tables

	static UnShort formWindows;
	static window helpWindow;

	FormRep *adan();
	void createFlds();
	void defaultValues(UnShort nflds, UnShort desp = 0);
	void statusLabel(StatusMsg msgno);
	int _nextFld(Int fno, int row);
	void init();
	void initValues();
	bool open(const String &name, int subf_id, Fm::Flag flags, 
			  FormRep *father, FmFieldRep *srcFld, schema *first_sch);
	char *physOpen(const String &name, schema *first_sch, Fm::Flag flags);
	void convert_offsets();
	void multDefault(Int firstFld, UShort columns, UShort row);
	void setAutoAttribs(Int firstFld, UShort columns, UShort row);
	void multIs(Int firstFld, UShort columns, UShort row);
	UnChar testEndIgnDel(UnChar cmd);
	bool chkNull(Int from, Int to);
	int buildKeyOpt(struct key_opt *k);
	UnChar helpKey();
	FmFieldRep *checkField(Int fn, char *fun);
	void _toDb(dbtable tab, Int from, Int to,
			char *funct, UnShort rowno);
	void _fromDb(dbtable tab, Int from, Int to,
			char *funct, UnShort rowno, bool disp);
	void getSchemas(schema *first_sch, Fm::Flag flags);
	Int validateNextField();
	FmFieldRep *findField(Int row, Int col, Int &rowp);
	void incModFlds();
	void decModFlds();
	bool isDisplayed();

	void prepareAutomaticSubforms(Int li, Int ls);
	void resetAllAutomaticSubforms(Int li, Int ls);
	void prepareAutomaticSubforms(Int rectorNo, Int firstFld, UnShort columns, 
								  UnShort rowno);

	FmFieldRep *subformField();

public:

	enum ArgType {
		arg_none	= 0,
		arg_in		= 1 << 0,
		arg_out		= 1 << 1
	};

private:


public:

	enum FldType {
		alpha		= 0,
		num			= 1,
		date		= 2,
		time		= 3,
		mult		= 4,
		sform		= 5,
		grouped		= 6,
		flt			= 7,
		boolean		= 8,
		ref			= 9,
	};
#if defined (ACADEMIC_VERSION)
	enum { MAGIC = 0606 };
#else
	enum { MAGIC = 0635 };
#endif
	// Constructors & destructors
	FormRep();
	virtual ~FormRep();

	void openAndAlloc(const String &name, Fm::Flag flags);

	bool ok()			{ idCheck(); return status_; } 

	Fm::Event execute(Fm::Fp precond = NULL, 
					  Fm::Fp postcond = NULL);

	void display();
	void undisplay();

	bool confirm(unsigned flag, const String &func);

	int nextFld(Int fno, int rowno = 0);
	Int findField(int row, int col, int *rowp);
	bool isKeyField(Int fld);
	bool isControlField(Int fld);
	void showFlds(Int from = 0, Int to = INT_MAX, Int rowno = 0);
	void toDb(Int from, Int to, int rowno = 0);
	void toTable(dbtable tab, int rowno = 0);
	void fromDb(Int from, Int to, int rowno = 0);
	void fromTable(dbtable tab, int rowno = 0);
	UnShort inOffset();
	void setInOffset(UnShort n);
	FmFieldRep *findField(const char *s);
	void status(Fm::Status st);
	Fm::Status status() const;
	void setError(FmFieldRep *f, ...);
	void setError(FmFieldRep *f, va_list ap);
	void setError(FmFieldRep *f, Int rowno, const String &msg);
	void setErrorFld(FmFieldRep *f, Int rowno = 0);
	void setDisplayOnly(Int from, Int to, bool value);
	const char *getMsg(int nromsg);
	Fm::Status errMsg(int message, ...);
	Fm::Status errMsg(int message, va_list ap);
	void recalc(Int currRow = 0);
	String name() const { idCheck(); return d.name; }

	void clearFlds(Int from = 0, Int to = INT_MAX, Int rowno = 0,
				   bool setMode = false, bool clearStd = true, Int mrow = 0);

	void _clearFlds(Int from = 0, Int to = INT_MAX, 
					Int rowno = 0, CLRMODE m = CLRSTD);
	Int checkSum();
	void verifyCheckSum(Int chksum);

	FmFieldRep *field(Int fno);

	void resolveArgs(FmFieldRep *caller_fld, int n, int row, 
						  ArgType argtype, bool onlyCheck);

	// messager methods
	static String msg(const char *mindex);
	static String msg(const String &mindex);
	static char *charPtrMsg(const char *m);
	static char *charPtrMsg(const String &m);
	static const char *lastMessage;	
	static char erroneousForm[MAX_FNAME+1];	
	static void setErrMsg(const char *, const String &nm);

	virtual FormRep *createSubForm(const String &name, int subf_id, 
								   Fm::Flag flags, FormRep *father, 
								   FmFieldRep *srcFld, schema *first_sch);

	virtual Fm::Event execSubForm(FormRep *sf, Int, Int, Int);
	
	virtual CFormRep *getCFormRep(void);

	static char *nextExpression(char *exprbuff, FormExpr **expr);

	Int nModFlds();
	bool isMod();
	
	void checkMods(bool rec);
	Int countModFlds(Int from, Int to, Int rowno, bool rec);
	
   	Fm::Action invokeOnKeyCallBack(Int fkey, Int fldno, Int row = 0);  
	bool haveOnKeyCallBack(Int fkey, Int fldNo);

	UnShort parRow()			{ idCheck(); return row; 		}

	bool currFldChanged()		{ idCheck(); return changed;	}

	Time timeStamp();
	static Time timeStamp(const String &name);

	String getMsg(const String &tab, const String &sec, const String &msg);
};

class FmFieldRep : public Resource {

	useIdCheck(FmFieldRep_id);

	friend class FormRep;
	friend class FormExpr;
#ifdef DECLARE_fmText	
	friend char *fmText(char *buffer, int n);
#endif
#ifdef DECLARE_fmInTable	
	friend char *fmInTable(char *buffer, int n);
#endif
// private:

public:

	FmFieldDict d;						// field dictionary
                  
	FormRep *fp;  						// Pointer to the form

	Int fldno;							// Offset in form's array

	Int cskip;							/*
											Conditional Skip: true if
											this field has an is expression
											and his current value is void.
										*/
	FormExpr *defexpr;					/*
											pointer to compiled 'default'
											expression,
											it's built with d.defexpr.
										*/
	FormExpr *isexpr;						/*
											pointer to compiled
											'is' expression,
											it's built with d.isexpr.
										*/	
	FormExpr *relcheck;					/*
											pointer to compiled relational
											check expression,
											it's built with d.relcheck.
										*/
	struct Relop {
		Int			op;					/*
											in list options
											(OP_IN, OP_NOT_IN)
										*/
		UnShort		nvals;				// number of values in 'values'
		char		**values;			// in list values
		char		**descr;			// description's values
	} ilcheck;							/*
											in list check structure, it's
											built with d.incheck, if the
											f_in_table flags is zero.
										*/
	struct s_t_check {

		dbindex ind;					// in table access index
		const char *table_name;			// table name that index belongs to
		const char *ind_name;			// name of the index

		UnShort n_display;				/*
											number of display fields that
											you want to see in the 'in table'
										*/
		struct dsp_fld *dsp_flds;		// displays fields array
		UnShort n_keys;					/*
											number of key fields that you want 
											to set in the 'in table'
										*/
		struct s_rvalue *kflds;			// key fields values array
	} incheck;							/*
											in table check structure, it's
											built with d.incheck, if the
											f_in_tables is one or two.
										*/
	union {
		struct _SimpleField {
			FmFieldRep		*mult;		/*
											if I have f_in_mult flag turn on,
											this is my multiple field.
										*/
			FormRep			**subf;		// array with subforms
		} s;
		struct _MultipleField {
			FmFieldRep		*rector;	/*
											rector field for a multiple
											or grouped field.
										*/
			UnShort			curr_row;	// current row in multiple field.
			UnShort			first_row;	// first row in multiple field.
		} m;
	} u;

	FmFieldRep *refFld;					// field that is my reference
    Int        size_;
    
	FormExpr *hasbefore;				/* expression that tells me 
										   if I have to execute the before 
										   routine
										 */
	FormExpr *hasafter;					/* expression that tells me 
										   if I have to execute the after
										   routine
										 */

	FormExpr *mask;						/* expression that defines the field
										   mask
										 */
	bool maskWasAllocated;				/* shows if the mask was or wasn't
										   allocated
										 */  

	FormExpr *onerror;					// on error expression 
	
	FormExpr *onhelp;					// on help expression
	
	FormExpr *descr;					// descr expression
	
	char *value(char *buf);
	bool defaultValue(int row);
	void buildDefExpr();
	void buildIsExpr();
	void buildRelCheck();
	void buildInCheck();
	void buildHasBefore();
	void buildHasAfter();
	void buildMask();
	void buildOnError();
	void buildOnHelp();
	void buildDescr();
	UnChar inpMult();
	bool leftMovement();
	bool rightMovement();
	UnChar inputFld(UnShort row, UnShort row_inpage, bool force_not_null);
	void setAutoAttribs(Int row);
	bool haveToSkip(Int row, const char * pv = NULL);
	bool isDisplayOnly(Int row);
	bool isNotNull(Int row = 0);
	bool hasBefore(UnShort row);
	bool hasAfter(UnShort row);
	UnChar fieldInput(register char *fbuf, UnShort row, UnShort row_inpage);
	bool validate(const char *value, UnShort row, bool do_check_after,
				  bool dbValidate);
	bool validate(char *fbuf, UnShort row, bool dbValidate, 
				  bool force_not_null, bool &f_recalc);
	bool nullValue(Int rowno);
	int chkMultNull();
	bool groupNull(UnShort row);
	bool checkAfter(UnShort n);
	bool chkUnique(UnShort row, UnShort nfields);
	UnChar helpEnum(char *fbuf, UnShort row);
	UnChar helpInTable(char *fbuf, UnShort row);
	char *getInTable(int row, bool first);
	bool inTableValidate_(UnShort row);
	bool inTableValidate(UnShort row);
	bool inListValidate(char *value, UnShort row);
	bool relOpValidate(char *val, UnShort row);
	bool checkRecord(UnShort row, dbindex ind, int nk);
	void setInTable(UnShort row, dbcursor curs=ERR, k_field_flags ifflags=0);
	void displayStdHelp(bool overlap);
	void displayNotNull(Int rowno = 0);
	void displayMsg(const char *value, Int rowno);
	void recalc(Int curr_row = 0, bool all_rows = false);
	void recalcIs(Int rowno);
	void recalcMask(Int rowno);
	const char *getFld(char *funct="", Int n = 0);
	char *getForSetFld(char *funct="", Int n = 0);
	void getCompiledMask(char *&tstmask, char *&omask, Int rowno);

	void correctDbField(schema sch);
	void correctInCheck(schema sch);

	void show(Int rowno = 0);

	sform_info *sformInfo(int rowno = 0);
	UnChar &subfNo(int rowno = 0);
	UnChar &firstTime(int rowno=0);
	bool isMod(Int rowno = 0);
	bool resetMod(Int rowno = 0);
	bool setMod(Int rowno = 0);

	void setFld(const char *v, Int n);
	bool isNull(int rowno = 0);
	char *inValue(UnShort ndx);
	char *inDescr(UnShort ndx);
	void setMask(const String &mask);
	char *detail(char *funct);
	void _showFlds(Int nflds, Int rowno = 0);
	void putArrows(UnShort first);
	char *fmtField(const char *val, Int rowno);
	int fldLen();
	bool inpGroup(Int fldno, UnShort row, UnShort row_inpage, 
					bool f_not_null);

	UnShort multLastRow();
	void multSetFirstRow(UnShort first_row);
	void multSetRow(UnShort &_row);
	void multSetRowFirst(UnShort &_row, UnShort first_row);

	char *subformInfo(int n, char **name, UnShort *nargs);
	static char *subformInfo(char *strval, int n, char **name, UnShort *nargs);
	static int cmp(itype t, register char *value, register char *operand);
	bool isControlFld();
	bool isSimpleFld(); 
	bool isValueFld();
	bool isMultFld();
	bool isGroupedFld();
	Int valid(Int row = 0);
	char * _autoIncr(Int row);
	void insMult(Int row);
	bool delMult(Int row);
	FmFieldRep *ref(Int rowno = 0);
	bool helpMsg(String &msg, Int row);

	void resetModFlds(Int rowno = 0);
	void sendSubformFields(NetConnection &conn, Int rowno);

	CFormRep *setCallBacks(const char *sfname, fm_status_fp before_fp,
						   fm_status_fp after_fp);

private:

	FmFieldRep *getLastNonSkipGroupField(UnShort row);
	Int nextNonRefFld();
	Int nextNonSkip(Int fromFld = ERR);
	void restorePrevVals(char *prev_val, gtype(Array, String) &prevVals, 
						 Int nextFld, Int lastFld, Int row);

public:

	enum CheckDigitType {
		no_cdigit,
		cdigit,
		cdigit_dash,
		cdigit_slash,
	};

	FmFieldRep();
	~FmFieldRep();
	FormRep *useSubform(UnShort subf_n, int rowno = 0);
	Fm::Event doSubform(Fm::Fp precond, Fm::Fp postcond,
				 	    UnShort subf_n, int rowno = 0,
				 	    bool manual = true, bool invoke_always = true);

	FormRep *getSubform(Int subf_n);

	Int length()	
		{ idCheck(); return d.length;									}
	Int rows()		
		{ idCheck(); assert(d.vtype == FormRep::mult); return d.length;	}
	Int id()	
		{ idCheck(); return fldno;										}
	
	String name()
		{ idCheck(); return d.name.strval;								}

	bool isInMult()
		{ idCheck(); return d.f_in_mult;								}
		
	FormRep *form()
		{ idCheck(); return fp;											}
		
	void size(Int s);
	Int  size();

	Fm::Action locked();
	Fm::Action eof();

};

#if 0
Form cbtes("cbtes");
Form cbtes("cbtes", Fm::NODISPLAY);

cbtes.clearFields(from, to);
cbtes.clearFields();

cbtes.display();
cbtes.undisplay();
cbtes.recalc();

cbtes(TCOMP) = 10;
cbtes(TCOMP, 10) = 10;
cbtes(TCOMP)[10] = 10;

FmField aux = cbtes(TCOMP);
aux = 20;
aux[20] = 10;

cbtes(TCOMP) == NULL_VALUE
cbtes(TCOMP).isNull()

cbtes(TCOMP).displayOnly();
cbtes(TCOMP).inMult();

cbtes.lastInOffset();
cbtes(TCOMP).inValue(n);
cbtes(TCOMP).inDescr(n);

cbtes(TCOMP).show();
cbtes.showFields(from, to);
cbtes.showFields();

cbtes.status(status);
cbtes.status();
cbtes.error(msg, field, row, ...);

cbtes.errorMsg(msg, ...);

Form subf(cbtes, APLIC, row);        // Use Subform
cbtes.useSubForm(APLIC, row);

cbtes.doSubForm(APLIC, row);

cbtes.id();							// SubForm Id
cbtes.father();
cbtes.fatherRow();					// Row in father form

cbtes.toDataBase(from, to);
cbtes.toDatabase();
cbtes.fromDatabase();

cbtes.prevField();
cbtes.fieldChange();


cbtes(TCOMP).length();

cbtes.event(kcode);
cbtes.event();

cbtes.execute();					// DoForm

cbtes(TCOMP).setMask(mask);

#endif
#endif

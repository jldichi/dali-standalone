/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: reprep.h,v 1.7 2003/02/04 15:21:45 albertoi Exp $
*
* DESCRIPTION:
*********************************************************************/

#ifndef REPREP_H
#define REPREP_H

#ifdef __GNUC__
#	pragma interface
#endif

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

// Constants
#define MAX_INP_OUT_STRING	256
#define MAX_RP_BUFF			1024

// Magic number definition

#if defined (ACADEMIC_VERSION)
#define    RPO_MAGIC    0607
#else
#define RPO_MAGIC 		0307
#endif

// Default values for page dimensions
#define DEF_LEFTMARG	 0
#define DEF_BOTMARG		 2
#define DEF_TOPMARG		 2
#define DEF_FLENGTH		66
#define DEF_WIDTH		80

/* Check digit codifiers */
#define RP_NO_CDIGIT	0
#define RP_CDIGIT		1
#define RP_CDIGIT_DASH	2
#define RP_CDIGIT_SLASH	3
#define RP_CDIGIT_UNDEF	4

#ifdef HAVE_PRAGMA_ALIGN
#	pragma SC align 4
#endif

#include <ifound/anyvalue.h>

class BaseOutput;
class AnyValue;
class MsgTable;

class ReportRep;
class RpZoneRep;
class RpFieldRep;
class RpAccumulator;
class ReportExpr;
class RpZoneObject;
class RpZoneExpr;
class RpZoneLabel;

class gtype(PtrArray, String);
class gtype(PtrArray, RpFieldRep);
class gtype(PtrArray, RpAccumulator);
class gtype(PtrArray, ReportExpr);
class gtype(PtrArray, RpZoneRep);
class gtype(PtrArray, RpZoneObject);

class RpAccumulator {

	useIdCheck(RpAccumulator_id);

	friend class ReportRep;
	friend class RpFieldRep;
	friend class RpZoneRep;

	// Start of dictionary
	Int			type_;			// Accumulator type

	/*
		accumulator argument, using the following format

		byte1 byte2 byte3 byte4
		\_________/ \_________/
		  zoneNo       exprNo
	*/
	Int argNo_;

	bool		noReset_; 		// flag to not reset Acc.
	// End of dictionary

	RpZoneRep	*zone_;			// Pointer to zone representation
	Int			accNo_;			// accumulator descriptor
	Int			count_;			// Number of times printed
	Num			acc_;			// Function accumulator

	// Resolved argument
	RpZoneExpr *arg_;			// Accumulator's expression argument

	void initValues();

public:

	enum {
		sum,
		avg,
		count,
		min,
		max
	};

	RpAccumulator(RpZoneRep *zr, Int type = ERROR, bool noReset = false,
			RpZoneExpr *ze = NULL);
	~RpAccumulator();

	Int accNo() const { return accNo_; }
	Int type() const { return type_; }

	bool load(FILE *);
	void store(FILE *);

	void dump(FILE *);

	ReportRep *reportRep();

	Num value();
	void accumulate(const Num &val);
};

class RpFieldRep {

	useIdCheck(RpFieldRep_id);

	friend class ReportRep;
	friend class RpZoneRep;

// private:
public:

	// Start of dictionary
	Int			type_;			// Type (TY_STRING, TY_DATE, etc.)
	Int			len_;			// Length
	Int			nDec_;			// Number of decimal places
	String		name_ ;			// Field name
	Int			vDbfield_;		// Data base field descrip.
								// ERROR ==> No DB field
	// toDo: nullZ_ is not used... we leave it for the moment to
	// avoid incompatibilities with previous versions
	bool		nullZ_;			// flag for null zero

	// End of dictionary

	ReportRep	*rp_;			// Pointer to report representation
	AnyValue	value_;			// Actual value
	AnyValue	prevVal_;		// Previous value
	bool		changed_;		// If field has changed
	Int			fldNo_;			// field descriptor

	void initValues();

public:

	RpFieldRep(ReportRep *r, const String &nm = NULL_STRING);
	~RpFieldRep();

	void setNull();
	const String &name() const { return name_ ? name_ : NULL_STRING; }
	Int length() const { return len_; }
	void length(Int l) { len_ = l; }
	Int nDec() const { return nDec_; }
	void nDec(Int nd) { nDec_ = nd; }
	Int type() const { return type_; }
	void type(Int t) { type_ = t; }
	Int dbField() const	{ return vDbfield_; }
	void dbField(Int dbf) { vDbfield_ = dbf; }
	Int fldNo() const { return fldNo_; }
	void set(const char *);
	
	bool load(FILE *);
	void store(FILE *);

	void dump(FILE *);

	AnyValue value();
	AnyValue prevVal();
};

class RpZoneRep {

	useIdCheck(RpZoneRep_id);

	friend class RpFieldRep;
	friend class ReportRep;

	Int			lines_;					// Lines ocupied by this zone
	Int			needLines_;				// Lines needed in page to print z.
	Int			atLine_;				// Line to write the zone
	Int 		nExprs_;				// Number of expressions in this zone
	bool		eject_;					// Eject a page when print zone
	bool		before_;	 			// True if must be ejected before
	bool		noPrint_;				// Don't print this zone
	String		name_;					// zone name
	unsigned char befAftCond_:1,		// before() or after() used in 'if' expression
				  resetAccum_:1,        // clear accumulators if the 'if' clause fails.
				  notused:6;            // Not used yet.
	Report::When when_;					// always, after or before
	Report::WhenType type_;				// page, field, report
	gtype(PtrArray, RpFieldRep) &flds_;	// used for after or before field
	ReportExpr &ifexpr_;				// if expression
	gtype(PtrArray, RpAccumulator) &accs_;	// Pointers to accumulators
	gtype(PtrArray, RpZoneObject) &zobjs_;	// Array with report expressions

	ReportRep	*rp_;						// report representation
	Int			zoneNo_;					// zone descriptor

	void initValues();

public:

	RpZoneRep(ReportRep *r);
	RpZoneRep(ReportRep *r, const String &str);
	~RpZoneRep();

	void print(int valmode);
	void clear();
	bool isAlways();
	Int zoneNo();
	Int nAccumulators() const;
	RpAccumulator *accumulator(Int n);
	Int accumNo(Int atype, bool noReset, const String &argName);
	Int addAccumulator(Int atype, bool noReset, const String &argName);
	void accumulate(RpZoneExpr *zexpr, const Num &a);
	void accumulate(RpFieldRep *fr, const Num &a);
	void resetAccumulators();

	// Used for rgen utility
	// Should find nth expression (skipping labels)
	RpZoneExpr *findExpr(Int n);
	RpZoneExpr *findExpr(const String &name);
	RpZoneExpr *findField(const String &name);
	Int insert(RpZoneObject *, RpZoneObject *);
	void nLines(Int n)					{ lines_ = n; }
	Int nLines() const					{ return lines_; }
	String name() const					{ return name_; }
	// Should count number of expressions in zobjs (skipping labels)
	Int nExprs();
	void needLines(Int n)				{ needLines_ = n; }
	Int needLines() const				{ return needLines_; }
	void addCondField(RpFieldRep *fr);
	Int add(RpZoneObject *obj);
	void noPrint(bool f)				{ noPrint_ = f; }
	bool noPrint() const				{ return noPrint_; }
	void eject(bool f)					{ eject_ = f; }
	bool eject() const					{ return eject_; }
	void before(bool f)					{ before_ = f; }
	bool before() const					{ return before_; }
	Report::When condWhen() const		{ return when_; }
	void condWhen(Report::When w)		{ when_ = w; }
	Report::WhenType condType() const	{ return type_; }
	void condType(Report::WhenType wt)	{ type_ = wt; }
	ReportExpr &ifExpr()				{ return ifexpr_; }
	void atLine(Int n)					{ atLine_ = n; }
	Int atLine() const					{ return atLine_; }
	ReportRep *reportRep()				{ return rp_; }
	Int nObjs() const;
	RpZoneObject *object(Int i);
	void befAftCond()					{ befAftCond_ = true; }
	bool automatic();

	bool load(FILE *);
	void store(FILE *);

	void dump(FILE *);

	void setResetAccumulators() 		{ resetAccum_ = true; }
	bool hasResetAccumulators()			{ return resetAccum_; }
};

class RpZoneObject {

	friend class RpZoneRep;

	useIdCheck(RpZoneObject_id);

protected:

	enum {
		expression,
		label,
	};

	// some time in the near future...
	// Coord xcoord;
	// Coord ycoord;
	RpZoneRep *zone_;

public:

	static RpZoneObject *create(RpZoneRep *r, FILE *in);

	RpZoneObject(RpZoneRep *zr);
	virtual ~RpZoneObject();

	ReportRep *reportRep() 					{ return zone_->reportRep(); }
	virtual RpZoneExpr *getRpZoneExpr()		{ return NULL; }
	virtual RpZoneLabel *getRpZoneLabel()	{ return NULL; }

	virtual bool load(FILE *) = 0;
	virtual void store(FILE *) = 0;

	virtual void print(bool noprint, int valmode) = 0;

	virtual void dump(FILE *) = 0;
	RpZoneRep *zone() { return zone_; }
};

class RpZoneExpr : public RpZoneObject {

	friend class RpZoneRep;

	// Start of dictionary
	Int			type_;
	Int 		chkDigit_;		// check digit attrib.
	Int 		nDec_;
	ReportExpr	&maske_;		// mask
	bool		fillZeros_;		// fill with zeros

	bool		nullZeros_;		// null zeros

	bool		thousandSep_;	// thousands separator
	bool		showSign_;		// represent negative numbers
	bool		doRound_;		// round to the number of decimals
	Int			length_;		// length
	ReportExpr	&expr_;			// expression
	// End of dictionary

	String		mask_;			// compiled mask
	Int			exprNo_;		// expression number
	RpFieldRep	*field_;		// lvalue field or null

	void initValues();

public:

	RpZoneExpr(RpZoneRep *z);
	RpZoneExpr(RpZoneRep *z, const String &exprStr);
	~RpZoneExpr();

	// Used for rgen utility
	Int type() const				{ return type_; }
	void type(Int t)				{ type_ = t; }
	Int nDec() const				{ return nDec_; }
	void nDec(Int n)				{ nDec_ = n; }
	Int length() const				{ return length_; }
	void length(Int l)				{ length_ = l; }
	RpZoneExpr *getRpZoneExpr()		{ return this; }
	Int checkDigit()				{ return chkDigit_; }
	void checkDigit(Int cd)			{ chkDigit_ = cd; }
	void fillZeros(bool v)			{ fillZeros_ = v; }
	bool fillZeros() const			{ return fillZeros_; }
	ReportExpr &maskExpr()			{ return maske_; }
	void nullZeros(bool v)			{ nullZeros_ = v; }
	String descr();
	ReportExpr &expr()				{ return expr_; }
	RpFieldRep *field()				{ return field_; }
	void showSign(bool v)			{ showSign_ = v; }
	bool showSign() const			{ return showSign_; }
	void doRound(bool v)			{ doRound_ = v; }
	bool doRound() const			{ return doRound_; }
	void thousandSep(bool v)		{ thousandSep_ = v; }
	bool thousandSep() const		{ return thousandSep_; }
	Int exprNo() const				{ return exprNo_; }

	bool load(FILE *);
	void store(FILE *);

	void dump(FILE *);

	void print(bool noprint, int valmode);
};

class RpZoneLabel : public RpZoneObject {

	String label_;

public:

	RpZoneLabel(RpZoneRep *z)
	: RpZoneObject(z)
	{ } 
	RpZoneLabel(RpZoneRep *z, const String &l)
	: RpZoneObject(z), label_(l)
	{ }

	RpZoneLabel *getRpZoneLabel() { return this; }

	bool load(FILE *);
	void store(FILE *);

	void dump(FILE *);

	void print(bool noprint, int valmode);
};

class ReportRep : public Resource {

	useIdCheck(ReportRep_id);

	friend class RpFieldRep;
	friend class RpZoneRep;
	friend class RpZoneExpr;
	friend class RpAccumulator;
	friend class RpZoneLabel;

#ifdef DECLARE_rpRepSpSub  
	friend int rpRepSpSub(int c, ReportRep *p);
#endif

protected:

	Int		maxLinAft_;						// Maximun lines for after pages
	String	module_;						// Report module name
	ReportExpr &flengthe_;					// Form length 
	ReportExpr &topmarge_;					// Top margin 
	ReportExpr &botmarge_;					// Bottom margin
	ReportExpr &leftmarge_;					// Left margin
	ReportExpr &widthe_;					// Width in characters
	ReportExpr &inpe_;						// Input source
	ReportExpr &outpute_;					// Output destination
	Int		inpFrom_;						// PRINTER, FILE or PIPE
	Int		outTo_;							// PRINTER, FILE or PIPE
	bool	noFFeed_;						// true if no use ffeed '\f'
	bool	doEject_;						// Flag a pending eject page
	gtype(PtrArray, String) &schemas_;		// Related schemas
	gtype(PtrArray, RpZoneRep) &zones_;		// Pointer to zones array
	gtype(PtrArray, RpFieldRep) &flds_;		// Pointer to fields
	gtype(PtrArray, RpFieldRep) &forder_;	// Fields order for input read

	FILE			*out_;				// Output file descriptor
	Int				currLine_;			// Line actually in print
	Int				currPage_;			// Page  "    "   "   "
	Int				lstPZone_;			// Last printed zone
	bool			repBody_;
	bool			started_;
	bool			eabort_;			// If RP_EABORT used on open
	bool			befReport_; 		// Before Report Flag
	char 			*tbuff_;			/* 
											If RP_IO_TERM buffer for 
											OutputColumn (MAX_RP_BUFF)
										*/
	char			*pbuff_;			// Idem but dinamic pointer
	void *		    tout_;				// For OpenTermOutput
	bool			exitByUser_;		// In OutputColumn: if user press K_END		*/
	bool			status_;
	bool			ejectingPage;		// true if I am ejecting a page

	Int				flength_;			// Form length 
	Int				topmarg_;			// Top margin 
	Int				botmarg_;			// Bottom margin
	Int				leftmarg_;			// Left margin
	Int				width_;				// Width in characters
	String			inp_;				// Input source
	String			output_;			// Output destination

	String			out_pm_wif_;		// Output destination for WIF
	static String	cap_;				// Name cap to process out_pm_wif_
	static String	port_;				// Parallel port to print out_pm_wif_
	
	Time			beginTime_;			// Time when the report was opened
	Report::When	when_;
	Report::WhenType whenType_;

	// message table
	static MsgTable *rmsg_;

	void printCondition(Report::When when, Report::WhenType t, 
			bool use_prev);
	int printf(const String &fmt, ...);
	void copyPrevious();
	void setChangeFlag(bool flag);
	void setChangedFields();
	void setNull();
	bool evalIf(Int zone);
	void initValues();

	Int outputTermChar(char c);

public:

	ReportRep(const String &nm, Report::Flag flags, Int ncopies = 1, 
			  const String &option = NULL_STRING);
	ReportRep();
	~ReportRep();

	// Methods to use in SetWIFProperties
	static void setCap(char *cap);
	static void setPort(char *port);

	void open(const String &nm, Report::Flag flags, Int ncopies,
			  const String &option);

	int begin(Int ncopies, const String &fmt);
	int execute(UnShort zone);
	int end();

	void clearZone(int zone);
	void ejectPage(bool use_prev = false);
	void skipLines(Int nlines);
	void drawZ(UnChar c);
	void fromDb(Int from, Int to, Int rowno = 0);
	Int checkSum();
	void verifyCheckSum(Int chksum);
	void setOutput(Report::OutputType out_to, const String &desp);
	void accumulate(RpZoneExpr *zexpr, const Num &a);
	void accumulate(RpFieldRep *f, const Num &a);

	bool ok();
	// RpAtomExpr *atomExpr(Int aeno);

	// messager methods
	static String msg(const char *mindex);
	static String msg(const String &mindex);

	// Used by rgen utility
	Int nSchemas() const;
	Int indOfSchema(const String &n) const;
	Int addSchema(const String &n);
	RpZoneExpr *findExpr(const String &name);
	RpZoneExpr *findField(const String &name);
	Int add(RpZoneRep *z);
	Int add(RpFieldRep *fr);
	RpFieldRep *field(const String &fName);
	const String &schemaName(Int i) const;
	Int fieldNo(const String &fName);
	Int variableNo(const String &vName);
	AnyValue varValue(Int varNo);
	RpFieldRep *field(Int n);
	RpFieldRep *orderField(Int n);
	Int nextField(RpFieldRep *fld);
	void clearOrderFlds();
	Int nOrderFlds() const;
	RpZoneRep *zone(const String &zName);
	RpZoneRep *zone(Int n);
	Int nZones() const;
	const String &name() const		{ return module_; }
	void name(const String &n)		{ module_ = n; }
	ReportExpr &inExpr()			{ return inpe_; }
	void inputFrom(Int v)			{ inpFrom_ = v; }
	ReportExpr &topMargExpr()		{ return topmarge_; }
	ReportExpr &botMargExpr()		{ return botmarge_; }
	ReportExpr &leftMargExpr()		{ return leftmarge_;}
	ReportExpr &widthExpr()			{ return widthe_;}
	ReportExpr &fLengthExpr()		{ return flengthe_;	}
	void noFormFeed(bool v)			{ noFFeed_ = v; }
	ReportExpr &outExpr()			{ return outpute_;	}
	void outputTo(Int v)			{ outTo_ = v; }
	void maxLinesAfter(Int n)		{ maxLinAft_ = n; }
	Int maxLinesAfter() const		{ return maxLinAft_; }
	Int nFields() const;
	void width(Int v)				{ width_ = v; }
	void leftMarg(Int v)			{ leftmarg_ = v; }
	Int fLength() const				{ return flength_; }
	void fLength(Int v)				{ flength_ = v; }
	Int topMarg() const				{ return topmarg_; }
	void topMarg(Int v)				{ topmarg_ = v; }
	Int botMarg() const				{ return botmarg_; }
	void botMarg(Int v)				{ botmarg_ = v; }
	const String &output() const	{ return output_;  }
	const Time &beginTime() const	{ return beginTime_; }

	// Used to evaluate expressions
	bool beforeReport();
	bool beforePage();
	bool beforeField(Int fldno);
	bool afterReport();
	bool afterPage();
	bool afterField(Int fldno);
		
	bool load(const String &nm, Report::Flag flags);
	bool load(FILE *);
	void store(FILE *);

	void dump(FILE *);

	ReportRep &operator<<(const String &out);
};

#ifdef HAVE_PRAGMA_ALIGN
#	pragma SC align __DEFALIGN
#endif

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif

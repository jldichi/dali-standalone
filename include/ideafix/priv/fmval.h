/*
 * $Id: fmval.h,v 1.1.1.1 1996/04/16 18:51:40 gustavof Exp $
 */

#ifndef FMVAL_H
#define FMVAL_H

#pragma interface

#include <ideafix/priv/fmbase.h>

class FmMultiLine;
class FmSubform;
class FmCheck;
class InListCheck;
class InTableCheck;

class FmValueObj : public FmBaseField {
	typedef FmBaseField inherited;

	friend class FmCheck;
	
	/*
		Internal field type.  This has to do with C representation
		for the field this is why the two kind of types.
		For example for FM_NUM you can have IT_SHORT, IT_LONG, etc.
	*/
	Int iType_;

	Int inheritDbField_;
	String dbTableName_;
	Int dbField_;				// The data base field this form field is
								// related to.  ERR ==> no db field.

	bool needRecalcIs_;				// Form's 'IS' need to recalc when modified
	bool needRecalcMask_;			// Form's 'MASK' need to recalc when modified
	bool manualHelp_;				// The user provides the help procedure
	bool manualSform_;				// subform will be opened by user
	bool lazyDefault_;				/*
										Lazy Default
										default that can not be
										calculated at the beginning
										of the input (because it references
										another field),
										and therefore must be delayed
									*/
									
	bool defltMin_;					// Default MIN
	bool defltMax_;					// Default MAX
	
	bool ignoreInList_;
	bool ignoreInTable_;
	
	bool dbValidate_;				// we should to validate inTable
	bool callUserAfter_;			// we should to call user's after
	
	Char argType_;					// Arg. type (in, out, inout)

	FmExpr *hlpMsg_;				// Help message
	FmExpr *descrMsg_;				// Description message
	
	FmExpr *defExpr_;				// 'default' expression
	FmExpr *maskExpr_;				// 'mask' expression

	Int disabled_;					// 'disable' flags: see DisableReason

	Int dataSize_;

	FmMultiLine *multi_;
	String sformName_;
	FmSubform *sform_;

protected:
	InListCheck		*inList;
	InTableCheck	*inTable;

private:
	void initValues();

	void updateItemValue(Int row);

	inline char *getBufferForSet(FmBufferRow br) const;
	inline String getBuffer(FmBufferRow br) const;
	void setBuffer(FmBufferRow br, const String &val);

	char &modifiedFlag(char *data) const;

	bool validateUnique(Int row);
    bool validateInList(Int row);
    bool validateInTable(Int row);

protected:
	// item related functions
	virtual wxItem *new_wxItem(wxPanel *panel,
			int x, int y, int w, int h, long style, char *name);
	virtual void setValue(wxItem *item, const String &value);
	virtual String getValue(wxItem *item) const;
	virtual void setSkip(wxItem *item, bool skip);
	virtual void setReadOnly(wxItem *item, bool readOnly);

	void updateDisable();

	// type related functions
	void iType(Int itype);
	void setNumIType(Int len);
	virtual String formatValue(const String &v) const;
	virtual String packValue(const String &v) const;

	inline String getBuffer(Int row) const;

	void dataSize(Int dataSize);

	String typeHelpMsg() const;
	virtual String posHelpMsg() const;
	
public:
	FmValueObj();
	~FmValueObj();
	
	FM_DECLARE_ABSTRACT_DATA_CLASS();

	FmValueObj *getValueObj();
	
	virtual Int bufferSize() const;

	virtual void _realize(); // to set readOnly

	virtual void _onConnectBuffer();
	virtual void _onDisconnectBuffer();

	// set & get the current value
	virtual void _value(const String &theValue, FmBufferRow br);
	virtual String _value(FmBufferRow br) const;

	virtual bool inheritFrom(Int dbfld);

	void rectorOf(FmMultiLine *multi);
	inline bool isRector() const;
	inline FmMultiLine *multi() const;

	bool modified(Int row) const;
	void modified(Int row, bool flag);

    virtual bool validate(Int row);
    
	// set & get the manual subform attribute
	void manualSform(bool v);
	inline bool manualSform() const;

	// set & get the manual help attribute
	void manualHelp(bool v);
	inline bool manualHelp() const;

	// set & get internal field type
	inline Int iType()	const;

	// set & get the data base field
	void dbField(Int dbfld);
	inline Int dbField() const;

	// display field's help message
	virtual void onHelp(wxKeyEvent &);

	String helpMsg(Int row);
	String defaultHelpMsg() const;
	
	String descrMsg(Int row);

	FormRep *useSubform(Int index, Int row = 0);

	// toDo: is this method necessary?
	// fm_cmd doSubform(Int index, Int row = 0);

	void sformName(const String &sformName);
	inline const String &sformName() const;
	void sform(FmSubform *theSform);
	inline FmSubform *sform() const;

	virtual void _clear(FmBufferRow br);

	enum DisableReason {
		dueIsAlwaysSkip,
		dueInKeyZone,
		dueInMultiLine,
		dueInForm
	};

	void enable(DisableReason,bool enable = true);
	inline bool enabled() const;

	// set & get recalc when modified
	inline void needRecalcIs(bool v);
	inline bool needRecalcIs() const;
	inline void needRecalcMask(bool v);
	inline bool needRecalcMask() const;

	void defExpr(const String &expr);
	void setMask(const String &expr);
	void maskExpr(const String &expr);
	FmExpr *defExpr() const;
	FmExpr *maskExpr() const;
	bool hasDefault() const;
	bool hasMask() const;

	void recalcIs(Int row);
	void recalcMask();

	void recalc(Int row);
	void recalcIsAll();
	void recalcAll();
	
	void ignoreInList(bool v);
	inline bool ignoreInList() const;

	virtual bool _isNull(FmBufferRow br) const;

	bool changed() const;

	inline Int dataSize() const;

	// get object's associated item(s)

	virtual bool canHaveFocus() const;
	bool canBeCurrent() const;

	virtual void savePrevValue(Int row);
	virtual bool assignDefault(Int row);
	virtual void showDescription(Int row);
	virtual void saveSkipValues(Int row);
	virtual void restoreSkipValues(Int row);

	virtual void toggleSubform();
	virtual void closeSubform(bool ok);
	virtual bool openSubform(Int row, Int subf_index,
				bool manual, bool invoke_always);
	
	void updateFirstRow();

	void onChangeValue(Int row);	// should be called on any change of
									// wxItem's value w/o changing buffer

	inline bool haveInTable() const;

	// add & query values
	inline bool haveInList() const;
	Int numInLists() const;
	String inListValue(Int index) const;
	
	void addInListValue(const String &value, const String &descr = NULL_STRING);

	Int inListIndex(Int row);

	void clearField();	// to call from action
    void onSetFocus(Int row);
};

/////////////////////////////////////////////////////////////////////
//                  deneral data fields: Fm*Field
/////////////////////////////////////////////////////////////////////

class FmCharField : public FmValueObj {
	typedef FmValueObj inherited;

	String testMask_;
	String outputMask_;
	
protected:
	virtual String posHelpMsg() const;
	virtual String formatValue(const String &v) const;
	virtual String packValue(const String &v) const;

public:
	FmCharField();
	
	FM_DECLARE_CLASS();

	virtual bool inheritFrom(Int dbfld);

	virtual FieldType fType() const;

	inline Int length() const;
	void length(Int len);
	
	inline const String &testMask() const;
	inline const String &outputMask() const;
};

class FmNumField : public FmValueObj {
	typedef FmValueObj inherited;

	Int decimal_;
	bool signed_;
	bool useComma_;

protected:
	virtual String posHelpMsg() const;
	virtual String formatValue(const String &v) const;
	virtual String packValue(const String &v) const;

public:
	FmNumField();

	FM_DECLARE_CLASS();

	virtual bool inheritFrom(Int dbfld);

	virtual FieldType fType() const;

	void digits(Int digits, Int decimal = 0);
	inline Int digits() const;
	inline Int decimal() const;

	void setSigned(bool v = true);
	inline bool isSigned() const;

	void useComma(bool v = true);
	inline bool usingComma() const;
};

class FmFloatField : public FmValueObj {
	typedef FmValueObj inherited;

protected:
	virtual String posHelpMsg() const;
	virtual String formatValue(const String &v) const;
	virtual String packValue(const String &v) const;

public:
	FmFloatField();
	
	FM_DECLARE_CLASS();

	virtual bool inheritFrom(Int dbfld);

	virtual FieldType fType() const;

	void length(Int length);
	inline Int length() const;
};

class FmBoolField : public FmValueObj {
	typedef FmValueObj inherited;

	static String yes_;
	static String no_;

protected:
	virtual String formatValue(const String &v) const;
	virtual String packValue(const String &v) const;

public:
	FmBoolField();
	
	FM_DECLARE_CLASS();

	virtual bool inheritFrom(Int dbfld);

	virtual FieldType fType() const;
	
	static inline const String &yes();
	static inline const String &no();
};

class FmDateTimeField : public FmValueObj {
	typedef FmValueObj inherited;

protected:
	virtual char delimiter() const = 0;
	virtual String formatValue(const String &v) const;
	virtual String packValue(const String &v) const;

public:
	FmDateTimeField();
};

class FmDateField : public FmDateTimeField {
	typedef FmDateTimeField inherited;

protected:
	virtual char delimiter() const;

public:
	FmDateField();
	
	FM_DECLARE_CLASS();

	virtual bool inheritFrom(Int dbfld);

	virtual FieldType fType() const;

	void digits(Int digits);
	inline Int digits() const;
};

class FmTimeField : public FmDateTimeField {
	typedef FmDateTimeField inherited;

protected:
	virtual char delimiter() const;

public:
	FmTimeField();
	
	FM_DECLARE_CLASS();

	virtual bool inheritFrom(Int dbfld);

	virtual FieldType fType() const;

	void digits(Int digits);
	inline Int digits() const;
};

#define DECLARE_FmField(wxFieldClass)									\
	typedef wxFieldClass wxField;										\
	static wxItem *item(wxFieldClass *field);							\
	static wxField *field(wxItem *item);								\
	virtual wxItem *new_wxItem(wxPanel *panel,							\
			int x, int y, int w, int h, long style, char *name);		\
	virtual void setSkip(wxItem *item, bool skip);						\
	virtual void setReadOnly(wxItem *item, bool readonly);				\
	virtual void setValue(wxItem *item, const String &value);			\
	virtual String getValue(wxItem *item) const;						\
	virtual bool dontProcessKey(wxKeyEvent &ev);						\
	virtual bool canHaveFocus() const;									\
	virtual bool internal() const

#define IMPLEMENT_FmField(FmFieldClass)									\
	inline wxItem *FmFieldClass::item(FmFieldClass::wxField *field)		\
	{																	\
		return field;													\
	}																	\
	inline FmFieldClass::wxField *FmFieldClass::field(wxItem *item)		\
	{																	\
		return (wxField*)item;											\
	}																	\
	bool FmFieldClass::internal() const									\
	{																	\
		return false;													\
	}

#include <ideafix/priv/fmval.icc>

#endif

/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: fmobj.h,v 1.1.1.1 1996/04/16 18:51:41 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef FMOBJ_H
#define FMOBJ_H

#pragma interface

#include <ideafix.h>
#include <ideafix/priv/fmbuff.h>
#include <ideafix/priv/fmopname.h>

class FmObject;
class FmFieldObj;
class FmValueObj;
class FmObjCollection;
class FmObjContainer;
class FmMultiLine;
class FormRep;
class Form;
class FmExpr;
class FmOption;
class OptionValue;
class ColorVal;

class wxItem;
class wxColour;
class wxKeyEvent;

class gtype(PtrArray, FmOption);
class gtype(PtrArray, FmObject);

void DUMP(const FmObject *obj);
void DUMP(Int i);

// *************************************************
// these methods should be redefined in EVERY class
// *************************************************
#define FM_DECLARE_ABSTRACT_CLASS()						\

#define FM_DECLARE_ABSTRACT_DATA_CLASS()				\
	virtual bool load(FILE *in);						\
	virtual void store(FILE *out);						\
	virtual bool addOption_(const FmOption &option);	\

#define FM_DECLARE_CLASS()								\
	virtual String type() const;						\
	virtual Int classId() const;						\
	static const Int classId_;							\
	virtual bool load(FILE *in);						\
	virtual void store(FILE *out);						\
	virtual bool addOption_(const FmOption &option);	\
	
class FmObject : public Resource {
	FmObject(const FmObject&);	   // just declare this in private section
	const FmObject &operator=(const FmObject&);		// to prevent copying.

	String name_;					// this object's name

	FormRep *fm_;					// Pointer to the form
	FmObjCollection *parent_;		// Pointer to parent object
	FmObjContainer *container_;		// Container this object is managed by

	gtype(PtrArray, FmOption)		&options_;

	Int style_;
	ColorVal *background_;
	ColorVal *foreground_;
	Int geom_X_, geom_Y_, geom_W_, geom_H_;

	void initValues();
	
public:
	FmObject();
	virtual ~FmObject();

	FM_DECLARE_ABSTRACT_DATA_CLASS();
	virtual String type() const;
	virtual Int classId() const;
	
	// **********************************
	// methods for interaction with forms
	// **********************************

	void formRep(FormRep *theForm);
	inline FormRep *formRep() const;
	
	// ***********************
	// methods for fields tree
	// ***********************

	// set & get object's parent
	void parent(FmObjCollection *theObject, Int position = ERR);
	inline FmObjCollection *parent() const;

	// This method should be redefined by container objects
	virtual Int nObjects() const;
	virtual FmObject *object(Int num) const;
	virtual Int objectNum(FmObject *obj) const;
	virtual void add(FmObject *theObject, Int position = ERR);
	virtual void del(FmObject *theObject);
	bool isParentOf(FmObject *theObject) const;

	// *****************************
	// methods to mantain multilines
	// *****************************

	// set & get object's container
	inline FmObjContainer *container() const;

	inline Int numItems() const;
	inline bool inMult() const;
	inline Int deltaY() const;
	inline Int _itemToRow(Int itemNum) const;	// private
	inline Int _rowToItem(Int rowNum) const;	// private
	inline Int firstRow() const;

	// ********************************
	// methods to change object's state
	// ********************************

	// interface methods for container
	inline bool realized() const;	// is realized

	void realize();	// create widgets this only
	void unrealize();	// destroy windgets this only

	void realizeAll();			// realize whole fields tree
	void unrealizeAll();		//           -- / --

	virtual void _realize();	// create widgets this only
	virtual void _unrealize();	// destroy windgets this only

	// ***********************
	// general virtual methods
	// ***********************

	struct EnumerateObjects {
		virtual bool execute() = 0;
		virtual bool operator()(FmObject *obj) = 0;
		EnumerateObjects();
	private:
		EnumerateObjects(const EnumerateObjects &);
		const EnumerateObjects &operator=(const EnumerateObjects &);
	};

	struct EnumAll : EnumerateObjects {
		FmObject *object;
		virtual bool operator()(FmObject *obj);
	};
	
	struct EnumFieldObj : EnumerateObjects {
		FmFieldObj *object;
		virtual bool operator()(FmObject *obj);
	};
	
	struct EnumValueObj : EnumerateObjects {
		FmValueObj *object;
		virtual bool operator()(FmObject *obj);
	};
	
	struct EnumerateObjectsRows {
		FmBufferRow br;
		virtual bool execute() = 0;
		virtual bool operator()(FmObject *obj) = 0;
		inline bool operator()(FmObject *obj, FmBufferRow theBr);
		EnumerateObjectsRows();
	private:
		EnumerateObjectsRows(const EnumerateObjectsRows &);
		const EnumerateObjectsRows &operator=(const EnumerateObjectsRows &);
	};
	
	struct EnumFieldObjRows : EnumerateObjectsRows {
		FmFieldObj *object;
		virtual bool operator()(FmObject *obj);
	};
	
	struct EnumValueObjRows : EnumerateObjectsRows {
		FmValueObj *object;
		virtual bool operator()(FmObject *obj);
	};
	
	virtual bool enumerateDown(EnumerateObjects &s);
	virtual bool enumerateUp(EnumerateObjects &s);
	virtual bool enumerateDown(EnumerateObjectsRows &s, FmBufferRow br);
	virtual bool enumerateUp(EnumerateObjectsRows &s, FmBufferRow br);

	// ************************
	// type cast methods
	// ************************

	// get field type
	enum FieldType {
		f_none		= ERR,
		f_alpha		= 0,
		f_num,
		f_date,
		f_time,
		f_mult,
		f_sform,
		f_grouped,
		f_flt,
		f_boolean,
		maxFieldType
	};
	virtual FieldType fType() const;

	virtual FmFieldObj *getFieldObj();
	virtual FmValueObj *getValueObj();
	virtual FmMultiLine *getMultiLine();

	// **********
	// properties
	// **********

	// get & set object's name
	inline const String &name() const;
	void name(const String &newName);

	wxColour *background() const;
    wxColour *foreground() const;
	const ColorVal *getBackground() const;
    const ColorVal *getForeground() const;
    void setBackground(const ColorVal &color);
    void setForeground(const ColorVal &color);
    
	void style( Int style );
	inline Int style() const;
	inline Int geom_X() const;
	inline Int geom_Y() const;
	inline Int geom_W() const;
	inline Int geom_H() const;
	virtual void geometry(Int x = ERR, Int y = ERR, Int w = ERR, Int h = ERR);
	
	// ****************************
	// methods for managing wxItems
	// ****************************

	// get object's associated item(s)
	virtual wxItem *item(Int rowNum) const;

	void setFmObject(wxItem *item, Int itemNum);
	void resetFmObject(wxItem *item);
	static FmObject *getFmObject(wxItem *item);
	static Int getItemNum(wxItem *item);

 	void updateOffsetAll();

	// ************************
	// specific virtual methods
	// ************************

	bool invalidOption(const FmOption &option) const;
	bool addOption(const FmOption &option);
	const FmOption *option(FmOpt::Name name);
	
	virtual void updateOffset(); // update position of field inside its parent

	// **********************
	// action virtual methods
	// **********************

	void print(Int level = 0) const;
	
	void _assert(const char *, const char *, int) const;
	void DUMP() const;
};

class FmFieldObj : public FmObject {
	typedef FmObject inherited;

	// Field type.
	// For example FormRep::NUM or FormRep::BOOL.
	Int objectId_;					// Offset in form's array
	Int offsetInBuffer_;			// offset of buffer

	String nextObj_;				// Next object (default traversal).
	String prevObj_;				// Prev object (default traversal).

	FmExpr *relCheck_;				// pointer to compiled relational check
	FmExpr *errMsg_;				// Error msg. when check fails

	FmExpr *isExpr_;				// skip if not void
	bool alwaysSkip_;				// skip will be always true (compiled)
	
	bool dspOnly_;					// true if should not allow modification
									// _but_ should not skip
	bool notNull_;					// true if does not allow null values
	bool unique_;

	bool neverHasBefore_;			// (compiled)
	bool neverHasAfter_;			// (compiled)
	FmExpr *hasBefore_;
	FmExpr *hasAfter_;

	void initValues();
	
protected:
	inline FmExpr *isExpr() const;
	void relCheck(const String &expr);
	void errMsg(const String &expr);
	void isExpr(FmExpr *expr);
	void isExpr(const String &expr);
	void hasBefore(const String &expr);
	void hasAfter(const String &expr);

public:
	FmFieldObj();
	~FmFieldObj();

	FM_DECLARE_ABSTRACT_DATA_CLASS();

	// *********************************
	// redefinition of inherited methods
	// *********************************

	virtual void _realize();	// create widgets this only
	virtual void _unrealize();	// destroy windgets this only

	// ********************************
	// methods for working with buffers
	// ********************************

	virtual Int bufferSize() const;
	inline Int offsetInBuffer() const;

	FmBufferRow buffer(Int row) const;		// bufferRow in selected form
	FmBufferRow buffer(FmBuffer *formBuff, Int row) const; 
				// bufferRow in specified form buffer
	Int currentRow() const;

	// *************************
	// multiline related methods
	// *************************

	inline Int numRows(FmBufferRow br) const;
	inline Int currNumRows() const;
	inline Int rowFrom(const FmFieldObj *obj, Int row) const;
	
	inline void value(const String &val, FmBuffer *formBuff, Int row);
	inline void value(const String &val, Int row);
	inline String value(FmBuffer *formBuff, Int row);
	inline String value(Int row);
	inline void clear(FmBuffer *formBuff, Int row);
	inline void clear(Int row);
	inline bool isNull(FmBuffer *formBuff, Int row) const;
	inline bool isNull(Int row) const;

	virtual void _value(const String &theValue, FmBufferRow br);
	virtual String _value(FmBufferRow br) const;
	virtual bool _isNull(FmBufferRow br) const;
	virtual void _clear(FmBufferRow br);

	virtual void _onConnectBuffer();	// called when current form's buffer
	virtual void _onDisconnectBuffer();	// is changed
	virtual void _allocBuffer(FmBufferRow br);	// allocate buffer memory
	virtual void _freeBuffer(FmBufferRow br);	// free all buffer memory

	void allocBufferAll(FmBufferRow br);
	void freeBufferAll(FmBufferRow br);
	void clearAll(FmBufferRow br);
	void clearAll();

	void onConnectBufferAll();		// called when current buffer is changed
	void onDisconnectBufferAll();	// called when current buffer is changed

	// **********************************
	// methods for interaction with forms
	// **********************************

	// set & get object descriptor
	void newObjectId();
	void clearObjectId();
	inline Int objectId() const;

	// **********
	// properties
	// **********

	// get help message's number
	String errorMsg(const String &val, Int row);
	
	// set & get display only flag
	void dspOnly(bool v);
	inline bool dspOnly() const;

	// get hasIs_ flag
	inline bool hasIs() const;
	inline bool alwaysSkip() const;
	bool haveToSkip(Int row);
	bool hasBefore(Int row);
	bool hasAfter(Int row);

	inline void notNull(bool v);
	inline bool notNull() const;

	inline void unique(bool v);
	inline bool unique() const;

	// ***********************************
	// methods for editing form
	// ***********************************

	bool fieldError(const String &msgName) const;
	virtual bool validate(Int row);
	bool validateRelCheck(Int row);

	bool canEnter(Int row);
	bool canLeave(Int row, bool reset = false);
	void enterNotify();
	void leaveNotify();
	bool userBefore(Int row);
	bool userAfter(Int row);

	virtual bool canHaveFocus() const;

	virtual void savePrevValue(Int row);
	virtual bool assignDefault(Int row);
	virtual void showDescription(Int row);
	virtual void saveSkipValues(Int row);
	virtual void restoreSkipValues(Int row);
	virtual void toggleSubform();
	virtual void closeSubform(bool ok);
	virtual bool openSubform(Int row, Int subf_index, bool manual,
								bool invoke_always);

	void assignDefaultsAll(FmBufferRow br);
	void assignDefaultsAll(Int row);

	void undo();

	// ************************
	// type cast method
	// ************************

	virtual FmFieldObj *getFieldObj();

	// **********************
	// action virtual methods
	// **********************

	virtual void updateFirstRow();
	
	// *************************
	// traversal related methods
	// *************************
	
	void setFocus(Int row);
	
	bool isCurrent(Int row) const;
	bool isCurrent() const;
	
	virtual void calcNextObjRow(FmFieldObj *&obj, Int &row);
	virtual void calcPrevObjRow(FmFieldObj *&obj, Int &row);
	
	virtual bool dontProcessKey(wxKeyEvent &event);
	
	virtual bool onFunctionKey(wxKeyEvent &event);
	virtual void onHelp(wxKeyEvent &event);
};


class FmObjCollection : public FmFieldObj {
	typedef FmFieldObj inherited;

	gtype(PtrArray, FmObject)	&objs_;

public:
	FmObjCollection();
	virtual ~FmObjCollection();
	
	FM_DECLARE_CLASS();

	// ***********************
	// methods for fields tree
	// ***********************

	virtual FmObjContainer *containerForChild();

	FmObject *objectByName(const String &name);

	virtual Int rowFromChild(Int numRow) const;

	virtual Int nObjects() const;
	virtual FmObject *object(Int num) const;
	virtual Int objectNum(FmObject *obj) const;
	virtual void add(FmObject *theObject, Int position = ERR);
	virtual void del(FmObject *theObject);

	// find child in path to descendant
	FmObject *childInPath(FmObject *obj) const;

	virtual void calcNextObjRow(FmFieldObj *&obj, Int &row);
	virtual void calcPrevObjRow(FmFieldObj *&obj, Int &row);
	
	virtual void enterChildNotify();
	virtual void leaveChildNotify();

	virtual bool canShowChild(FmObject *obj);
	virtual void showChild(FmObject *obj);
 	virtual Int offsetX() const;

	virtual bool enumerateDown(EnumerateObjects &s);
	virtual bool enumerateUp(EnumerateObjects &s);
	virtual bool enumerateDown(EnumerateObjectsRows &s, FmBufferRow br);
	virtual bool enumerateUp(EnumerateObjectsRows &s, FmBufferRow br);
};

// forms and multilies are derived from FmObjCollection
// they are manages numRows, numItems, deltaY and buffers
class FmObjContainer : public FmObjCollection {
	typedef FmObjCollection inherited;

	Int rowSize_;

public:
	FmObjContainer();
	~FmObjContainer();
	
	FM_DECLARE_ABSTRACT_CLASS();

	virtual Int allocOffset(Int theBufferSize); // returns buffer offset
	void alignOffset(Int theBufferSize);

	inline Int rowSize() const;

	virtual Int bufferSize() const;

	virtual FmBuffer *childBuffer(FmBufferRow br) const;

	virtual void onChange(FmFieldObj *object, FmBufferRow br);
										// called by buffer when changed ?
	virtual void allocBuffer(FmBuffer *buff);
										// called by buffer when created
	virtual void freeBuffer(FmBuffer *buff);
										// --//--//--//--//--//- destroyed
	
	virtual void _onConnectBuffer();	// when buffer becomes current
	virtual void _onDisconnectBuffer(); // when no buffer becomes current
	virtual void _allocBuffer(FmBufferRow br);	// allocate buffer memory
	virtual void _freeBuffer(FmBufferRow br);	// free own part of buffer

	virtual void _unrealize();

	virtual FmObjContainer *containerForChild();

	virtual Int childNumRows(FmBufferRow br) const;
	virtual Int childNumItems() const;
	virtual Int childFirstRow() const;
	virtual Int childCurrRow() const;
	virtual Int childDeltaY() const;

	virtual bool enumerateDown(EnumerateObjects &s);
	virtual bool enumerateUp(EnumerateObjects &s);
	virtual bool enumerateDown(EnumerateObjectsRows &s, FmBufferRow br);
	virtual bool enumerateUp(EnumerateObjectsRows &s, FmBufferRow br);
};

#include <ideafix/priv/fmobj.icc>

#endif


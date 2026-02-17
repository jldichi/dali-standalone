/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: fmrep.h,v 1.1.1.1 1996/04/16 18:51:40 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef WXFMREP_H
#define WXFMREP_H

#pragma interface

#include <ideafix/priv/fmobj.h>

class FormRep;
class CFormRep;
class FmAction;
class FmKeyZone;
class FmDataZone;
class Form;
class FmFrame;
class FmPanel;
class FmToolbar;
class EnumObject;

class wxPanel;
class wxFrame;
class wxKeyEvent;
class wxMenuBar;
class wxMenu;

struct FormalArg {
	String name_;
	enum Type {
		in		= 1 << 0,
		out		= 1 << 1,
		inout	= in | out
	};
	Type type_;
	inline FormalArg(const String &theName, Type theType);
};

struct FmMessage {
	String name_;
	String content_;
	inline FmMessage(const String &theName, String theContent);
};

typedef Byte SubformIndex;
class SubformDescr {
	friend class FormRep;

	String name_;
	FormRep *subform_;

public:
	SubformDescr(const String &name, FormRep *subform);
	const String &name() const;
	FormRep *subform() const;
	void store(FILE * out);
};

class gtype(PtrArray, FmFieldObj);
class gtype(Array, String);
class gtype(PtrArray, FormRep);
class gtype(PtrArray, FmMessage);
class gtype(PtrArray, FmAction);
class gtype(PtrArray, SubformDescr);
class gtype(PtrArray, FormalArg);
class gtype(PtrArray, EnumObject);


/*
	FmObjects inside a FormRep form a tree with the following
	sample structure:

	FormRep -- FmObjCollection --- FmObjGroup -- FmValueObj
	        |                 |              |
	        |                 |              -- FmValueObj
	        |                 |
	        |                 --- FmObjGroup -- FmValueObj
	        |                                |
	        |                                -- FmValueObj
	        |
	        -- FmObjCollection -- FmMultiLine -- FmObjGroup -- FmValueObj
	                          |                |             |
	                          |                |             -- FmValueObj
	                          |                |
	                          |                -- FmValueObj
	                          |
	                          -- FmValueObj
*/


class FormRep : public FmObjContainer {
	typedef FmObjContainer inherited;
	
	friend class FmFrame;
	friend class FmPanel;

	friend int fgenpar_parse();
	
	FmKeyZone *key_;
	FmDataZone *data_;

protected:

	FormRep *formParent_;					// parent of subform (field)
	SubformIndex subformNum_;				// subform identification

	FmBuffer baseBuffer;
	FmBuffer *selectedBuffer_;
	Int subformRow_;						// row in parent form

	FormRep *currentSubform_;
	static FormRep *currentForm_;

	gtype(PtrArray, FmAction)	&actns_;	// array of form actions
	gtype(PtrArray, FmMessage)	&msgs_;		// Form messages
	gtype(Array, String)		&schemas_;	// Schemas used within this form
	gtype(PtrArray, FormalArg)	&fArgs_;	// formal arguments
	gtype(PtrArray, FmFieldObj)	&idObjs_;	// array of id objects
	gtype(PtrArray, FmObject)	&allObjs_;	// array of all objects
	gtype(PtrArray, SubformDescr) &subfs_;	// Subforms w/ names
	gtype(PtrArray, EnumObject)	&enums_;	// enums

	// message table
	static MsgTable *fmsg;

	Form *form_;						// Form interface

	FmFrame *frame_;					// Frame surrounding us
	Int frameStyle_;
	wxMenuBar *menuBar_;
	FmToolbar *toolbar_;				// Our tool bar
	FmPanel *panel_;					// Our panel

	Int	flags_;							// flsgs from OpenForm
	String label_;						// Window label

	bool invalid_;						// used to check constructor's result


	// whether the read action found a record or not
	bool exists_;

	bool needRecalcIs_;
	bool needRecalcMask_;
	bool autoWrite_;
	bool dspStatus_;

	String		prevVal_;			// prev value
	FmFieldObj	*prevValObj_;
	Int			prevValRow_;

	FmFieldObj	*currObj_;			// current object
	Int			currRow_;			// current instance of object (for multis)
	
	FmFieldObj	*lastObj_;			// previous object (needed for traversal)
	Int 		lastRow_; 			// previous row (needed for traversal)
	
	FmFieldObj	*dstObj_;
	Int			dstRow_;

	FmFieldObj 	*desiredObj_; 		//	If NULL, the form
	Int			desiredRow_;		//	follows the normal sequence,
									//	otherwise it is the next object to
									//	be processed.

	Int		fkey_;					//	last pressed function key,
									//	returned by FmKeyCode

	SubformIndex nextSubformIndex_;	// subform to select (can be changed in
									// user's after()

	void initValues();					// assignment of default values

	void _toDb(Int tab, Int from, Int to,
		/* char *funct, */ 	Int rowno);
	void _fromDb(Int tab, Int from, Int to,
		/* char *funct, */ Int rowno, bool disp);

	void _setCurrObject(FmFieldObj *obj, Int row);
	
public:
	enum TraversalKey {
		k_none,
		k_enter,
		k_next,
		k_prev,
		k_left,
		k_right,
		k_down,
		k_up,
		k_toDest
	};
	static TraversalKey traversalKey(wxKeyEvent &ev);

protected:
	bool _traverseResetTo(FmFieldObj *obj);
	bool _traverseTo(FmFieldObj *obj, Int row);
	void _traverse(TraversalKey key);

public:
	enum { MAGIC = 0633 };   // correct to valid number !!!

	static Int first_sch;

	FormRep();
	FormRep(const String &formName);
	~FormRep();
	
	FM_DECLARE_CLASS();

	void openAndAlloc(const String &theName, Int theFlags = 0);
	virtual CFormRep *getCFormRep();

	void setCurrent();
	void unsetCurrent();
 
	virtual Fm::Action callUserAction(Fm::Event event);
	virtual bool callUserBefore(const FmFieldObj *theObject, Int rowNo);
	virtual bool callUserAfter(const FmFieldObj *theObject, Int rowNo);

	virtual int execute();
	virtual void executeAction(FmAction &);
	inline void executeAction(Fm::Event id);
	
    inline Form *getForm();

	// subform related stuff
	void addSubform(const String &name, FormRep *subform);
	void addSubform(const String &name);
	void addSubform(FormRep *subform);

private:	
	SubformDescr *findSubform(const String &name);
	SubformDescr *findSubform(const FormRep *object);
	
public:
	SubformIndex numSubforms() const;
	const String &subformName(SubformIndex index) const;
	FormRep *subform(SubformIndex index) const;
	SubformIndex subformIndex(const String &name) const;

	inline FormRep *formParent() const;
	inline bool isSubform() const;

	virtual void _realize();
	virtual void _unrealize();

	void selectBuffer(FmBuffer *theBuffer, Int row);
	FmBuffer *selectedBuffer() const;
	
	inline Int subformRow() const;
	inline FormRep *currentSubform() const;
	static inline FormRep *currentForm();
	inline bool isCurrentForm() const;		// this FormRep is current
	void disable();
	void enable();

	virtual void _onConnectBuffer();
	virtual void _onDisconnectBuffer();

	virtual void _allocBuffer(FmBufferRow br);
	virtual void _freeBuffer(FmBufferRow br);
	virtual void _clear(FmBufferRow br);

	virtual FmBuffer *childBuffer(FmBufferRow br) const;
	
	void clearAll();

	virtual Int numItems() const;

	virtual Int rowFromChild(Int numRow) const;

	// building facilities
	Int numIdObjects() const;
	void addIdObject(FmFieldObj *theObject);
	void delIdObject(FmFieldObj *theObject);
	void assignObjIds();
	void clearObjIds();

	Int numAllObjects() const;
	void addObject(FmObject *theObject);
	void delObject(FmObject *theObject);
	
	Int numSchemas() const;
	Int addSchema(const String &schName);
	Int addMessage(const String &msgName, const String &msgContent);
	Int addFormalArg(const String &argName, FormalArg::Type type);
	
	void resolveArgs(FmValueObj *fld, Int row, Int subf_index, 
						FormalArg::Type type, bool onlyCheck);

	void addEnum(EnumObject *enumObj);
	EnumObject *getEnum(const String &name) const;

	// set & get exists_ flag
	inline void exists(bool v);
	inline bool exists() const;

	inline String prevValue() const;
	inline void saveValue();
	inline void restoreValue();
	inline bool fieldWasChanged() const;

	// set/get 'needRecalc' flag
	inline void setNeedRecalcIs();
	inline void setNeedRecalcMask();
	inline bool needRecalcIs() const;
	inline bool needRecalcMask() const;

	inline bool autoWrite() const;

	// set/get form flags
	inline void flags(Int theFlags);
    inline Int flags() const;

	virtual void add(FmObject *theObject, Int position = ERR);
	virtual void del(FmObject *theObject);
	
	// set & get key container object
	void key(FmKeyZone *k);
	inline FmKeyZone *key() const;

	// set & get data container object
	void data(FmDataZone *d);
	inline FmDataZone *data() const;
	
	void addToKey(FmObject *obj);
	void addToData(FmObject *obj);

	// check if some object is in a specific zone
	bool inDataZone(FmObject *theObj = NULL) const;	// NULL - current
	bool inKeyZone(FmObject *theObj = NULL) const;	// NULL - current
	bool goFromKeyZone();
	bool goFromDataZone();

	// set & get the current object
	inline bool isCurrent(const FmFieldObj *obj, Int row) const;
	inline FmFieldObj *currObj() const;
	inline Int currRow() const;
	inline FmFieldObj *lastObj() const;
	inline Int lastRow() const;
	inline bool lastMoveWasUp() const;
	inline void setLastToCurrent();
	
	inline wxItem *currentItem() const;

	// set & get the desired object
	inline void resetDesiredObject();
	void gotoDesiredObject();
	inline void desiredObject(FmFieldObj *theObject, Int rowNum);
	inline FmFieldObj *desiredObject() const;
	inline Int desiredRow() const;
	inline FmFieldObj *dstObj() const;
	inline Int dstRow() const;

	// go to specific object disallowing going to key zone
	bool goToDataObject(FmFieldObj *theObj, Int theNum);

	// go to specific object disallowing going to data zone
	bool goToKeyObject(FmFieldObj *theObj, Int theNum);

	// new traversal functions
	bool inTraverse() const;
	bool traverseToOrBack(FmFieldObj *obj, Int row);
	bool traverseToOrNext(FmFieldObj *obj, Int row);
	bool traverseToOrPrev(FmFieldObj *obj, Int row);
	bool traverseResetTo(FmFieldObj *obj);
	void traverse(wxKeyEvent &event);
	void traverse(TraversalKey code);

	void setFocus();

	void clearFlds(Int from = 0, Int to = INT_MAX, Int rowno = 0);

	bool hasKeyZone() const;

	// privilege checking
	bool canUpdate() const;
	bool canAdd() const;
	bool canDel() const;

	// predefined actions
	inline FmAction &nullAction();

	// user defined actions
	FmAction &action(Fm::Event id);
	inline FmAction &action(Int id);
	FmAction &action(const String &theName);

	FmAction *addAction(Int theId, const String &theName);
	FmAction *addAction(const String &theName);

	// propagate the exists_ flag to each subform
	void markSubforms();

	// return multiline row number of parent

	// assign default values to each object
	void assignDataDefaults();
	void assignKeyDefaults();

	// message displaying
	void clearStatus();
	void displayStatus(const String &m);
	void displayStatus(const char *m);
	void clearError();
	void displayError(const String &m);
	void displayError(const char *m);
	void clearMessage();
	void displayMessage(const String &m);
	void displayMessage(const char *m);

	// check for null not-null objects
	FmValueObj *checkNotNulls() const;

	// check if current object is valid
	bool currObjectValid() const;

	inline bool ok() const;
    
    void onShow(); // called just before showing
    void show(bool s);
    bool shown() const;

	virtual void calcNextObjRow(FmFieldObj *&obj, Int &row);
	virtual void calcPrevObjRow(FmFieldObj *&obj, Int &row);
	
	virtual void onMenuCommand(Int commandId);
	virtual bool onFunctionKey(wxKeyEvent &event);
	virtual void onHelp(wxKeyEvent &event);

	// find the youngest common ancestor between the two objects
	FmFieldObj *youngestCommonAncestor(FmFieldObj *object1, Int rowNum1,
			FmFieldObj *object2, Int rowNum2);

	FmFieldObj *idObject(Int n) const;
	FmFieldObj *idObject(const String &name) const;
	Int nKeys() const;

	wxFrame *frame() const;
	wxPanel *panel() const;
	inline FmToolbar * toolbar() const;
	void updateToolbarSize();

	// move up & down the object hierarchy
	bool upFromLeaf(FmFieldObj *fObj, Int fNum, FmFieldObj *tObj);
	bool downToLeaf(FmFieldObj *fObj, FmFieldObj *tObj, Int tNum);

	virtual void onHelp();

	// message gathering
	static String getMessage(const char *);
	static String getMessage(const String &key);
	FmMessage *message(Int msgNum);
	FmMessage *message(const String &theName);
	Int nMessage(const String &theName);
	const String &getMessage(Int msgNum);
	Fm::Action errorMessage(Int msgNum, va_list ap);
	Fm::Action errorMessage(Int msgNum, ...);

	void createToolBar();
	void createMenus();

	inline Int keyCode() const;
	inline void keyCode(Int code);

	virtual bool canShowChild(FmObject *obj);
	virtual void enterChildNotify();
	virtual void leaveChildNotify();

	virtual void showChild(FmObject *obj);
 	virtual Int offsetX() const;

	void toDb(Int from, Int to, Int rowno);
	void toTable(Int tab, Int rowno);
	void fromDb(Int from, Int to, Int rowno);
	void fromTable(Int tab, Int rowno);
	void setDisplayOnly(Int from, Int to, bool value);
	bool open(const String &theName, FormRep *theParent);
	FormRep *loadSubform(const String &nm);
	virtual FormRep *createSubform();

	Int checkSum();
	bool verifyCheckSum(Int chksum);
	void recalc(Int curr_row);

	Fm::Action invokeOnKeyCallBack(FmFieldObj *obj, Int row);
	
	inline SubformIndex nextSubformIndex() const;
	void nextSubformIndex(SubformIndex index);

	//  load and store methods
	void store();
	virtual bool loadActions(FILE *in);
	virtual void storeActions(FILE *out);
	void storeMenu(FILE *out, wxMenu *menu);
	virtual wxMenu *loadMenu(FILE *in);
};

class FmKeyZone : public FmObjCollection {
	typedef FmObjCollection inherited;
	
public:
	FmKeyZone();
	
	FM_DECLARE_CLASS();
	
	virtual void calcNextObjRow(FmFieldObj *&obj, Int &row);
	virtual void calcPrevObjRow(FmFieldObj *&obj, Int &row);
};

class FmDataZone : public FmObjCollection {
	typedef FmObjCollection inherited;
	
public:
	FmDataZone();
	
	FM_DECLARE_CLASS();
	
	virtual void calcNextObjRow(FmFieldObj *&obj, Int &row);
	virtual void calcPrevObjRow(FmFieldObj *&obj, Int &row);
};

#include <ideafix/priv/fmrep.icc>

#endif


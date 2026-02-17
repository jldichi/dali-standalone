/*
 * $Id: fmmult.h,v 1.1.1.1 1996/04/16 18:51:40 gustavof Exp $
 */

#ifndef FMMULT_H
#define FMMULT_H

#pragma interface

#include <ideafix/priv/fmobj.h>

class FmMultiScrollBar;
class FmMultiScrollBarX;
class FmMultiScroller;

//////////////////////////////////////////////////////////////////

class FmMultiLine : public FmObjContainer {
	typedef FmObjContainer inherited;

	friend class FmMultiScrollBar;
	friend class FmMultiScroller;

	Int numItems_;
	Int	firstRow_;				// first row in multiple field.
	Int	currRow_;				// current row in multiple field.
	Int	deltaY_;				// delta Y on screen

	bool ignoreAdd_;			// Add a new row
	bool ignoreDel_;			// Delete a row
	bool ignoreIns_;			// Insert a row

	FmValueObj *rector_;
	FmMultiScroller *scroller_;	// headers/footers

	FmMultiScrollBar *scrollBar;
	Int scrollBarRows_;
	FmMultiScrollBarX *scrollBarX;

	Int valueX_;				// current value of scrollBarX
	Int valueX0_;				// value of scrollBarX when objects are
								// at the default position
	Int addLengthX_;			// current length of scrollBarX - geom_W()

 	void setValueX(Int offset);	// set value of scrollBarX

	Int	needFirstRow_;			// to try to set this when enter

	Int scrollBarRows() const;

	void scrollToRow(Int row);

	void setScrollBarPos();

	bool isNullAll(FmBufferRow br);	// is row -> rowIsClear, else _isNull
	bool validateDest();

	void getBuffer();
	void setBuffer(char *buffer,Int rowSize, Int bufferRows);
	
	FmBuffer *currChildBuffer();
	
	void updateNumRows(FmBufferRow br);
	void updateScrollBar();

public:
	FmMultiLine();
	~FmMultiLine();
	
	FM_DECLARE_CLASS();

	virtual FieldType fType() const;
	virtual FmMultiLine *getMultiLine();
	
	void numItems(int numItems);

	void rector(FmValueObj *field);
	inline FmValueObj *rector() const;

	inline FmMultiScroller *scroller() const;
	inline FmObjCollection *header() const;
	inline FmObjCollection *footer() const;
	FmMultiScroller *scrollerNotNull();
	inline FmObjCollection *headerNotNull();
	inline FmObjCollection *footerNotNull();

	inline void addToHeader(FmObject *obj);
	inline void addToFooter(FmObject *obj);

	virtual void _realize();
	virtual void _unrealize();

	virtual bool _isNull(FmBufferRow br) const;
	virtual void _clear(FmBufferRow br);
	virtual void _allocBuffer(FmBufferRow br);
	virtual void _freeBuffer(FmBufferRow br);

	virtual void _onConnectBuffer();
	virtual void _onDisconnectBuffer();

	virtual Int rowFromChild(Int numRow) const;

	bool canLeaveRector();
	virtual void enterChildNotify();
	void enterRectorNotify();
	void leaveRectorNotify();
	void onChangeRector(FmBufferRow br);

	virtual bool validate(Int row);

	inline bool ignoreAdd() const;
	inline void ignoreAdd(bool theIgnAdd);
	inline bool ignoreDel() const;
	inline void ignoreDel(bool theIgnDel);
	inline bool ignoreIns() const;
	inline void ignoreIns(bool theIgnIns);

	void notifyScrollBar(Int pos);
	void notifyScrollBarX(Int pos);

	virtual Int childNumItems() const;
	virtual Int childNumRows(FmBufferRow br) const;
	virtual Int childFirstRow() const;
	virtual Int childCurrRow() const;
	virtual Int childDeltaY() const;

	Int	nFields() const;

	void deltaY( Int theDeltaY );

	void addRow();
	void delRow();

	virtual void calcNextObjRow(FmFieldObj *&obj, Int &row);
	virtual void calcPrevObjRow(FmFieldObj *&obj, Int &row);
	
	virtual bool onFunctionKey(wxKeyEvent &event);

	virtual bool canShowChild(FmObject *obj);
	virtual void showChild(FmObject *obj);
 	virtual Int offsetX() const;
 	inline Int addLengthX() const;
 	inline Int lengthX() const;
};

/////////////////////////////////////////////////////////////

class FmMultiScroller : public FmObjCollection {
	typedef FmObjCollection inherited;
	typedef FmObjCollection HeaderFooter;

	FmMultiLine *multi_;
	FmObjCollection *header_;
	FmObjCollection *footer_;

public:
	FmMultiScroller();
	
	FM_DECLARE_CLASS();

	virtual void add(FmObject *theObject, Int position = ERR);
	virtual void del(FmObject *theObject);
	
	inline FmMultiLine *multi() const;
	inline HeaderFooter *header() const;
	inline HeaderFooter *footer() const;
	HeaderFooter *headerNotNull();
	HeaderFooter *footerNotNull();

	virtual void _realize();

	virtual bool canShowChild(FmObject *obj);
	virtual void showChild(FmObject *obj);
 	virtual Int offsetX() const;
};

#include <ideafix/priv/fmmult.icc>

#endif

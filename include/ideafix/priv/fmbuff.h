/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: fmbuff.h,v 1.1.1.1 1996/04/16 18:51:40 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef FMBUFF_H
#define FMBUFF_H

#pragma interface

#include <ideafix.h>

class FmFieldObj;
class FmObjContainer;

class gtype(PtrArray, char);
declare(PtrArray, char);

#define assert2(EX,P1) \
		do { if (EX) break; ::DUMP(P1); _assert(quote(EX), __FILE__, __LINE__); } while(0)
#define assert3(EX,P1,P2) \
		do { if (EX) break; ::DUMP(P1); ::DUMP(P2); _assert(quote(EX), __FILE__, __LINE__); } while(0)

class FmBuffer
{
	gtype(PtrArray, char) buffer_;
	FmObjContainer	*object_;

	// to disallow copying
	void operator =(FmBuffer &);
	FmBuffer(FmBuffer&);

	// to disallow heap allocation of FmBuffer
	void *operator new(size_t s);
	void *operator new[](size_t s);
	void operator delete(void *);
	void operator delete[](void *);

	void *operator new(size_t s, FmBuffer *p);
	
public:
	FmBuffer();
	~FmBuffer();
	
	void connectTo(FmObjContainer *theContainer);
	void disconnect();

	inline FmObjContainer *object() const;
	inline Int numRows() const;
	
	Int rowSize() const;
	
	inline char *rowBuffer(Int row);
	
	void onChange(FmFieldObj *object, Int row); // notification
	
	void insRow(Int row);
	inline void addRow();	 // at the end
	void delRow(Int row);
	
	void _assert(const char *, const char *, int) const;
	void _assert(const char *, const char *, int, const String &) const;
};


///////////////////////////////////////////////////////////////

class FmBufferRow
{
	FmBuffer *buffer_;
	Int      row_;
public:
	inline FmBufferRow();
	inline FmBufferRow(const FmBufferRow &buffRow);
	inline FmBufferRow(const FmBufferRow &buffRow, Int theRow);
	inline FmBufferRow(FmBuffer *theBuffer, Int theRow);
	FmBufferRow(FmFieldObj *theObject, Int theRow );
	
	inline FmBuffer *buffer() const;
	inline Int row() const;
	char *data(const FmFieldObj *obj) const;
	inline FmObjContainer *object() const;
	
	inline void onChange(FmFieldObj *object); // notification
	
	inline FmBufferRow &operator=(const FmBufferRow &buffRow);
	inline bool operator==(const FmBufferRow &buffRow) const;
	inline bool operator!=(const FmBufferRow &buffRow) const;

	void _assert(const char *, const char *, int) const;
	void _assert(const char *, const char *, int, const String &) const;
};

void DUMP(const FmBuffer *buff);
void DUMP(const FmBufferRow &br);

////////////////////////////////////////////////////////////////////////

#if 0
#define FmField FMField

class FmField : public FmBufferRow
{
	FmObject	*object;
	
	inline bool equalTo(const String &str) const;
	FmField &assign(const String &str);
	
	void validate();
	
public:
	FmField(FmObject *, FmBufferRow );
	FmField(FmObject *, Int row);	// use current buffManager & row
	FmField(FmObject * );			// use current buffManager & row
	FmField(const FmField &fld);
	
	FmField operator()(Int subField, Int row) const;
	
	operator String() const;
	
	inline FmField &operator=(const FmField &fld);
	inline FmField &operator=(const char *str);
	inline FmField &operator=(const String &str);
	
	inline bool operator==(const FmField &fld) const;
	inline bool operator==(const char *str) const;
	inline bool operator==(const String &str) const;
};

#undef FMField
#endif

#include <ideafix/priv/fmbuff.icc>

#endif

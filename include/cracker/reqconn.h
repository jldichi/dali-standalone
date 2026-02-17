/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: reqconn.h,v 1.1.1.1 1996/04/16 18:51:50 gustavof Exp $
*
* DESCRIPTION:
*********************************************************************/

#ifndef REQCONN_H
#define REQCONN_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/priv/pconfig.h>
#include <essentia/netconn.h>

class FramedObject;
class gtype(PtrArray, FramedObject);

#define Stack(CONTENT)		gtype(Stack, CONTENT)
#define Stackdeclare(CONTENT)										\
class Stack(CONTENT) {												\
public:																\
	Stack(CONTENT)() : size_d(0), next_d(0), ptr_d(NULL) {}			\
	~Stack(CONTENT)() { reAlloc(0); }								\
																	\
	Int size() const { return size_d; }								\
	void push(const CONTENT elem)									\
	{																\
		if (next_d >= size_d)										\
			reAlloc(next_d + 8);									\
		ptr_d[next_d++] = elem;										\
	}																\
	CONTENT pop()													\
	{																\
		assert(next_d>0);											\
		return ptr_d[--next_d];										\
	}																\
																	\
private:															\
	void reAlloc(Int newSize);										\
	Int size_d;														\
	Int next_d;														\
	CONTENT *ptr_d;													\
};

#define Stackimplement(CONTENT)										\
void Stack(CONTENT)::reAlloc(Int size)								\
{																	\
	CONTENT *newPtr;												\
	next_d = min(next_d, size);										\
	if (size == 0)													\
		newPtr = NULL;												\
	else {															\
		newPtr = tr_new CONTENT[size];								\
		for (Int i = 0; i<next_d; i++)								\
			newPtr[i] = ptr_d[i];									\
	}																\
	tr_delete_vec(ptr_d);											\
	ptr_d = newPtr;													\
	size_d = size;													\
}

struct RequestInfo {
	Int object;
	Short command;
};

class gtype(Stack, RequestInfo);
declare(Stack, RequestInfo);

class RequestConnection : public NetConnection
{

public:

	explicit RequestConnection(const String &, const String &, bool execSv = true);
	explicit RequestConnection(int handle = 0, ConvType ct = NO_CONVERT);
	explicit RequestConnection(const String &service, bool local = true);
	~RequestConnection();

	enum MsgType {
		Info,
		Request,
		Reply
	};
	
	Int currObject() const;
	Short currCommand() const;
	MsgType messageType() const;
	bool needReply() const;
	bool isReply() const;
	
	operator bool() const;

	// to send
	RequestConnection &operator()(Short cmd, FramedObject *obj = NULL);
	RequestConnection &request(Short cmd, FramedObject *obj = NULL);
	RequestConnection &reply();

	void endMessage();
	void endReply();
	void flush();
    
    RequestConnection &waitForReply();
    
	// to read
	bool nextRequest();		// return false if is reply (should exit loop)
	void dispatch();

	Int verboseLevel() const;
	void verboseLevel(Int vl);
	
	FramedObject *findObject(Int id);

	void setDefaultObject(FramedObject *obj);
	Int addObject(FramedObject *obj);
	void delObject(FramedObject *obj);
	
//	void operator<<(void (*)(RequestConnection &));

protected:
	void _end();

	void dumpState(const String &mess) const;
	
	bool canWrite();
	bool canRead();
	bool empty();
	
	bool isWriting() const;
	bool isReading() const;
	bool notLast() const;

	virtual void _flush();
	virtual Int _fill();
	virtual void flushBuff();
	virtual void fillBuff();
	
	void start(FramedObject *obj, Short cmd, MsgType type);
	void start(Int obj, Short cmd, MsgType type);
	
private:
	typedef NetConnection inherited;

	void initValues();

	bool connected(FramedObject *) const;

    // objects related to this connection
    enum {
    	DefaultId = 0,
    	NotLast = 0x80000000,
    	IdMask = ~NotLast
    };
	Int lastId_d;
	gtype(PtrArray, FramedObject) &objs_d;
	
	Int verboseLevel_d;

	// current request header data
	Int currObj_d;
	Short currCmd_d;
	MsgType msgType_d;
	
	Stack(RequestInfo) requests_d;

	// current state of buffer
	enum {
		Empty,
		Reading,
		Writing
	} state_d;

	// to hide these names
	void flushResult(Int);
	short getOp();
	void putC(char c);
	char getC();
	bool moreData();
};

//inline void endMessage(RequestConnection &);
//inline void endReply(RequestConnection &);

#include <cracker/reqconn.icc>

#endif 


/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: bhashtab.h,v 1.1.1.1 1996/04/16 18:51:31 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/
//Oscar was documented this file (/usr2/acct/oscar/doc/bhash/bhash.doc)
#ifndef BHASHTAB_H
#define BHASHTAB_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/template.h>
#include <ifound/str.h>

struct HashNode {
	HashNode *next;
};

typedef	Int (*FP_CMP)(const void *, const void *) ;
typedef Int (*FP_HASH)(const void *, Int) ;
typedef void (*FP_DEL)(const HashNode *);


class BaseHashTab {

	friend class BaseHashTabCursor;

// private:

	HashNode **tab;

	BaseHashTab(const BaseHashTab &);
    void operator=(const BaseHashTab &);
    
protected:

	Int modulo;

	FP_CMP f_cmp;
	FP_HASH f_hash;
	FP_DEL f_del;

	HashNode *addNode(HashNode *node);
	void delNode(const HashNode &node);
	HashNode *findNode(void *node) const;

public:

	explicit BaseHashTab(Int m);
	~BaseHashTab();

#ifdef ISDEBUG
	static Int min;
	static Int max;
	static Int nAdds;
	static Int nDels;
	static Int nFinds;
	static Int nCmps;
	static Int nElems;
	static Int nBuckets;

	static void resetStats();
	static void computeStats(BaseHashTab *ht);
	static void printStats();

	HashNode **buckets() const { return tab; }
#endif
};

class BaseHashTabCursor {

// private:

	BaseHashTab *ht;
	Int slot;
	HashNode *nextNode;

	BaseHashTabCursor(const BaseHashTabCursor &);
    void operator=(const BaseHashTabCursor &);

protected:

	HashNode *currNode;

public:

	explicit BaseHashTabCursor(BaseHashTab *bht);

	operator void *();

	void first();
	void next();

};

#endif // BHASHTAB_H

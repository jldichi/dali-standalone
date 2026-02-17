/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* physlout.h,v 1.1.1.1 1996/04/16 18:51:50 gustavof Exp
*
* DESCRIPTION
*
*********************************************************************/

#ifndef	PHYSLOUT_H
#define	PHYSLOUT_H

#ifdef __GNUC__
#	pragma interface
#endif

class gtype(PtrArray, PhysLayoutEntry);
class gtype(PtrArray, String);
class gtype(PtrArray, Range);
class LRegExp;
class PackedEntry;
class RoundRobinEntry;
class RangedEntry;
class BufferedFile;

class PhysLayoutEntry {

protected:

	LRegExp &re_d;
	String dirSuffix_d;

public:

	PhysLayoutEntry(const LRegExp &r);
	PhysLayoutEntry(const String &r);
	virtual ~PhysLayoutEntry();

	bool belongsTo(const String &file);
	const String &pattern();

	void dirSuffix(const String &ds);

	virtual Int nParts() = 0;
	virtual String part(Int i) = 0;
	virtual Int part(Long &offset, Int bs ) = 0;
	virtual void store(BufferedFile *fp) = 0;

	virtual Int partSize(int part) = 0;

	virtual PackedEntry *packedEntry()			{ return NULL;	}
	virtual RoundRobinEntry *roundRobinEntry()	{ return NULL;	}
	virtual RangedEntry *rangeEntry()			{ return NULL;	}
};

class PackedEntry : public PhysLayoutEntry {

//private:

	String dir_d;

public:

	PackedEntry(const LRegExp &r, const String &d);
	PackedEntry(const String &r, const String &d);

	~PackedEntry();

	PackedEntry *packedEntry()					{ return this;	}

	Int part(Long &, Int = 0)					{ return 1;		}

	Int nParts();
	String part(Int i);

	void store(BufferedFile *fp);

	Int partSize(int part) {return 1;}
};

class RoundRobinEntry : public PhysLayoutEntry {

//private:

	gtype(PtrArray, String) &dirs_d;
	String strSuggPageSize_d;
	Int suggPageSize_d;

public:

	RoundRobinEntry(const LRegExp &r, const String &d, const String &ps);
	RoundRobinEntry(const String &r, const String &d, const String &ps);
	~RoundRobinEntry();

	RoundRobinEntry *roundRobinEntry()		{ return this;	}

	Int nParts();
	String part(Int i);
	Int part(Long &offset, Int bs = 0);

	void store(BufferedFile *fp);

	Int partSize(int part) {return 1;}
};

struct Range {

	String dir;
	String amount;
	Int nbytes;
	
	Range(const String &d, const String &a);
	~Range();
};

class RangedEntry : public PhysLayoutEntry {

//private:

	gtype(PtrArray, Range) &ranges_d;

	inline Int nBytes(Int n, Int bs, Int nbytes);

public:

	RangedEntry(const LRegExp &r, const String &d);
	RangedEntry(const String &r, const String &d);
	~RangedEntry();

	RangedEntry *rangeEntry()			{ return this;	}

	Int nParts();
	String part(Int i);
	Int part(Long &offset, Int bs = 0);

	Int partSize(int part);

	void store(BufferedFile *fp);
};

class PhysLayout {

//private:

	gtype(PtrArray, PhysLayoutEntry) &parts_d;
	String fileName_d;
	bool dirty_d;
	bool ok_d;
		
	void load();

public:

	PhysLayout(const String &file);
	~PhysLayout();
	
	Int nEntries();
	PhysLayoutEntry *entry(Int i);
	
	PhysLayoutEntry *find(const String &file, Int from = 0);

	const String &fileName()			{ return fileName_d;	}
	bool ok()							{ return ok_d;			}

	void addPackedEntry(const String &ln, const String &d, 
						const String &ds = NULL_STRING);
	void store();

	void delEntry(Int i);
};

#endif

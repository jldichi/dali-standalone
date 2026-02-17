/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: osfile.h,v 1.5 2003/05/29 22:27:06 albertoi Exp $
*
* DESCRIPTION
* Interface to operating system files
*********************************************************************/

#ifndef	OSFILE_H
#define	OSFILE_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>
#include <ifound/itypes.h>
#include <local/physlout.h>

#define IO_LOCKED		(-2)

class PhysFile;
class RegExpSet;
class PhysLayout;

class OsFile {

	friend class PhysFile;

public:

	enum {
		READ   = 1 << 0,
		WRITE  = 1 << 1,

		CREAT  = 1 << 4,
		TRUNC  = 1 << 5,
		FAIL   = 1 << 6,
		LOCK   = 1 << 7,
		TEST   = 1 << 8,
		APPEND = 1 << 9,
		EXCL   = 1 << 10,
		SYNC   = 1 << 11
	};

private :

	String	name_d;				// physical name
	String	ext_d;				// physical extension
	UnShort	errno_d;			// errno of last operation

	String lName_d;				// logical name
	Int nparts_d;				// amount of physical files
	PhysFile *pfiles_d;			// array with physical files
	PhysLayoutEntry *plentry_d;	// phys. layout entry which describes
								// how the phys. files are distributed
	Int bsize_d;				// block size used by the entry when it
								// is ranged or round-robin type

	OsFile(const OsFile&);
	void operator=(const OsFile&);

	void resetCounters();
	void checkIfExists();
	void setPhyFileFlags(Int flags);

	void closeAndDelete();

	inline void checkMethod(const char *func) const;

	PhysFile *pfiles(Int i = 0);
	const PhysFile *pfiles(Int i = 0) const;

	PhysFile *physFile(Long &offset);
	
protected:

	virtual	bool syncCache();
	virtual	bool flush();

	// these routines can be used to access directly to a certain
	// part (the programmer has to be carefully when using these methods)
	Int	read(Int part, void *buff, Int offset, Int length);

	Int	write(Int part, void *buff, Int offset, Int length);
	Int	awrite(Int part, void *buff, Int offset, Int length);
	
	inline Int nparts();
	String fullName(Int part);
	bool isPacked();

	Int partSize(int part) { return plentry_d->partSize(part); }

	Int nreads_d;				// Number of reads for the object
	Int nwrites_d;				// Number of writes for the object
		
public:

	static void syncAllCache();
	static void flushAll();

	OsFile();
	OsFile(const String &n, Int flgs);
	OsFile(const String &p, const String &n, const String &e, Int flgs);
	OsFile(PhysLayout *p, const String &ln, const String &e, Int flgs, 
		   Int bz, const String &pn = NULL_STRING);
	virtual ~OsFile();

	bool open(PhysLayout *p, const String &n, const String &e, Int flgs,
			  Int bz, const String &pn = NULL_STRING);
	bool open(const String &p, const String &n, const String &e, Int flgs);
	bool open(Int flgs);
	void close();

	inline bool ok() const;
	inline operator const void *() const;
	inline int operator!() const;

	bool 	isOpen();
	String	fullName();
	
	short	getFd();

	Int		write(const void *buff, Long offset, Int length);
	Int		awrite(const void *buff, Long offset, Int length);
	Int		read(void *buff, Long offset, Int length);
	Int		read(void *buff, Long offset, Int length, Int mode);
	Int		lock(Int mode);
	Int		lock(Long offset, Int length, Int mode);
	void	free(Long offset = 0, Int length = 0);
	void	freeAnFd();
	void	unLink(void);
	void	rename(const String &p, const String &n, const String &e);
	void 	rename(const String &n, const String &e);
	bool	empty();
	Int		size();
	Time	mtime();
	Int		owner();
	Int		sync();
	Int		lockOwner(Long offset = 0, Int length = 0);
	bool	isFileIfExists();
	bool	isDir();

	inline int errNo() const;
	inline void errNo(int err);

	inline void bsize(Int bs);

	inline String nameExt();
	const String &path();
	inline const String &name();
	inline const String &ext();
	inline const String &logicalName();
	
	static Int nReads();
	static Int nWrites();

	Int nObjReads();
	Int nObjWrites();

	static void setHandler(IFPCCPI func);
	static void waitForWrites();
};

#include <local/osfile.icc>

#endif

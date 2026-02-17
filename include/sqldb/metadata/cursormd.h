//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: cursormd.h,v 1.10 2002/03/19 21:24:43 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef CURSORMD_H
#define	CURSORMD_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>
#include <essentia/dbconst.h>
#include <sqldb/fwork/cursor.h>

/**
 * This class is used to encapsulate cursor metadata. A CursorMetadata 
 * object is created from a pointer to an allocated s_dbcur structure
 * (this may be taken from an allocated array of s_dbcur), a Cursor
 * object.
 *
 * @version
 * @author Eduardo Chiocconi.
 */
class CursorMetadata {

public:
	CursorMetadata(struct s_dbcur *c, const SQLFWCursor &cur, Int id, Int lmode);
	~CursorMetadata();

	// Get methods.
	inline struct s_dbcur *cursorRep() const;
	inline dbindex getIndexId() const;
	inline Int lockingMode() const;
	inline Int getNFields() const;
	inline char *getRangeFromBuffer() const;
	inline char *getRangeToBuffer() const;
	inline void changeRep(struct s_dbcur *c);
	inline Int getCursorId() const;
	inline char *getCache() const;
	inline Int getFlags() const;
	char *getRangeFromFldBuff(Int fld) const;
	char *getRangeToFldBuff(Int fld) const;
	inline bool hasToRetrieveAllFields() const;
	inline dbfield *cursorFields() const;
	inline Int nCursorFields() const;
	inline bool hasToRetrieveCursorFieldsByPK() const;

	// Set methods
	inline void setFlags(Int flags);
	void setRangeFromFldBuff(Int fld, const char *buff, Int bufflen);
	void setRangeToFldBuff(Int fld, const char *buff, Int bufflen);

	// Debug
	void dump() const;

private:
	struct s_dbcur *cursorRep_d;
	Int cursorId_d;		// This is the position of the CursorMetadata in the
						// array of opened cursors.

	// Set methods.
	void setId(Int id);
	void setIndexDescriptor(dbindex ind);
	void setLockMode(Int lmode);
	void setRecsAllocs(Short recs);
	void setCache(Int tablen);
	void setRangeToBuffer(Int tablen);
	void setRangeFromBuffer(Int tablen);
	void setFields(dbfield *flds);
	void setNFields(Int nflds);
	void setCursor(const SQLFWCursor &c);
	inline Short setRecsAllocs() const;

	void initializeSCursor();
};

#include <sqldb/metadata/cursormd.icc>

#endif	// CURSORMD_H

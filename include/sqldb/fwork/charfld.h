//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: charfld.h,v 1.10 2002/03/20 15:12:38 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef	CHARFIELD_H
#define	CHARFIELD_H

#include <sqldb/fwork/field.h>

/**
 * This class is used to represent SQL character fields.
 * 
 * @version
 * @author Eduardo Chiocconi
 * @see Field
 */
class CharacterField : public Field {

public:
	// Dinamically created field.
	CharacterField(const String &name, Int len, const ConstraintList &cl, Int dim = 0);
	~CharacterField();

	// Get Information methods.
	inline Int getTypeCode() const			{ return (length()>255) ? CHARGT255_TYPE_CODE : CHAR_TYPE_CODE; }
	inline bool hasLength() const			{ return true; }
	inline Int length() const				{ return len_d; }
	inline bool hasDecimals() const			{ return false; }
	Object *clone() const;

	// Debug
	void dump() const;

private:
	Int len_d;

	CharacterField(const CharacterField &c);
	CharacterField &operator=(const CharacterField &c);

};

#endif	// CHARFIELD_H

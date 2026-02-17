//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: object.h,v 1.5 2002/03/19 20:44:04 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef OBJECT_H
#define	OBJECT_H

class String;

/**
 * This class is used to represent Objects of any kind. Almost all classes
 * derive from this one giving us a great grade of polymorfism.
 * 
 * @version
 * @author Eduardo Chiocconi
 */
class Object {

public:
	Object();
	virtual ~Object();

	virtual String convertToString() const = 0;

	// Debug
	virtual void dump() const = 0;
	virtual Object *clone() const = 0;

private:
	Object(const Object &c);
	Object &operator=(const Object &c);
};

#endif	// OBJECT_H

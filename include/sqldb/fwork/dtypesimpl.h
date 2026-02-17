//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: dtypesimpl.h,v 1.4 2002/03/19 20:46:16 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef DATATYPESIMPL_H
#define DATATYPESIMPL_H

#include <ifound.h>
#include <ifound/str.h>

#define	MAX_TYPES_TABLE_LEN		10

/**
 * Define codes for the DataTypes.
 */
#define	CHAR_TYPE_CODE		0
#define	BIT_TYPE_CODE		1
#define	BOOL_TYPE_CODE		2
#define	NUM_TYPE_CODE		3
#define	INTEGER_TYPE_CODE	4
#define	DEC_TYPE_CODE		5
#define	FLOAT_TYPE_CODE		6
#define	DATE_TYPE_CODE		7
#define	TIME_TYPE_CODE		8
#define	CHARGT255_TYPE_CODE	9

struct typeStruct {
	Int typeCode_d;
	char *typeName_d;
};

typedef	struct typeStruct typeStruct;

/**
 * This class is used to get the String for a datatype given the code
 * of the desired code. We should not create an object of this class.
 * Instead we should be call getTypeAsString method to get the desired
 * type String.
 *
 * @author	Eduardo Chiocconi.
 */
class DataTypesImpl {
public:
	DataTypesImpl();
	virtual ~DataTypesImpl();
	
	virtual String getTypeAsString(Int typeCode) const;
private:
	static typeStruct typesTable_d[];
};

#endif	// DATATYPESIMPL_H


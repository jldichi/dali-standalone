/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: typeid.h,v 1.3 1998/08/27 14:11:33 eduardoc Exp $
*
* DESCRIPTION
* Type defines
*********************************************************************/

#ifndef TYPEID_H
#define TYPEID_H

/*	General data types */

#define TY_NUMERIC			(0)
#define TY_STRING			(1)
#define TY_DATE				(2)
#define TY_TIME				(3)
#define TY_FLOAT			(4)
#define TY_BOOL				(5)
#define TY_ORDPAIR			(6)
#define TY_INSTID			(7)
#define TY_PTR				(8)
// We need to add TY_OBJECT to allow compatibility with version 432.
#define TY_OBJECT			(8)
#define TY_BINARY			(9)
#define TY_INTEGER			(10)
#define TY_REFERENCE		(11)
#define	TY_ARROWS			(0x80)
#define	TY_TYPE				(0xFF)
#define	TY_FULL_HEIGHT		(0x100)
#define	TY_SKIP				(0x200)
#define	TY_ARROWS_WIDTH		(0x400)
#define	TY_SUBFORM			(0x2000)
#define	TY_CONTROL			(0x4000)
#define	TY_INSIDE_DIALOG	(0x8000)
#define TY_DEFAULT			(-1)

#define TY_SQL_CHAR			(0)
#define TY_SQL_VARCHAR		(1)
#define TY_SQL_DECIMAL		(2)
#define TY_SQL_NUMERIC		(3)
#define TY_SQL_BIT			(4)
#define TY_SQL_TINYINT		(5)
#define TY_SQL_SMALLINT		(6)
#define TY_SQL_INTEGER		(7)
#define TY_SQL_BIGINT		(8)
#define TY_SQL_REAL			(9)
#define TY_SQL_FLOAT		(10)
#define TY_SQL_DOUBLE		(11)
#define TY_SQL_DATE			(12)
#define TY_SQL_TIME			(13)
#define TY_SQL_BOOL			(14)
#define TY_SQL_BINARY		(15)
#define TY_SQL_TEXT	   	  	(16)
/* Type define added to support compatibility with version 432. */
#define TY_SQL_OBJECT           (17)
#define TY_SQL_DEFAULT		(-1)

#endif
                 

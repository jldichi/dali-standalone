/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: itypesp.h,v 1.2 2003/02/28 20:15:25 albertoi Exp $
*
* DESCRIPTION
* Private include for itypes
*********************************************************************/

#ifndef	ITYPESP_H
#define	ITYPESP_H

// String class

#define	NBLOCK	16
#define	BYTESALLOC(used)	(((used)+NBLOCK) & ~(NBLOCK-1))
#define VALID_PTR(p)		((char *)(p) != HIGH_VALUE && (p))

class srep {

	useIdCheck(srep_id);

	UnShort	n_;
	UnShort	len_;
	char s_[1];

public:

	// This class is somewhat strange... do not define constructors
	// or destructors!  Use build to construct and destroy instead
	// of ~srep() to destroy.
	static inline srep *build(UnShort len);	
	inline void destroy();

	inline UnShort &n();
	inline UnShort &len();
	inline char *s();

	inline void start();
	inline bool notEmpty() const;
};

#define ST_MAX_EXP		0xFFFF
#define ST_MIN_EXP		0
#define ST_ZERO			0x8000	
#define ST_SHORT_HIGH	0x8000	
#define ST_CHAR_HIGH	0x80	

#include <ifound/priv/itypesp.icc>

#endif		//	ITYPES_H

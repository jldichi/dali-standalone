/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: criobj.h,v 1.2 1999/08/06 00:30:30 eduardoc Exp $
*
* DESCRIPTION
* Class definitions for the Critical Object data type
*********************************************************************/

#ifndef	CRIOBJ_H
#define	CRIOBJ_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif


class CriticalObject {

// private:

		CriticalObject *next;                   
		
		CriticalObject(const CriticalObject &);
		void operator=(const CriticalObject &);

public:

		static void clear();

		CriticalObject();
		~CriticalObject();
};

#endif

/***********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: farmem.h,v 1.1.1.1 1996/04/16 18:51:35 gustavof Exp $
*
*
* DESCRIPTION:
*				far memory definitions
*
*********************************************************************/

#ifndef FARMEM_H
#define FARMEM_H

#ifndef HAVE_FARMEM
#	define farmalloc(a)			Alloc((a))
#	define farfree(a)			Free((a))
#	define hugeptr_add(a, b)	((a)+(b))
#	define hugeptr_diff(a, b)	((a)-(b))
#endif

#endif

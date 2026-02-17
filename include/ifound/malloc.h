/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: malloc.h,v 1.3 2002/03/15 19:01:30 hardaiz Exp $
*
*
* DESCRIPTION:
*			malloc header
*				
*
*********************************************************************/

#ifndef MALLOC_H
#define MALLOC_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

extern "C" {
	void malloc_stats();
	DLLIMPORT void *malloc(SizeT) __THROW;
	DLLIMPORT void *realloc(void* mem, SizeT) __THROW;
	DLLIMPORT void free(void* mem) __THROW;
	void *calloc(unsigned int n, unsigned int elem_size);
	unsigned int malloc_usable_size(void* mem);
};
#endif

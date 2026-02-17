/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: gnrange.c,v 1.1.1.1 1996/04/16 18:52:01 gustavof Exp $
*
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_OutOfRange    | Check database fields ranges
*********************************************************************/

#include <ideafix.h>
#include <ideafix/priv/gndefs.h>

/* to fix bug in gcc-2.6.0 running on iris5.2 */
#ifdef DEBUG
static double pow10(int len)
{
	switch (len) {
	case 0: 	return 1e0;
	case 1: 	return 1e1;
	case 2: 	return 1e2;
	case 3: 	return 1e3;
	case 4: 	return 1e4;
	case 5: 	return 1e5;
	case 6: 	return 1e6;
	case 7: 	return 1e7;
	case 8: 	return 1e8;
	case 9: 	return 1e9;
	case 10: 	return 1e10;
	case 11: 	return 1e11;
	case 12: 	return 1e12;
	case 13: 	return 1e13;
	case 14: 	return 1e14;
	case 15: 	return 1e15;
	default:
		assert(0);
		return -1.0;
	}
}
#else
static double pow10[] = {
	1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12,
	1e13, 1e14, 1e15 
};
#endif

int _OutOfRange(double val, int len)
{
	assert(len >= 0 && len < 16);
	return len && val!=MIN_DOUBLE && val!=MAX_DOUBLE && 
#ifdef DEBUG
			 (val <= -pow10(len) || val >= pow10(len));
#else
			 (val <= -pow10[len] || val >= pow10[len]);
#endif
}

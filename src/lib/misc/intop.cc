/********************************************************************
* Copyright (c) 1991 longerSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF longerSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: intop.cc,v 1.1.1.1 1996/04/16 18:51:58 gustavof Exp $
*
*
* DESCRIPTION:
*				Add, Div, Mult to long numbers handling special cases
*
*********************************************************************/

#include <ifound.h>

#if (__GNUC__ >= 8)
	Int idx::add(Int a, Int b)
#else
	Int add(Int a, Int b)
#endif
{ 
	if (a == -b) return 0;
	if (a > 0 && b > 0) {
		unsigned long ret = a + b;
		return ret >= LONG_MAX ? LONG_MAX : ret;
	}
	if (a < 0 && b < 0) {
		unsigned long ret = -a - b;
		return ret >= LONG_MAX ? LONG_MIN : -ret;
	}
	if (a == LONG_MAX || a == LONG_MIN) return a;
	if (b == LONG_MAX || b == LONG_MIN) return b;

	return a+b;
}

#if (__GNUC__ >= 8)
	Int idx::mult(Int a, Int b)
#else
	Int mult(Int a, Int b)
#endif
{ 
	double ret = a * b;	
	return ret > double(LONG_MAX) ? LONG_MAX : 
		  (ret < double(LONG_MIN) ? LONG_MIN : Int(ret));
}


#if (__GNUC__ >= 8)
	Int idx::div(Int a, Int b)
#else
	Int div(Int a, Int b)
#endif
{ 
	if (b == 0)
		return a >= 0 ? LONG_MAX : LONG_MIN;

	double ret = a / b;
	return ret > double(LONG_MAX) ? LONG_MAX : 
		  (ret < double(LONG_MIN) ? LONG_MIN : Int(ret));
}

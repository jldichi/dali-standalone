/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: gntyconv.c,v 1.1.1.1 1996/04/16 18:52:02 gustavof Exp $
*
*
* DESCRIPTION:
*      General routines for Types convertions.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*ItoL			 Convert short to long.
*ItoF			 Convert short to double.
*LtoF			 Convert long to double.
*LtoI			 Convert long to short.
*FtoI			 Convert double to short.
*FtoL			 Convert double to long.
*******************  Internal use only ******************************
*BtoI         Convert byte to short
*BtoL			 Convert byte to long
*BtoF			 Convert byte to float
*ItoB         Convert short to byte
*LtoB			 Convert long to byte
*FtoB			 Convert float to byte
*********************************************************************/

#include <ideafix.h>

/*
 * Convert short to long.
 */

long ItoL(short i)
{
	if (i == NULL_SHORT) return NULL_LONG;
	if (i == MAX_SHORT)  return MAX_LONG;
	return i;
}

/*
 * Convert short to double.
 */
double ItoF(short i)
{
	if (i == NULL_SHORT) return NULL_DOUBLE;
	if (i == MAX_SHORT)  return MAX_DOUBLE;
	return i;
}

/*
 * Convert long to double.
 */
double LtoF(long i)
{
	if (i == NULL_LONG) return NULL_DOUBLE;
	if (i == MAX_LONG)  return MAX_DOUBLE;
	return i;
}

/*
 * Convert long to short.
 */
short LtoI(long i)
{
	if (i == NULL_LONG) return NULL_SHORT;
	if (i == MAX_LONG)  return MAX_SHORT;
	return (short) i;
}

/*
 * Convert double to short.
 */
short FtoI(double i)
{
	if (i == NULL_DOUBLE) return NULL_SHORT;
	if (i == MAX_DOUBLE)  return MAX_SHORT;
	return (short) i;
}

/*
 * Convert double to long.
 */
long FtoL(double i)
{
	if (i == NULL_DOUBLE) return NULL_LONG;
	if (i == MAX_DOUBLE)  return MAX_LONG;
	return (long) i;
}

/*
 * Convert byte to double.
 */
double BtoF(char c)
{
	if ((signed char) c == _NULL_BYTE) return NULL_DOUBLE;
	if ((signed char) c == _MAX_BYTE)  return MAX_DOUBLE;
	return (signed char) c;
}

/*
 * Convert byte to long
 */
long BtoL(char c)
{
	if ((signed char) c == _NULL_BYTE) return NULL_LONG;
	if ((signed char) c == _MAX_BYTE)  return MAX_LONG;
	return (signed char) c;
}

/*
 * Convert byte to short
 */
short BtoI(char c)
{
	if ((signed char) c == _NULL_BYTE) return NULL_SHORT;
	if ((signed char) c == _MAX_BYTE)  return MAX_SHORT;
	return (signed char) c;
}

/*
 * Convert short to byte
 */
signed char ItoB(short i)
{
	if (i == NULL_SHORT) return _NULL_BYTE;
	if (i == MAX_SHORT) return _MAX_BYTE;
	return (signed char) i;
}

/*
 * Convert long to byte
 */
signed char LtoB(long i)
{
	if (i == NULL_LONG) return _NULL_BYTE;
	if (i == MAX_LONG) return _MAX_BYTE;
	return (signed char) i;
}

/*
 * Convert double to byte
 */
signed char FtoB(double i)
{
	if (i == NULL_DOUBLE) return _NULL_BYTE;
	if (i == MAX_DOUBLE) return _MAX_BYTE;
	return (signed char) i;
}

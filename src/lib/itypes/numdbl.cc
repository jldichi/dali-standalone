/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numdbl.cc,v 1.2 2004/09/29 15:30:48 damianm Exp $
*
* DESCRIPTION
* Convertion methods
*********************************************************************/

#include <stdlib.h>

#include <ifound.h>
#include <ifound/numdefs.h>
#include <ifound/stdc.h>

extern "C" void FToNum(double d, NUM *n)
{
	FCVTRETTYPE cs;
	int  ndec, sign;

	if (d == DOUBLE_MIN) {
		n->len = 0;
		n->exp = -MAX_EXP;
		return;
	} else if (d == DOUBLE_MAX) {
		n->len = 0;
		n->exp = MAX_EXP;
		return;
	}

	cs = eCvt(d, 15, &ndec, &sign);

	StrToNum(cs, n);
	NumShift(n, ndec-15);
	if (sign) n->exp = -n->exp;
}

extern "C" double NumToF(const NUM *n)
{
	if (n->exp == MAX_EXP)
		return DOUBLE_MAX;

	if (n->exp == -MAX_EXP)
		return DOUBLE_MIN;

	return atof(NumToStr(n, NTSDEF, INT_MAX));
}

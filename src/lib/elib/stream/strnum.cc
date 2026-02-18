/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strnum.cc,v 1.1.1.1 1996/04/16 18:52:43 gustavof Exp $
*
* DESCRIPTION: Input/Ouput of a number
*********************************************************************/
#include <ifound.h>
#include <ifound/chset.h>
#include <local/istream.h>
#include <local/strfile.h>

InputStream & InputStream::operator>>(Num &n)
{
	FmtBuff aux_buff;
	char c, *pbuff = aux_buff;
	// st tracks which position I'm processing
	// <0>-<1>9999<2>.<3>99999<4>E<5>-<6>9999
	int st = 0;

	while (*this >> c) {
		switch (c) {
		case '-' :
			if (st != 0 && st != 5) goto end;
			st++;
			break;
		case '.' :
			if (st > 2) goto end;
			st = 3;
			break;
		case 'e' :
		case 'E' :
			if (st != 2 && st != 4) goto end;
			st = 5;
			break;
		default :
			if (!isdigit(c)) goto end;
			if (st < 2)
				st = 2;
			else if (st == 3)
				st = 4;
			else if (st == 5)
				st = 6;
			break;
		}
		*pbuff++ = c;
	}
end :
	*pbuff = 0;
	n = Num(aux_buff);
	return *this;
}

OutputStream & OutputStream::operator << (const Num &num)
{
	return *this << toCharPtr(num);
}

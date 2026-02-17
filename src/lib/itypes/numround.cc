/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numround.cc,v 1.1.1.1 1996/04/16 18:51:54 gustavof Exp $
*
* DESCRIPTION: Rutinas de redondeo numerico.
*			  
* nthDig : Devuelve el n-esimo digito de un numero.
*          n n n n . n n
*          4 3 2 1   0 -1
* trunc  : Trunca un numero.
* floor  : Redondeo hacia abajo.
* ceil   : Redondeo hacia arriba.
* round  : Redondeo.
*
*********************************************************************/

#include <ifound.h>
#include <ifound/itypes.h>

Num Num::ceil(Int ndec) const
{
	Num a = *this;
	NumTrunc(&a, ndec, exp > 0);
	return a;
}

Num Num::floor(Int ndec) const
{
	Num a = *this;
	NumTrunc(&a, ndec, exp < 0);
	return a;
}

Num Num::round(Int ndec) const
{
	Num a = *this;
	NumTrunc(&a, ndec, NumNthDig(&a, -ndec) >= 5 );
	return a;
}

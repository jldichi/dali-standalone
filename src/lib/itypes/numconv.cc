/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: numconv.cc,v 1.3 1999/07/30 14:06:10 ekonig Exp $
*
* DESCRIPTION
* Convertion methods
*********************************************************************/

#include <ifound.h>
#include <ifound/numdefs.h>
#include <ifound/stdc.h>
#include <ifound/chset.h>
#include <string.h>

#define BASE_2 (long(NUM_BASE)*NUM_BASE)

static short tenpow[5] = { 1, 10, 100, 1000, 10000 };

inline int abs(int x) 
{ 
	return x < 0 ? -x : x;
}

extern "C" void ByteToNum(signed char i, NUM *a)
{
	assert(a != NULL);
	if (i == 0) {
		a->exp = a->len = 0;
		return;
	}
	if (i == (signed char) BYTE_MIN) { 
		a->exp = -MAX_EXP;
		a->len = 0;
		return;
	}
	if (i == (signed char) BYTE_MAX) {
		a->exp = MAX_EXP;
		a->len = 0;
		return;
	}

	LToNum(long(i), a);
}

extern "C" void IToNum(short i, NUM *a)
{
	assert(a != NULL);
	if (i == 0) {
		a->exp = a->len = 0;
		return;
	}
	if (i == SHORT_MIN) { 
		a->exp = -MAX_EXP;
		a->len = 0;
		return;
	}
	if (i == SHORT_MAX) {
		a->exp = MAX_EXP;
		a->len = 0;
		return;
	}

	LToNum(long(i), a);
}

extern "C" void LToNum(long j, NUM *a)
{
	assert(a != NULL);
	if (j == 0) {
		a->exp = a->len = 0;
		return;
	}

	if (j == LONG_MIN) { 
		a->exp = -MAX_EXP;
		a->len = 0;
		return;
	}
	if (j == LONG_MAX) {
		a->exp = MAX_EXP;
		a->len = 0;
		return;
	}

	register short *ps = a->s;
	bool minus = j < 0;
	if (minus) j = -j;

	if ((*ps = short(j/(BASE_2)))) {
		a->exp = BASE_EXP + 3;

		if ((j %= BASE_2) == 0)
			a->len = 1;
		else {	
			*++ps  = short(j/NUM_BASE);
			if ((*++ps  = short(j%NUM_BASE)) == 0)
				a->len = 2;
			else
				a->len = 3;
		}				
	}
	else if ((*ps = short(j/NUM_BASE)) != 0) {
		a->exp = BASE_EXP + 2;
		if ((*++ps  = short(j%NUM_BASE)) == 0)
			a->len = 1;
		else
			a->len = 2;
	}
	else {
		*ps = short(j);
		a->exp = BASE_EXP + 1;
		a->len = 1;
	}
	if (minus) a->exp = -a->exp;
}

extern "C" void StrToNum(const char *str, NUM *n)
{
	assert(n != NULL);

	if (str == LOW_VALUE) { 
		n->exp = -MAX_EXP;
		n->len = 0;
		return;
	}
	if (str == HIGH_VALUE) {
		n->exp = MAX_EXP;
		n->len = 0;
		return;
	}

	// Skip white space
	while (isspace(*str)) str++;

	// Check if null string

	if (*str == 0) {
		n->exp = -MAX_EXP;
		n->len = 0;
		return;
	}

	// Extract the sign

	int minus = (*str == '-');
	if (minus || *str == '+')
		str++;

	while (*str == '0') 
		str++;

	n->exp = BASE_EXP;

	register const char *p = str;
	register short *sp = n->s;
	const char *last;
	for (last = p; isdigit(*last); last++)
		;
	int length = last - str;

	if (length) {
		n->exp++;
		*sp = 0;
		for (register int j = (length-1) % 4 + 1; j; --j)
			*sp = *sp * 10 + (*p++ - '0');

		while (++sp < &(n->s[NUM_SHORTS])) {
			*sp = 0;
			register int k;
			for (k = 4; p < last && k; --k)
				*sp = *sp * 10 + (*p++ - '0');
			if (k)
				break;
			n->exp++;
		}
		n->exp += (last - p) / 4;
	}
	if (*p == '.') {
		/*
			Don't increment in the if since when it fails it leaves
			the pointer modified.
		*/
		p++;
		if (length == 0) {
			for (str = p; *str == '0'; str++) 
				;
			length = str - p;
			n->exp -= length/4;
			p = str - (length%4);
		}
		while (sp < &(n->s[NUM_SHORTS])) {
			*sp = 0;
			register int j;
			for (j = 4; isdigit(*p) && j; --j)
				*sp = *sp * 10 + (*p++ - '0');
			if (j == 4) break;
			while (j--)
				*sp *= 10;
			sp++;
		}
	}
	while (sp > n->s && sp[-1] == 0)
		--sp;
		
	if ((n->len = sp - n->s) == 0)
		n->exp = 0;
	else
	{
		if (minus) n->exp = -n->exp;

		if (toUpperCase(*p++) == 'E') {
			minus = (*p == '-');

			if ( !isdigit(*p) ) p++;

			int exponent = 0;

			while ( isdigit(*p) )
				exponent = exponent * 10 + *p++ - '0';

			(void)NumShift(n, minus ? -exponent : exponent );
		}
	}
}

extern "C" long  NumToL(const NUM *n)
{
	assert(n != NULL);
	short e = (n->exp >= 0 ? n->exp : -n->exp) - BASE_EXP;

	if (e <= 0)
		return 0;

	if (n->exp == -MAX_EXP)
		return INT_NULL;

	Int result = n->s[0];
	if (e > 1) {
		result *= NUM_BASE;
		if (n->len > 1) result += n->s[1];	

		if (e > 2) {
			if (e > 3 ||
				result > INT_MAX / NUM_BASE ||
				result == INT_MAX / NUM_BASE && 
				n->len > 2 && n->s[2] > INT_MAX % NUM_BASE)
				return n->exp >= 0 ? INT_MAX : INT_MIN;

			result *= NUM_BASE;
			if (n->len > 2) result += n->s[2];	
		}
	}
	return n->exp >= 0 ? result : -result;
}

extern "C" const char *NumToStr(const NUM *number, Int n, Int precision)
{
	assert(number != NULL);
	assert(precision >= 0);
	NUM tmp = *number;
	NUM *num = &tmp;
	static FmtBuff l_buf = {'\0'};
	register char *p = l_buf;
	Int nint, ndec, places;

	// el numero es NULL
	if (num->exp == -MAX_EXP) {
		*p = '\0';
		return l_buf;
	}

	if (num->exp == MAX_EXP) {
		// The number is very large.
		strcpy(l_buf, HIGH_VAL_IMAGE);
		return l_buf;
	}

	if (num->exp != 0) {
		places = NumTotDigits(num, &nint, &ndec);
		if (ndec > precision)
			NumRound(num, precision);
	}		
	if (num->exp == 0) {
		*p++ = '0';
		if (precision && precision != INT_MAX) {
			*p++ = '.';
			while (precision--) *p++ = '0';
		}	
		*p = 0;
		return l_buf;
	}

	if (num->exp < 0) {  	// the number is negative
		*p++ = '-';
		places++;
	}
	Int exponent = abs(num->exp) - BASE_EXP;
	if (exponent < 0) places++;	// Leading zero;
	if (ndec>0) places++;	// Has Decimal point

	if (n < 10) n = 10;  // Smallest posible buffer.

	if (places > n - 1) {    // The number doesn't fit in the buffer.
		NumExpFmt(num, p, short(n));
		return l_buf;
	}

	// The whole number fits in the buffer.

	bool after_dec_point = false;
	register short *sp = num->s;
	register short i = num->len;

	if (exponent <= 0) {   // Complete the leading 0's.
		*p++ = '0';
		*p++ = '.';
		for (register Int j = (-exponent)*4; j > 0; j--)
			*p++ = '0';
		after_dec_point = true;
	} else {
		// First short not copy leading zeroes
		int flag = false;
		if (*sp/1000) {
			*p++ = *sp / 1000 + '0';
			flag = true;
		}
		if (flag || *sp % 1000 / 100) {
			*p++ = *sp % 1000 / 100 + '0';
			flag = true;
		}
		if (flag || *sp % 100 / 10)
			*p++ = *sp % 100 / 10 + '0';
		*p++ = *sp%10 + '0';
		--i;
		sp++;
	}

	for (; i; i--, sp++) {
		if (!after_dec_point && num->len - i == exponent) {
			*p++ = '.';
			after_dec_point = true;
		}
		*p++ = *sp / 1000		+ '0';
		*p++ = *sp % 1000 / 100 + '0';
		*p++ = *sp % 100 / 10	+ '0';
		*p++ = *sp % 10			+ '0';
	}

	if (after_dec_point) {
		while (*--p == '0')
			;
		p++;
	}
	else {
		// I'm probably still missing some 0's at the end.
		if (num->len < exponent)
			for (i = short((exponent - num->len)*4); i > 0; i--)
				*p++ = '0';
	}
	if (precision != INT_MAX && ndec < precision) {
		if (!after_dec_point) *p++ = '.';
		while (ndec++ < precision) *p++ = '0';
	}
	*p = '\0';
	return l_buf;
}

static short trailingZeroes(short n)
{
	if (n % 10) return 0;
	if (n % 100) return 1;
	if (n % 1000) return 2;
	return 3;
}

static short leadingZeroes(short n)
{
	if (n >= 1000) return 0;
	if (n >= 100) return 1;
	if (n >= 10) return 2;
	return 3;
}

extern "C" long NumTotDigits(const NUM *a, long *nint, long *ndec)
{
	assert(a != NULL && nint != NULL && ndec != NULL);
	if ( a->len == 0 )
		return *nint = *ndec = 0;

	short exponent = abs(a->exp) - BASE_EXP;

	if ( exponent >= a->len ) {
		*ndec = 0;
		*nint = exponent * 4;
	}
	else {
		*ndec = (a->len-exponent) * 4 - trailingZeroes(a->s[a->len-1]);
		*nint = exponent < 0 ? 1 : exponent * 4;
	}
	if (exponent > 0) *nint -= leadingZeroes(a->s[0]);
	return *nint + *ndec;
}

extern "C" void NumExpFmt(const NUM *a, char *p, short n)
{
	assert(a != NULL);
	int next_dgt;
	int exponent = abs(a->exp) - BASE_EXP;

	// Calculate exponent.
	int tenexp = exponent * 4 - (next_dgt = (a->s[0] < 1000 ?
			a->s[0] < 100 ? a->s[0] < 10 ? 4 : 3 : 2 : 1 ));

	// Count the amount of available digits.
	int exp_dgts;

	// sign, dec. point, e+-, exp_digits.
	int av_dgts = n - (a->exp < 0) - 4 -
			(exp_dgts = abs(tenexp) < 100 ? abs(tenexp) < 10 ? 1 : 2 : 3);

	// Build the string.
	*p++ = (a->s[0] / tenpow[4-next_dgt]) % 10 + '0';
	*p++ = '.';

	register const short *sp;
	for (sp = a->s; av_dgts && sp < &(a->s[a->len]); sp++) {
		while (next_dgt++ < 4 && --av_dgts)
			*p++ = (*sp / tenpow[4-next_dgt]) % 10 + '0';
		next_dgt = 0;
	}

	// Remove the trailing zeros.
	while (*--p == '0');
	if (tenexp) {
		if (*p == '.')
			*p = 'e';
		else
			*++p = 'e';
		*++p = tenexp < 0 ? '-' : '+';
		tenexp = abs(tenexp);
		while (exp_dgts) {
			*++p = (tenexp / tenpow[exp_dgts-1]) % 10 + '0';
			tenexp = tenexp % tenpow[exp_dgts-1];
			exp_dgts--;
		}
	}
	*++p = '\0';
}

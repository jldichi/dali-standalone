/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: doformat.cc,v 1.10 2004/09/29 15:10:36 damianm Exp $
*
* DESCRIPTION
*********************************************************************/

#include <stdlib.h>
#include <string.h>

#include <ifound.h>
#include <ifound/itypes.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/chset.h>
#include <ifound/stdc.h>
#include <ifound/num.h>
#include <ifound/str.h>

static ConvertFunction functionPointer(int c);

#define MAX_WIDTH 255
#define LOWER_CONV 1
#define UPPER_CONV 2
#define OUTPUT_CHAR(chr) \
			if ((*ofunct)((chr), funct_arg) == -1) \
				return -1;                         \
			else                                   \
				++charcount

/*
	Formats a number using decimal point and thousand separator.
	Using 'decPoint()' to obtain the decimal point
	and 'thousandSep()' for thousand separator.
*/
extern "C" void FmtNum(char *obuff, char *ibuff, int length, int ndec,
			int comma)
{
	unsigned n; 
	char *p1, *p2, *ibuffx;


	ibuffx = ibuff;
	while (*ibuff && (*ibuff<'0' || *ibuff>'9')) ibuff++;

	n = strlen(ibuff);
	if (!length) {
		length = n;
		if (ndec) {
			if (length>ndec) length++;
			else length = ndec+2;
		}
		length += ibuffx-ibuff;
	}
	p1  = obuff+length;
	*p1 = 0;

	p2  = ibuff+n;

	if (p2 > ibuff)
		while (p1>obuff && ndec--) {
			*--p1 = p2>ibuff ? *--p2 : '0'; 
			if (ndec==0) {
				*--p1 = decPoint(); 
				if (p2==ibuff) *--p1 = '0';
			}
		}

	for (n=0; p1>obuff; n++) {
		if (comma && n && p2>ibuff && n%3==0) *--p1 = thousandSep();
		*--p1 = p2>ibuffx ? *--p2 : ' ';
	}
	return;
}

int doFormat(IFPICP ofunct, char *funct_arg, const char *fmt, va_list ap)
{
	int c;
	char wbuffer[MAX_WIDTH + 100];
	int charcount = 0;

	if (!fmt)
		return charcount;

	while ((c = *fmt++)) {
		/*
		* Output literal chars
		*/
		if (c != '%' || (c = *fmt++) == '%') {
			OUTPUT_CHAR(c);
			continue;
		}

		// Assign default values
		bool repflag = false;
		bool rj      = true;
		bool comma   = false;
		int  caseConv= 0;
		int  fillc   = ' ';
		int  width   = 0;		
		Int  maxwidth= INT_MAX;
		Int  numdec  = INT_MAX;
		int  size    = ' ';
		char *mask   = NULL;
		char *auxMask= NULL;
		
		// Check if left justified
		if (c == '-') {
			rj = false;
			c = *fmt++;
		}

		// Check if fill character is '0'.
		if (c == '0') {
			fillc = '0';
			c = *fmt++;
		}

		// Extract width
		if (c == '*') {
			width = va_arg(ap, int);
			c = *fmt++;
		} else {
			while (isdigit((UnChar)c)) {
				width = width*10 + c - '0';
				c = *fmt++;
			}
		}

		// Extract maxwidth
		if (c == '.') {
			if ((c = *fmt++) == '*') {
				maxwidth = va_arg(ap, int);
				c = *fmt++;
			} else {
				for (maxwidth = 0; isdigit(UnChar(c)); c = *fmt++)
					maxwidth = maxwidth*10 + c - '0';
			}
		}

		// Check if the formatting number is being used.
		if (c == '#') {
			if (*fmt == ',') {
				comma = true;
				fmt++;
			}
			if ((c = *fmt++) == '*') {
				numdec = va_arg(ap, int);
				c = *fmt++;
			} else {
				for (numdec = 0; isdigit(UnChar(c)); c = *fmt++)
					numdec = numdec * 10 + c - '0';
			}
		}

		// Check if the formatting mask is being used.
		if (c == '\'') {
			if ((c = *fmt++) == '*') {
				mask = va_arg(ap, char *);
				if ((c = *fmt++) == '\'')
					c = *fmt++;
			}
			else {
				mask = (char*)(fmt - 1);
				while (c != '\'') 
					c = *fmt++;
				auxMask = tr_new char[fmt - mask];
				strncpy(auxMask, mask, fmt - mask - 1);
				mask = auxMask;
				c = *fmt++;
			}
		} 

		// Check if conversion to upper or lower case is required
		if (c == '>') {
			caseConv = UPPER_CONV;
			c = *fmt++;
		} else if (c == '<') {
			caseConv = LOWER_CONV;
			c = *fmt++;
		}

		// Check if argument is Int or short
		if (c == 'l' || c == 'h') {
			size = c;
			c = *fmt++;
        }

		Int length;
		char *cp = wbuffer;
		ConvertFunction fp = functionPointer(c);
		if (fp) {
        	//cp = fp(c, &fmt, size, &maxwidth, &ap, wbuffer, mask, &numdec);
        	cp = fp(c, &fmt, size, &maxwidth, ap, wbuffer, mask, &numdec);
			tr_delete_vec(auxMask);
			auxMask = NULL;
			length = strlen(cp);
		} else {
			// default behavior
			*cp = (c == 'c') ? (char) va_arg(ap, int)
		  	    : (c == '%') ? *fmt++ : (char) c;
			if (maxwidth == INT_MAX) maxwidth = 1;
			length = maxwidth;
			repflag = true;
		}
		
		// Check if numeric formatting
		if (numdec != INT_MAX && length < 40) {
			cp[length] = '\0';
			if (width > MAX_WIDTH) width = MAX_WIDTH;
			FmtNum(wbuffer+100, cp, width, numdec, comma);
			length = width;
			cp = wbuffer+100;
		}

		if (length > maxwidth)
			length = maxwidth;
		
		if (rj) {
			if ((*cp == '-' || *cp == '+') && fillc == '0') {
				--width;
				--length;
				OUTPUT_CHAR(*cp++);
			}
			while (width-- > length)
				OUTPUT_CHAR(fillc);
		}

		for (int k = 0; *cp && k < maxwidth; ++k ) {
			char chr = caseConv == UPPER_CONV ? (char) toupper(*cp)
					 : caseConv == LOWER_CONV ? (char) tolower(*cp)
					 : *cp;
			OUTPUT_CHAR(chr);		 
			if (!repflag) ++cp;
		}
		
		if (!rj) {
			while (width-- > length)
				OUTPUT_CHAR(' ');
		}
	}
	va_end(ap);
	return charcount;
}

// convert double to string
#define BUFF_SIZE 64

void floatToStr(double val, char *buff, int dec,
							int tipo)
{
	int signo, pto_dec;
	FCVTRETTYPE cp;
	char *pbuff = NULL, *fcp;
	char *pto_ptr = NULL;
	char fbuff[BUFF_SIZE];

	memset(fbuff, 0, BUFF_SIZE);

	switch (tipo) {
	case 0:
		dec += 6;
	case 2:
		if (val == DOUBLE_NULL) {
			*buff = '\0';
			return;
		}
		if (val == DOUBLE_MAX) {
			strcpy(buff, HIGH_VAL_IMAGE);
			return;
		}

		if (val < 0.0) {
			*buff++ = '-';
			if (tipo) dec--;
		}

		pbuff = buff;

		cp = eCvt(val, dec, &pto_dec, &signo);

		// end of string
		fcp = (char*)(cp+dec-1);

		// decimal point falls within output length
		if (tipo && pto_dec <= dec && pto_dec >= -4) {
			if (pto_dec <= 0) 
				// put zeros to the left (if any)
				*pbuff++ = '0';
			else while (pto_dec > 0) {
				// copy up to decimal point
				*pbuff++ = *cp++;
				pto_dec--;
			}

			// remove trailing zeros
			for (;fcp >= cp && *fcp == '0';fcp--);
			fcp[1] = '\0';

			/*
				If enough space for decimal point and something to
				copy... go on
				'dec - 1' is to avoid puting just the dot ('.')
			*/
#ifdef HAVE_FLOATS_BUG
			char * ch = (char *)cp;
			if (*ch) {
#else
			if (*cp) {
#endif
				if (pbuff-buff < dec-1) {
					pto_ptr = pbuff;
					*pbuff++ = '.';

					// place leading zeros if needed
					while (pto_dec < 0) {
						*pbuff++ = '0';
						pto_dec++;
					}

					// copy whats left
					while (*cp && pbuff-buff < dec)
						*pbuff++ = *cp++;
				}
				// now round off
				if (*cp && *cp > '4') {

					for (fcp = pbuff - 1;
							fcp >= buff && (*fcp == '9' || *fcp == '.');
							fcp--) {
						if (pto_ptr && fcp >= pto_ptr)
							*fcp = '\0';
						else if (*fcp == '9')
							*fcp = '0';	
					}
					// are they all nines
					if (fcp < buff) {
						*pbuff++ = '0';
						*buff = '1';
					}
					else
						*fcp = *fcp + 1;	
				}
				else {
					/* pudieron quedar ceros de mas */
					for(fcp = pbuff-1;
						fcp >= buff && (*fcp=='0' || *fcp=='.'); fcp--);
					if (fcp >= buff) 
						fcp[1] = '\0';
					else {
						fcp[!signo] = '0';
						fcp[!signo + 1] = '\0';
					}
				}	
			}
		}
		else {
			/* Redondeo */
			UnChar last;
			fcp = (char*)(cp + dec - 5);
			last = *fcp;
			*fcp-- = '\0';
			if (last > '4') {
				for (;fcp >= cp && *fcp=='9'; fcp--)
					*fcp = '0';	
				if (fcp < cp) {
					*pbuff++ = '1';
					*(char*)(cp + dec - 5 - 1) = '\0';
					pto_dec++;
				}
				else {
					*fcp = *fcp + 1;
					*pbuff++ = *cp++;
				}
			}
			else *pbuff++ = *cp++;
			if (*cp) *pbuff++ = '.';
			while (*cp)  *pbuff++ = *cp++;
			*pbuff++ = 'e';
			if (--pto_dec >= 0) *pbuff++ = '+';
			else {
				*pbuff++ = '-';
				pto_dec = -pto_dec;
			}
			if (pto_dec > 100)
				*pbuff++ = (char) (pto_dec/100 + '0');
			pto_dec %= 100;
			*pbuff++ = (char) (pto_dec/10 + '0');
			*pbuff++ = (char) (pto_dec%10 + '0');
		}
		break;
	case 1:
		pbuff = buff;
		if (val == DOUBLE_NULL) {
			*buff = '\0';
			return;
		}
		if (val == DOUBLE_MAX) {
			strcpy(buff, HIGH_VAL_IMAGE);
			return;
		}

		char *aux = fCvt(val, dec, &pto_dec, &signo);
		assert(strlen(aux) < BUFF_SIZE);
		strcpy(fbuff, aux);

		/* Correct bug in Interactive's fcvt */
		if (val == 0.0) {
			pto_dec = 0;
		}

		cp = fbuff;
		if (!cp[0] || (!cp[1] && cp[0] == '0')) {
			*(char*)cp = 0;
			signo = 0;
			pto_dec = -dec; /* To fix a bug of the U 6050 */
		}

		if (signo && -pto_dec <= dec)
			*pbuff++ = '-';

		if (pto_dec <= 0) 
			*pbuff++ = '0';
		else {
			while (pto_dec > 0) {
				*pbuff++ = *cp++;
				pto_dec--;
			}
		}
		if (dec) *pbuff++ = '.';
		while (pto_dec++ < 0 && dec-- > 0) *pbuff++ = '0';
		while (*cp) *pbuff++ = *cp++;
		break;
	}
	assert(pbuff != NULL);
	*pbuff++ = '\0';
}

char *longToStr(Int v, char *cp, int base)
{
	static char digits[]="0123456789abcdef";
	bool signo = false;
	UnInt val = v;

	if (base < 0) {
		base = -base;
		if (v < 0) {
			val = - v;
			signo = true;
		}
	}

	*cp = '\0';
	//if (base != 10 || (v != INT_NULL && v != SHORT_NULL)) {
	if (base != 10 || (v != INT_NULL && v != SHORT_NULL && v != LONG_NULL)) {
		do {
			*--cp = digits[int(Int(val%base))];
		} while ((val /= base) != 0);
		if (signo)
			*--cp = '-';
	}
	return cp;
}

char *integerConversion(int c, const char **, int size, Int *,
	//			        va_list *ap, char *wbuff, char *, Int *)
				        va_list ap, char *wbuff, char *, Int *)
{
	Int val;
	Int base = -10;
	bool upper = false;
	
	switch (c) {
	case 'b':
		base = 2;
		break;
	case 'o':
		base = 8;
		break;
	case 'u':
		base = 10;
		break;
	case 'X':
		upper = true;
	case 'x':
		base = 16;
		break;
	case 'd':
		base = -10;
        break;
	}
	if (size == 'l')
		val = va_arg(ap, Int);
	else {
		
		if (size == 'h') {
		//	val = va_arg(*ap, short);
			val = va_arg(ap, int);    // For gcc 2.96
			if (short(val) == SHORT_NULL) 
				val = SHORT_NULL;
		}
		else {
			val = va_arg(ap, int);
			if (int(val) == INT_NULL) 
				val = INT_NULL;
		}
		
	}
	char *cp = longToStr(val, wbuff+32, base);
	return cp;
}

char *environmentVar(int , const char **fmt, int , Int *,
		//		     va_list *ap, char *wbuff, char *, Int *)
				     va_list ap, char *wbuff, char *, Int *)
{
	char *cp;

	/* Environment variable */
	if (**fmt == '*') {
		cp = va_arg(ap, char *);
		(*fmt)++;
	}
	else {
		cp = wbuff;
		if (isalnum((UnChar)**fmt))
			while (isalnum((UnChar)**fmt)) *cp++ = *(*fmt)++; 
		else if (**fmt == '{') {
			(*fmt)++;
			while (**fmt && **fmt != '}') *cp++ = *(*fmt)++; 
			if (**fmt) (*fmt)++;
		}
		*cp = '\0';
		cp = wbuff;
	}

	/* We don't want to Abort	*/
	char *VarName  = cp;
	if ((cp = (char*)readEnvVar(VarName)) == NULL) 
		(cp=wbuff)[0] = 0;

	return cp;
}

char *charPtrConversion(int , const char **, int , Int *,
	//			     	va_list *ap, char *wbuff, char *mask, Int *)
				     	va_list ap, char *wbuff, char *mask, Int *)
{
	static char *null_pointer = "(null)";
	char *cp;
	if ((cp = va_arg(ap, char *)) == NULL)
		cp = null_pointer;
	else if (mask != NULL) {
		processMask(wbuff + 100, mask, cp);
		cp = wbuff + 100;
	}
	return cp;
}

char *boolConversion(int , const char **, int size, Int *,
	//				 va_list *ap, char *, char *, Int *)
					 va_list ap, char *, char *, Int *)
{
	static char *yes = NULL, *no = NULL;
	if (yes == NULL) {
		String auxYes = Boolean(true).format();
		String auxNo = Boolean(false).format();
		yes = tr_new char[auxYes.length() + 1];
		no  = tr_new char[auxNo.length() + 1];
		strcpy(yes, toCharPtr(auxYes));
		strcpy(no, toCharPtr(auxNo));
	}
	Int v = size == 'l' ? va_arg(ap, Int) : va_arg(ap, int);
	char *cp = v == true  ? yes
			 : v == false ? no
			 : (char *)" "; //NULL
	return cp;
}

char *stringConversion(int , const char **, int , Int *,
		//		     	va_list *ap, char *, char *, Int *)
				     	va_list ap, char *, char *, Int *)
{
	char high[13];          

#ifndef __VC__
	String *s = va_arg(ap, String *);
	assert(s != NULL);
	char *cptr = toCharPtr(*s);
#else // hmmm...
	String s = va_arg(ap, String );
	assert(&s != NULL);
	char *cptr = toCharPtr(s);
#endif
  	strcpy(high, HIGH_VALUE_IMAGE);
    return cptr == HIGH_VALUE ? high : cptr;
}

char *numConversion(int c, const char **, int , Int *maxwidth,
	//			    va_list *ap, char *wbuff, char *, Int *numdec)
				    va_list ap, char *wbuff, char *, Int *numdec)
{
	char *cp = NULL;
	
	switch (c) {
	case 'e':
	case 'f':
	case 'g':
	case 'N' :
		{
			// Ponemos 6 porque es la presicion que se ve en el estandar de C.
			// Antes habia un 7 y en algunos casos daba signal 11.
			Int precision = *maxwidth==INT_MAX ? 6 : *maxwidth;
			if (c == 'N') {
				NUM *tmp;
				tmp = va_arg(ap, NUM*);
				strcpy(wbuff, NumToStr(tmp, NTSDEF, precision));
			}
			else {
				double aux = va_arg(ap, double);
				floatToStr(aux, wbuff, precision, c-'e');
			}
			if ((cp = strchr(wbuff, '.')) != NULL) {
				if (*numdec == INT_MAX) 
					*cp = decPoint();
				else {
				  (void) strcpy(cp, cp+1);
				  *numdec += strlen(cp);
				}
			}
			cp = wbuff;
			*maxwidth = 200;
		}
		break;
	default:
		assert(0);
		break;
	}

	return cp;
}

static ConvertFunction fpArray[] = {
	NULL,				// ' '
	NULL,				// '!'
	NULL,				// '"'
	NULL,				// '#'
	environmentVar,		// '$'
	NULL,				// %
	NULL,				// '&'
	NULL,				// '''
	NULL,				// '('
	NULL,				// ')'
	NULL,				// '*'
	NULL,				// '+'
	NULL,				// ','
	NULL,				// '-'
	NULL,				// '.'
	NULL,				// '/'
	NULL,				// '0'
	NULL,				// '1'
	NULL,				// '2'
	NULL,				// '3'
	NULL,				// '4'
	NULL,				// '5'
	NULL,				// '6'
	NULL,				// '7'
	NULL,				// '8'
	NULL,				// '9'
	NULL,				// ':'
	NULL,				// ';'
	NULL,				// '<'
	NULL,				// '='
	NULL,				// '>'
	NULL,				// '?'
	NULL,				// '@'
	NULL,				// 'A'
	boolConversion,		// 'B'
	NULL,				// 'C'
	NULL,				// 'D'
	NULL,				// 'E'
	NULL,				// 'F'
	NULL,				// 'G'
	NULL,				// 'H'
	NULL,				// 'I'
	NULL,				// 'J'
	NULL,				// 'K'
	NULL,				// 'L'
	NULL,				// 'M'
	numConversion,		// 'N'
	NULL,				// 'O'
	NULL,				// 'P'
	NULL,				// 'Q'
	NULL,				// 'R'
	stringConversion,	// 'S'
	NULL,				// 'T'
	NULL,				// 'U'
	NULL,				// 'V'
	NULL,				// 'W'
	integerConversion,	// 'X'
	NULL,				// 'Y'
	NULL,				// 'Z'
	NULL,				// '['
	NULL,				// '\'
	NULL,				// ']'
	NULL,				// '^'
	NULL,				// '_'
	NULL,				// '`'
	NULL,				// 'a'
	integerConversion,	// 'b'
	NULL,				// 'c'
	integerConversion,	// 'd'
	numConversion,		// 'e'
	numConversion,		// 'f'
	numConversion,		// 'g'
	NULL,				// 'h'
	NULL,				// 'i'
	NULL,				// 'j'
	NULL,				// 'k'
	NULL,				// 'l'
	NULL,				// 'm'
	NULL,				// 'n'
	integerConversion,	// 'o'
	NULL,				// 'p'
	NULL,				// 'q'
	NULL,				// 'r'
	charPtrConversion,	// 's'
	NULL,				// 't'
	integerConversion,	// 'u'
	NULL,				// 'v'
	NULL,				// 'w'
	integerConversion,	// 'x'
	NULL,				// 'y'
	NULL,				// 'z'
	NULL,				// '{'
	NULL,				// '|'
	NULL,				// '}'
	NULL,				// '~'
};

#define fpArraySize		int(sizeof(fpArray) / sizeof(fpArray[0]))

static ConvertFunction functionPointer(int c)
{
	c -= ' ';
	return c >= 0 && c < fpArraySize ? fpArray[c] : NULL;
}

void addConvertFunction(int c, ConvertFunction fp)
{
	c -= ' ';
	assert(c >= 0 && c < fpArraySize);
	fpArray[c] = fp;
}


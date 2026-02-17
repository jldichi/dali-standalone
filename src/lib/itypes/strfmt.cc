/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strfmt.cc,v 1.7 2004/09/29 16:00:38 damianm Exp $
*
* DESCRIPTION: String general displaying formats.
*
*********************************************************************/

#include <string.h>

#include <ifound.h>
#include <ifound/stdc.h>
#include <ifound/str.h>
#include <ifound/chset.h>
#include <ifound/priv/tstmask.h>
#include <ifound/priv/itypesp.h>
extern "C" UnChar TstChrMask(UnChar chr, char mask, char* value);
String &String::rTrim()
{
	if (!p->notEmpty())
		return *this;

	UnShort plen = p->len();

	char *p_end = p->s() + plen - 1;

	while (plen && isspace(*p_end--))
		plen--;

	if (plen != p->len()) {
		realloc(plen);
		if (plen > 0) {
			p->len() = plen;
			p->s()[plen] = '\0';
		} else {
			p->destroy();
			p = NULL;
		}
	}

	return *this;
}

String &String::lTrim()
{
	if (!p->notEmpty())
		return *this;

	UnShort plen = p->len();
	char *pp = p->s();

	while (isspace(*pp)) {
		pp++;
		plen--;
	}

	if (plen != p->len()) {
		realloc(plen);
		if (plen > 0) {
			mMove(p->s(), p->s() + p->len() - plen, plen + 1);
			p->len() = plen;
			p->s()[plen] = '\0';
		} else {
			p->destroy();
			p = NULL;
		}
	}

	return *this;
}

String &String::fullTrim()
{
	if (!p->notEmpty())
		return *this;

	rTrim();
	lTrim();

	return *this;
}

String &String::prune()
{
	if (!p->notEmpty())
		return *this;

	fullTrim(); // Get rid of whitespace

	// make sure that if refcount is > 1 the string gets duplicated
	realloc(p->len());

	char *pp = p->s();
	char *aux = p->s();
	int i = 0;

	while (*pp != '\0') {
		if (isspace(*pp)) {
			*pp = ' ';
			if (isspace(*(pp+1))) {
				--p->len();
				mMove(aux+i, pp+1, p->len() - i);
				aux[p->len()] = '\0';     
				pp = aux+i;
				pp[p->len() - i] = '\0';
				continue;
			}
		}
		pp++;
		i++; 
	}

	if (p->len() == 0) {
		p->destroy();
		p = NULL;
	} else
		p->s()[p->len()] = '\0';

	return *this;
}


String &String::encode()
{
	if (!p->notEmpty())
		return *this;

	UnChar *s = (UnChar *) p->s();
	short leng = p->len();
	char *buff = tr_new char[p->len()*2+1];
	register char *pbuff = buff;
	char slash = '\\';

	register UnChar c;

	while (leng--) {

		switch (*s) {
		case '\n'  : *pbuff++ = slash; c = 'n';  break;
		case '\f'  : *pbuff++ = slash; c = 'f';  break;
		case '\r'  : *pbuff++ = slash; c = 'r';  break;
		case '\t'  : *pbuff++ = slash; c = 't';  break;
		case '\v'  : *pbuff++ = slash; c = 'v';  break;
		case '\b'  : *pbuff++ = slash; c = 'b';  break;
		case '\033': *pbuff++ = slash; c = 'e';  break;
		case '\\'  : *pbuff++ = slash; c = '\\'; break;
		case COD_ZERO  : *pbuff++ = slash; c = '0'; break;
		default    : c = *s;   break;
		}
		s++;
		*pbuff++ = c;
	}
	*pbuff = '\0';
	*this = buff;
	tr_delete_vec(buff);
	return *this;
}


String &String::decode()
{
	if (!p->notEmpty())
		return *this;

	char *s = p->s();
	short leng = p->len();
	char *buff = tr_new char[p->len()+1];
	register char *pbuff = buff;

	register UnChar c, i, base, dig;

	while (leng--) {

		base = 8;

		switch (*s) {
		case '^':			// control characters
			c = *++s;
			c = UnChar(toUpperCase(char(c)) & 037);
			if (c == '\0')
				c = COD_ZERO;
			break;

		case '\\':			// control characters or numbers
			switch (toLowerCase((char) *++s)) {
			case 'n': c = '\n'; break; // newline
			case 'f': c = '\f'; break; // formfeed
			case 'r': c = '\r'; break; // carriage return
			case 't': c = '\t'; break; // horizontal tab
			case 'v': c = '\v'; break; // vertical tab
			case 'b': c = '\b'; break; // backspace
			case 'e': c = '\033'; break; // escape

			case 'x' :	// hexadecimal number
			case 'X' :
				s++;
				for (c = i = 0; i < 2; s++, i++) {
					if (*s >= '0' && *s <= '9') 
						dig = *s - '0';
					else if (*s >= 'a' && *s <= 'f') 
						dig= *s-'a'+10;
					else if (*s >= 'A' && *s <= 'F') 
						dig= *s-'A'+10;
					else 
						break;
					c = c * 16 + dig;
				}
				if (c == '\0')
					c = COD_ZERO;
				s--;
				break;
			
			case '\\' :
				c = *s;
				break;

			case 'd' :	// decimal number
			case 'D' :
				s++;
				base = 10;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':	// octal
				for (c = i = 0; i < 3 && *s >='0' && *s <='9'; s++, i++)
					c = c * base + *s - '0';
				if (c == '\0')
					c = COD_ZERO;
				s--;
				break;

			default:	// leave character as is
				c = *s;
				break;
			}
			break;

		default:
			c = *s;
		}
		s++;
		*pbuff++ = c;
	}
	*pbuff = '\0';
	*this = buff;
	tr_delete_vec(buff);
	return *this;
}

String &String::compileMask()
{
	static char omask[MAX_MASK_LENGTH]	= { '\0' };
	static char tstmask[MAX_MASK_LENGTH]= { '\0' };

	char *buff = p->notEmpty() ? p->s() : NULL;
	
	if (::compileMask(buff, tstmask, omask) == ERR)
		*this = NULL_STRING;
	else	
  		*this = String("\1") + String(omask) + String("\1") + String(tstmask);

	return *this;
}

// "hellohello" -= "he" -----> "llollo"
String &String::operator-=(const String &st)
{
	*this = replace(st, NULL_STRING);
	return *this;
}

String String::replace(const String &search, const String &repl) const
{
	String aux;

	StrCursor sc(*this, search);
	aux = sc.value();
	++sc;
	
	for ( ; sc.ok() ; ++sc) 
		aux += repl + sc.value();
		
	return aux;
}

String String::replaceArgs(const String args[]) const
{
	if (!p->notEmpty())
		return NULL_STRING;
		
	String result;
	
	for (const char *cp = p->s(); *cp != '\0'; cp++) {
		if (*cp != '#')
			result += *cp;
		else if (cp[1] == '#')
			result += *++cp;
		else if (isdigit(cp[1]))
			result += args[*++cp - '0'];
	}
	return result;
}

String String::format(String omask) const
{
	// compile the mask if it isn't already formatted
	if (omask[0] != '\1') omask.compileMask();

	// calculate omask length
	register char *pmask = omask.p->s() + 1;
	char *msepar = strchr(pmask, '\1');
	assert(msepar != NULL);
	int leng = msepar - pmask;

	// Create Output string
	String ret = String(leng, ' ');
	char *pret = ret.p->s();

	// Get input buffer
	register char *pinput = p->notEmpty() ? p->s() : NULL;
	
	// processMask formats the input string but only uses the 
	// ouput mask, I have to patch the pmask because it has
	// ouput and input mask.
	*msepar = '\0';
	processMask(pret, pmask, pinput);
	*msepar++ = '\1';	

	int i=0, j=0; 
	while (i < leng) {
		if ( pmask[i+1] == '_' )
			pret[i] = TstChrMask(pret[i], msepar[j++], "");
		i++;
	}
	
	ret.p->len() = strlen(pret);
	assert(ret.p->len() <= leng);
	
	return ret;
}



String String::center(int width) const
{
	int spaces = width - length();

	if (spaces <= 0)
		return *this;

	return String(spaces/2, ' ') + *this + String(spaces - spaces /2, ' ');
}
// end of strfmt

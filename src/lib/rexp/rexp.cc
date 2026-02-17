/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rexp.cc,v 1.7 2008/07/04 21:40:39 hardaiz Exp $
*
* DESCRIPTION
* Implementation of Regular Expression class
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <string.h>

#include <ifound/rexp.h>
#include <ifound/stdc.h>
#include <ifound/priv/itypesp.h>

char RegExp::compile_buf[C_BUFF_SIZE];

void RegExp::newrexp(const String &re)
{
	idCheck();

	if (re != NULL_STRING)
		re_pattern = re;
	else
		re_pattern = limited ? "*" : ".*";

	re_length = 0;

	char *re_end;
	int c_len;

	search_s = NULL_STRING;
	state = 0;		// initially we're in good state
	sp = re_pattern.p->s();
	re_end = compile(compile_buf, &compile_buf[C_BUFF_SIZE], '\0');

	if (state == 0) {
		c_re = tr_new char[c_len = re_end - compile_buf];
		mCopy(c_re, compile_buf, c_len);
	} else
		c_re = NULL;

	hasOffset = false;

//albertoi
	loc1 = loc2 = NULL;
}

RegExp::RegExp(bool l)
{
	idStart();

	limited = l;
	newrexp(NULL_STRING);
}

RegExp::RegExp(const String &re, bool l)
{
	idStart();

	limited = l;
	newrexp(re);
}

RegExp::RegExp(const char *re, bool l)
{
	idStart();

	limited = l;
	newrexp(re);
}

RegExp::RegExp(const RegExp &re, bool l)
{
	idStart();

	limited = l;
	c_re = NULL;
	*this = re;
}

RegExp::~RegExp()
{
	idCheck();
	if (c_re != NULL)
		tr_delete_vec(c_re);
	idStop();
}

const RegExp &RegExp::operator=(const RegExp &re)
{
	idCheck();
	if (this != &re) {
		if (c_re != NULL)
			tr_delete_vec(c_re);
		newrexp(re.re_pattern);
	}
	return *this;
}

const RegExp &RegExp::operator=(const String &re)
{
	idCheck();
	if (c_re != NULL)
		tr_delete_vec(c_re);
	newrexp(re);
	return *this;
}

RegExp::operator void*()
{
	idCheck();
	return state ? NULL : this;
}

bool RegExp::match(const String &ss, Int offset)
{
	idCheck();
	if (!*this)
		return false;

	Int len;

	// If we receive an offset off the bounds of the string
	// return a no match.
	if (offset > (len = ss.length()))
		return false;

	Int pos = search(ss, offset);

	/*
		We have a match if what we searched for was found at the first
		position and it has the same length as the target string.
	*/
	if (pos == offset)
		return (ss.length()-offset == re_length);
	else
		return false;
}

Int RegExp::backSearch(const String &ss, Int offset)
{
	idCheck();
	if (offset > ss.length())
		offset = ss.length();

	if (offset >= 0) {
		// Make a local copy of the string
		search_s = ss;
		char *searchBuff = search_s != NULL_STRING ? search_s.p->s() : NULL;
		if (backStep(searchBuff, offset, c_re)) {
			re_length = loc2 - loc1;
			return loc1 - searchBuff;
		}
		re_length = 0;
	}

	// not found
	return ERR;
}

Int RegExp::search(const String &ss, Int offset)
{
	idCheck();
	if (offset < 0)
		offset = 0;

	if (offset <= ss.length()) {

		// Make a local copy of the string
		search_s = ss(offset, ss.length()-offset);
		hasOffset = (offset > 0);
		char *searchBuff = search_s != NULL_STRING ? search_s.p->s() : NULL;
		if (step(searchBuff, c_re)) {
			re_length = loc2 - loc1;
			return offset + loc1 - searchBuff;
		}
		re_length = 0;
	}

	// not found
	return ERR;
}

Int RegExp::length() const
{
	idCheck();
	return re_length;
}

#define BACKSLASH '\\'

static Int slashOrAmpersand(const String &str, Int i)
{
	if (!str) return ERR;
	const char *start = toCharPtr(str);
	const char *p = start + i;
	while (*p && *p != BACKSLASH && *p != '&')
		++p;
	return *p ? p - start : ERR;
}

String RegExp::expand(const String &str) const
{
	idCheck();
	if (!*this)
		return NULL_STRING;

	String newstr = NULL_STRING;
	Int i = 0;
	char temp[2];
	Int buf, pos;
	temp[1] = '\0';

	while ((pos = slashOrAmpersand(str, i)) != ERR) {
		if (pos != i) {
			newstr += str(i, pos-i);
			i = pos;
		}
		if (str[pos] == BACKSLASH) {
			*temp = str[pos+1];
			switch (*temp) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if ((buf = (*temp) - '1') < nbra)
					newstr += String(braslist[buf],
							braelist[buf]-braslist[buf]);
				break;
			case 't':
				newstr += "\t";
				break;
			case 'n':
				newstr += "\n";
				break;
			case '\0':
				break;
			default:
				newstr += temp;
				break;
			}
			++i;
		} else
			newstr += search_s(loc1 - search_s.p->s(), re_length);
		++i;
	}

	Int lenstr = str.length();
	if (i < lenstr)
		newstr += str(i, lenstr-i);

	return newstr;
}

// macros used to get the chars of the RE

// opening parenthesis
#define	CBRA	2
// precedes normal characters
#define	CCHR	4
// any character
#define	CDOT	8
// characters represented in 16 bytes (only ASCII)
#define	CCL		12
// characters represented in 32 bytes (full 256)
#define	CXCL	16

// end of line ('$')
#define	CDOL	20
// end of file
#define	CCEOF	22
// closing parenthesis
#define	CKET	24
#define	CBACK	36
// not (in brackets)
#define NCCL	40

#define	STAR	01
#define RNGE	03

/*
	PLACE turns on the bit corresponding to the character.

	ISTHERE returns true if the character has been placed.

	These macros are used to map one bit to each of the either 128 or
	256 characters, indicating wether the character is or not inside
	the brackets.
*/
#define PLACE(c)	ep[c >> 3] |= bittab[c & 07]
#define ISTHERE(c)	(ep[c >> 3] & bittab[c & 07])
#define ecmp(s1, s2, n)	(!strncmp(s1, s2, n))

static char bittab[] = { 1, 2, 4, 8, 16, 32, 64, 128 };

#define	GETC()				(*sp++)
#define	PEEKC()				(*sp)
#define	UNGETC(c)			(--sp)
#define	RETURN(c)			return c
#define	RE_ERROR(c)			re_error(c)

bool RegExp::hasMetaChars() const
{
	/*
		If the circumflex was used, it means it has metacharacters,
		except when the regexp is limited since it will always be
		forced to have the circumflex.
	*/

	idCheck();

	if (circf && !limited)
		return true;
	else {
		char* p = c_re;
		while (*p != CCEOF) {
			if (*p++ != CCHR)
				return true;
			p++;
		}
		return false;
	}
}

WideChar RegExp::firstChar() const
{
	idCheck();

	if (c_re[0] == CCHR)
		return c_re[1];

	return '\0';
}

char *RegExp::compile(char *ep, char *endbuf, int seof)
{
	idCheck();

	register int c;
	register int eof = seof;
	char *lastep = sp;
	int cclcnt;
	char bracket[NBRA], *bracketp;
	int closed;
	int neg;
	int lc;
	int i, cflg;
	int iflag;			/* used for non-ascii characters in brackets */

	lastep = 0;

	if ((c = GETC()) == eof || c == '\n') {
		if (c == '\n') {
			UNGETC(c);
			//nodelim = 1;
		}
		//if (*ep == 0 && !sed)
		if (*ep == 0)
			return RE_ERROR(41);
		RETURN(ep);
	}

	bracketp = bracket;
	circf = closed = nbra = 0;

	if (limited)  {
		circf++;
		UNGETC(c);
	} else {
		if (c == '^')
			circf++;
		else
			UNGETC(c);
	}

	forever {
		if (ep >= endbuf)
			return RE_ERROR(50);
		c = GETC();
		if (c != '*' && ((c != '\\') ||
				(PEEKC() != '{')))
			lastep = ep;
		if (c == eof) {
			*ep++ = CCEOF;
			if (bracketp != bracket)
				return RE_ERROR(42);
			RETURN(ep);
		}
		switch(c) {

		case '.':
			if (!limited) {
				*ep++ = CDOT;
				continue;
			}
			goto defchar;

		case '?':
			if ( limited) {
				*ep++ = CDOT;
				continue;
			}
			goto defchar;

		case '\n':
			//if (!sed) {
			UNGETC(c);
			*ep++ = CCEOF;
			//nodelim = 1;
			if (bracketp != bracket)
				return RE_ERROR(42);
			RETURN(ep);
			//} else return RE_ERROR(36);
		case '*':
			if (limited) {
				*ep++ = CDOT | STAR;
				continue;
			}
			if (lastep == 0 || *lastep == CBRA || *lastep == CKET)
				goto defchar;
			*lastep |= STAR;
			continue;

		case '$':
			if (limited ||
					(PEEKC() != eof &&
					PEEKC() != '\n'))
				goto defchar;
			*ep++ = CDOL;
			continue;

		case '[':
			/*
				are there 16 bytes available?
			*/
			if (&ep[17] >= endbuf)
				return RE_ERROR(50);

			*ep++ = CCL;
			lc = 0;
			for(i = 0; i < 16; i++)
				ep[i] = 0;

			neg = 0;
			if ((c = GETC()) == '^') {
				neg = 1;
				c = GETC();
			}
			iflag = 1;
			do {
				c &= 0377;
				if (c == eof || c == '\n')
					return RE_ERROR(49);
				if ((c & 0200) && iflag) {
					iflag = 0;
					/*
						now need 32 bytes (16 more)
					*/
					if (&ep[32] >= endbuf)
						return RE_ERROR(50);
					ep[-1] = CXCL;
					for (i = 16; i < 32; i++)
						ep[i] = 0;
				}
				if (c == '-' && lc != 0) {
					if ((c = GETC()) == ']') {
						PLACE('-');
						break;
					}
					if ((c & 0200) && iflag) {
						iflag = 0;
						/*
							now need 32 bytes (16 more)
						*/
						if (&ep[32] >= endbuf)
							return RE_ERROR(50);
						ep[-1] = CXCL;
						for(i = 16; i < 32; i++)
							ep[i] = 0;
					}
					while (lc < c ) {
						PLACE(lc);
						lc++;
					}
				}
				lc = c;
				PLACE(c);
			} while ((c = GETC()) != ']');
			
			if (iflag)
				iflag = 16;
			else
				iflag = 32;
			
			if (neg) {
				if (iflag == 32) {
					for(cclcnt = 0; cclcnt < iflag; cclcnt++)
						ep[cclcnt] ^= 0377;
					ep[0] &= 0376;
				} else {
					ep[-1] = NCCL;
					/* make nulls match so test fails */
					ep[0] |= 01;
				}
			}

			ep += iflag;

			continue;

		case '\\':
			switch(c = GETC()) {

			case '(':
				if (nbra >= NBRA)
					return RE_ERROR(43);
				*bracketp++ = nbra;
				*ep++ = CBRA;
				*ep++ = nbra++;
				continue;

			case ')':
				if (bracketp <= bracket) 
					return RE_ERROR(42);
				*ep++ = CKET;
				*ep++ = *--bracketp;
					closed++;
					continue;

				case '{':
					if (lastep == (char *) 0)
						goto defchar;
					*lastep |= RNGE;
					cflg = 0;
				nlim:
					c = GETC();
					i = 0;
					do {
						if ('0' <= c && c <= '9')
							i = 10 * i + c - '0';
						else
							return RE_ERROR(16);
					} while (((c = GETC()) != '\\') && (c != ','));
					if (i >= 255)
						return RE_ERROR(11);
					*ep++ = i;
					if (c == ',') {
						if (cflg++)
							return RE_ERROR(44);
						if ((c = GETC()) == '\\')
							*ep++ = 255;
						else {
							UNGETC(c);
							goto nlim;
							/* get 2'nd number */
						}
					}
					if (GETC() != '}')
						return RE_ERROR(45);
					if (!cflg)	/* one number */
						*ep++ = i;
					else if ((ep[-1] & 0377) < (ep[-2] & 0377))
						return RE_ERROR(46);
					continue;

			case '\n':
				return RE_ERROR(36);

			case 'n':
				c = '\n';
				goto defchar;

			default:
				if (c >= '1' && c <= '9') {
					if ((c -= '1') >= closed)
						return RE_ERROR(25);
					*ep++ = CBACK;
					*ep++ = c;
					continue;
				}
			}
	/* Drop through to default to use \ to turn off special chars */

		defchar:
		default:
			lastep = ep;
			*ep++ = CCHR;
			*ep++ = c;
		}
	}
	return ep;
}

int RegExp::step(register char *p1, register char *p2)
{
	idCheck();

	if (p1 == NULL) return false;

	register char c;

	if (circf) {
		loc1 = p1;
		/*
			When specifying a starting offset, if circf was used,
			we are shure it won't match.
		*/
		if (hasOffset)
			return false;

		return advance(p1, p2);
	}

	/* fast check for first character */
	if (*p2 == CCHR) {
		c = p2[1];
		do {

			if (*p1 != c)
				continue;
			if (advance(p1, p2)) {
				loc1 = p1;
				return 1;
			}
		} while (*p1++);
		return 0;
	}

	/* regular algorithm */
	do {
		if (advance(p1, p2)) {
			loc1 = p1;
			return(1);
		}
	} while (*p1++);
	return 0;
}

int RegExp::backStep(register char *p1, Int len, register char *p2)
{
	idCheck();

	register char c;
	char *start = p1;

//albertoi
	if (p1 == NULL) return 0;
	if (p2 == NULL) return 0;
//albertoi	

	if (circf) {
		loc1 = p1;
		return(advance(p1, p2));
	} else
		p1 += len;

	/* fast check for first character */
	if (*p2 == CCHR) {
		c = p2[1];
		do {
			if (*p1 != c)
				continue;
			if (advance(p1, p2)) {
				loc1 = p1;
				return (1);
			}
		} while (--p1 >= start);
		return(0);
	}
		/* regular algorithm */
	do {
		if (advance(p1, p2)) {
			loc1 = p1;
			return(1);
		}
	} while (--p1 >= start);
	return(0);
}

int RegExp::advance(register char *lp, register char *ep)
{
	idCheck();
	if (lp == NULL) return 0;

	register char *curlp;
	int c;
	char *bbeg, *locs = NULL; 
	register char neg;
	int ct;

	forever {
		neg = 0;
		switch(*ep++) {

		case CCHR:
			if (*ep++ == *lp++)
				continue;
			return(0);
	
		case CDOT:
			if (*lp++)
				continue;
			return(0);
	
		case CDOL:
			if (*lp == 0)
				continue;
			return(0);
	
		case CCEOF:
			loc2 = lp;
			return(1);
	
		case CXCL: 
			c = (unsigned char)*lp++;
			if (ISTHERE(c)) {
				ep += 32;
				continue;
			}
			return(0);
		
		case NCCL:	
			neg = 1;

		case CCL: 
			c = *lp++;
			if (((c & 0200) == 0 && ISTHERE(c)) ^ neg) {
				ep += 16;
				continue;
			}
			return(0);
		
		case CBRA:
			braslist[*ep++] = lp;
			continue;
	
		case CKET:
			braelist[*ep++] = lp;
			continue;
	
		case CCHR | RNGE:
			c = *ep++;
			getrnge(ep);
			while (low--)
				if (*lp++ != c)
					return(0);
			curlp = lp;
			while (size--) 
				if (*lp++ != c)
					break;
			if (size < 0)
				lp++;
			ep += 2;
			goto star;
	
		case CDOT | RNGE:
			getrnge(ep);
			while (low--)
				if (*lp++ == '\0')
					return(0);
			curlp = lp;
			while (size--)
				if (*lp++ == '\0')
					break;
			if (size < 0)
				lp++;
			ep += 2;
			goto star;
	
		case CXCL | RNGE:
			getrnge(ep + 32);
			while (low--) {
				c = (unsigned char)*lp++;
				if (!ISTHERE(c))
					return(0);
			}
			curlp = lp;
			while (size--) {
				c = (unsigned char)*lp++;
				if (!ISTHERE(c))
					break;
			}
			if (size < 0)
				lp++;
			ep += 34;		/* 32 + 2 */
			goto star;
		
		case NCCL | RNGE:
			neg = 1;
		
		case CCL | RNGE:
			getrnge(ep + 16);
			while (low--) {
				c = *lp++;
				if (((c & 0200) || !ISTHERE(c)) ^ neg)
					return(0);
			}
			curlp = lp;
			while (size--) {
				c = *lp++;
				if (((c & 0200) || !ISTHERE(c)) ^ neg)
					break;
			}
			if (size < 0)
				lp++;
			ep += 18; 		/* 16 + 2 */
			goto star;
	
		case CBACK:
			bbeg = braslist[*ep];
			ct = braelist[*ep++] - bbeg;
	
			if (ecmp(bbeg, lp, ct)) {
				lp += ct;
				continue;
			}
			return(0);
	
		case CBACK | STAR:
			bbeg = braslist[*ep];
			ct = braelist[*ep++] - bbeg;
			curlp = lp;
			while (ecmp(bbeg, lp, ct))
				lp += ct;
	
			while (lp >= curlp) {
				if (advance(lp, ep))	return(1);
				lp -= ct;
			}
			return(0);
	
	
		case CDOT | STAR:
			curlp = lp;
			while (*lp++)
				;
			goto star;
	
		case CCHR | STAR:
			curlp = lp;
			while (*lp++ == *ep)
				;
			ep++;
			goto star;
	
		case CXCL | STAR:
			curlp = lp;
			do {
				c = (unsigned char)*lp++;
			} while (ISTHERE(c));
			ep += 32;
			goto star;
		
		case NCCL | STAR:
			neg = 1;

		case CCL | STAR:
			curlp = lp;
			do {
				c = *lp++;
			} while (((c & 0200) == 0 && ISTHERE(c)) ^ neg);
			ep += 16;
			goto star;
	
		star:
			do {
				if (--lp == locs)
					break;
				if (advance(lp, ep))
					return(1);
			} while (lp > curlp);
			return(0);

		}
	}
}

void RegExp::getrnge(register char *str)
{
	idCheck();

	low = *str++ & 0377;
	size = ((*str & 0377) == 255) ? 20000 : (*str & 0377) - low;
}

char *RegExp::re_error(int err_code)
{
	idCheck();

	state = err_code;
	return NULL;
}


String noMetaChars(const String &str)
{
	String temp = NULL_STRING;
	Int i;
	for (i = 0; i < str.length(); i++) {
		if (isMetaChar(str[i]))
			temp += '\\';
		temp += str[i];
	}
	return temp;
}

// end of fregexp

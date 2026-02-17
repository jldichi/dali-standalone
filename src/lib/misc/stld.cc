/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: stld.cc,v 1.4 2008/07/04 21:40:03 hardaiz Exp $
*
* DESCRIPTION
* Load and store data types in cpu independent format.
*********************************************************************/

#include <string.h>
#include <ideafix.h>
#include <cfix.h>
#include <ifound/priv/gfuncs.h>

#if !IDEAWIN || IDEADLL
long ldlong(register const void *p)
{
	int b0, b1, b2, b3;
	long aux;	
	
	b0 = ldint(p);
	b1 = ldint((const char *)p+2);
	b2 = ldint((const char *)p+4);
	b3 = ldint((const char *)p+6);
	
	aux = (long) b0 << 48 | (long) (b1 & 0xFFFF) << 32 | (long) (b2 & 0xFFFF) << 16 | b3 & 0xFFFF;
	//fprintf(stderr,"aux: %ld|%lX\n", aux, aux);
	return aux;
//	return (long) ldint(p) << 16 | lduint((const char *) p + 2);
}

void stlong(register long l, register void *p)
{
	stint((int)(l>>48), p);
	stint((int)(l>>32), p+2);
	stint((int)(l>>16), p+4);
	stint((int)(l&0xFFFF), (char *) p + 6);
}

void stInt(Int l, char *&p)
{
	stint((int)((l>>16) ^ 0x8000), p);
	stint((int)(l & 0xFFFF), p + 2);
	p += sizeof(Int);
}

Int ldInt(const char *&p)
{
	Int ret = Int(((ldint(p) ^ 0x8000) << 16) | lduint(p + 2));
	p += sizeof(Int);
	return ret;
}

double lddbl(register const void *p)
{
	double aux;
	mCopy((char *) &aux, p, sizeof(double));
	return aux;
}

void stdbl(double d, register void *p)
{
	mCopy(p, (char *) &d, sizeof(double));
}
#endif         
        
UnShort ldUnShort(FILE *fp)
{
	UnShort s;
	fread(&s, sizeof(UnShort), 1, fp);
	return lduint(&s);
}

short ldShort(FILE *fp)
{
	short s;
	fread(&s, sizeof(short), 1, fp);
	return ldint(&s);
}

Int ldInt(FILE *fp)
{
	Int l;
	fread(&l, sizeof(Int), 1, fp);
	return ldlong(&l);
}

String ldString(FILE *fp)
{
	UnShort len;
	len = ldUnShort(fp);

	if (len == UnShort(0xffff))
		return HIGH_VALUE;

	if (len == 0)
		return NULL_VALUE;

	String s;
	char *p = s.getBuffer(len);
	fread(p, SizeT(len), 1, fp);
	return s;
}

bool ldBool(FILE *fp)
{
	// use signed char to make sure BOOL_NULL can be represented
	return bool(ldChar(fp));
}

signed char ldChar(FILE *fp)
{
	// use signed char to make sure BOOL_NULL can be represented
	signed char c;
	fread(&c, sizeof(signed char), 1, fp);
	return c;
}

Num ldNum(FILE *fp)
{
	Num n;
	Int i;
	for (i = 0; i < NUM_SHORTS; ++i)
		n.s[i] = ldShort(fp);
	n.exp = ldChar(fp);
	n.len = ldChar(fp);
	return n;
}

void stString(FILE *fp, const String &str)
{
	if (str == HIGH_VALUE)
		stUnShort(fp, 0xffff);
	else if (str == NULL_VALUE)
		stUnShort(fp, 0);
	else {
		stUnShort(fp, UnShort(str.length()));
		char *p = (char *) toCharPtr(str);
		fwrite(p, SizeT(str.length()), 1, fp);
	}
}

void stInt(FILE *fp, Int n)
{
	char buff[sizeof(Int)];
	stlong(n, buff);
	fwrite(buff, sizeof(Int), 1, fp);
}

void stUnShort(FILE *fp, UnShort n)
{
	char buff[sizeof(UnShort)];
	stuint(n, buff);
	fwrite(buff, sizeof(UnShort), 1, fp);
}

void stShort(FILE *fp, short n)
{
	char buff[sizeof(short)];
	stint(n, buff);
	fwrite(buff, sizeof(short), 1, fp);
}

void stBool(FILE *fp, bool v)
{
	// use signed char to make sure BOOL_NULL can be represented
	stChar(fp, (signed char) v);
}

void stChar(FILE *fp, signed char c)
{
	fwrite(&c, sizeof(signed char), 1, fp);
}

void stNum(FILE *fp, const Num &n)
{
	Int i;
	for (i = 0; i < NUM_SHORTS; ++i)
		stShort(fp, n.s[i]);
	stChar(fp, n.exp);
	stChar(fp, n.len);
}

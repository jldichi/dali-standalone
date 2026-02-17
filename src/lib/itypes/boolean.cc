/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: boolean.cc,v 1.2 2004/09/29 17:20:32 damianm Exp $
*
* DESCRIPTION
* Implementation of Boolean class
*********************************************************************/

#include <string.h>

#include <ifound/itypes.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/stdc.h>
#ifdef IDEAWIN
#include <mswi/iwres.h>
extern "C" char * IwErrMsg(int);
#endif

Boolean::Boolean(const char *sd)
{
	val = (sd == LOW_VALUE || sd == NULL || *sd == '\0' ? BOOL_NULL : 
														 !!toInt(sd));
}


#define BOOL_MASK_SEP	'\n'

const char *Boolean::format(const char *mask) const
{
	// format buffer
	static FmtBuff fBuff = { '\0' };
	char *pf = (char *) fBuff;

	*pf = '\0';
	if (mask == NULL || *mask == '\0') {
		static String *defMask = NULL;
		if (defMask == NULL) {
#ifndef  IDEAWIN
			defMask = tr_new String(libMsg("BOOLMASK"));
#else
			defMask = tr_new String(IwErrMsg(IDS_BOOLMASK));
#endif
		}
		mask = toCharPtr(*defMask);
	}

	if (mask == NULL || *mask == '\0')
		return pf;

	const char *sep = strchr(mask, BOOL_MASK_SEP);
	// we compare for equality to distinguish BOOL_NULL as another case
	if (val == int(true)) {

		if (sep == NULL)
			sep = mask + strlen(mask);

		strncpy(pf, mask, sep - mask);
		pf[sep - mask] = '\0';
		return pf;
	}

	if (sep == NULL)
		return pf;

	mask = sep + 1;
	sep = strchr(mask, BOOL_MASK_SEP);

	if (val == int(false)) {

		if (sep == NULL)
			sep = mask + strlen(mask);

		strncpy(pf, mask, sep - mask);
		pf[sep - mask] = '\0';
		return pf;
	}

	if (sep == NULL)
		return pf;

	mask = sep + 1;
	strcpy(pf, mask);
	return pf;
}

Boolean toBoolean(Int i)
{ 
	Boolean d; 
	d.val = (i < 0 ? BOOL_NULL : !!i);
	return d; 
}

Boolean toBoolean(const Num &n)
{ 
	return toBoolean(toInt(n));
}

const char *toCharPtr(Boolean t)
{
	return t.val == BOOL_NULL ? LOW_VALUE : toCharPtr(t.val);
}

Boolean &Boolean::operator=(const Boolean &b)
{
	if (this != &b)
		val = b.val;
	return *this;
}

void Boolean::store(char *&buff) const
{
	stlong(long(val), buff);
	buff += sizeof(long);
}

Boolean Boolean::load(const char *&buff)
{
	long l = ldlong(buff);
	buff += sizeof(long);
	return Boolean(bool(l));	
}

Int Boolean::storeLength()
{
	return sizeof(long);	
}

// end of boolean

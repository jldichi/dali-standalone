/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: timemisc.cc,v 1.2 2004/09/29 16:20:31 damianm Exp $
*
* DESCRIPTION
* Implementation of time class
*********************************************************************/

#include <string.h>

#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/priv/timedefs.h>

// useful constant: seconds between 1/1/1970 and 1/1/1984
#ifdef TIME_FROM68
#define TIME_OFFSET	504921600L
#else
#define TIME_OFFSET	441763200L
#endif

// map an osTime value to IdeaFix time units

LTIME _ostime_to_time(long s)
{
	return s - TIME_OFFSET;
}

long _time_to_ostime(LTIME s)
{
	return s + TIME_OFFSET;
}

void _time_to_dhms(LTIME itime, long *d, long *h, long *m, long *s)
{
	// Consider time boundaries

	if (itime == INT_MIN) {
		*d = *h = *m = *s = INT_MIN;
		return;
	}

	if (itime == INT_MAX) {
		*d = *h = *m = *s = INT_MAX;
		return;
	}

	*d = itime / SECONDS_DAY;
	itime %= SECONDS_DAY;
	*h = itime / SECONDS_HOUR;
	itime %= SECONDS_HOUR;
	*m = itime / SECONDS_MINUTE;
	*s = itime % SECONDS_MINUTE;
}

long _dhms_to_time(long d, long h, long m, long s)
{
	double aux;
	
	aux = d*SECONDS_DAY;
	if (aux >= double(INT_MAX)) {
		return INT_MAX;
	}
	if (aux <= double((long) INT_MIN)) {
		return INT_MIN;
	}

	aux = h*SECONDS_HOUR;
	if (aux >= double(INT_MAX)) {
		return INT_MAX;
	}
	if (aux <= double((long) INT_MIN)) {
		return INT_MIN;
	}

	aux = m*SECONDS_HOUR;
	if (aux >= double(INT_MAX)) {
		return INT_MAX;
	}
	if (aux <= double((long) INT_MIN)) {
		return INT_MIN;
	}

	if (s >= INT_MAX) {
		return (long) INT_MAX;
	}
	if (s <= (long) INT_MIN) {
		return INT_MIN;
	}

	return d * SECONDS_DAY + h * SECONDS_HOUR + m * SECONDS_MINUTE + s;
}

LTIME _str_to_time(const char *sd)
{
	// handle special cases

	if (sd == HIGH_VALUE) return INT_MAX;
	if (sd == LOW_VALUE || *sd == '\0' ) return INT_MIN;
	if (!strcmp(sd, NOW)) return _ostime_to_time(_osTime());

	// Parse a time string with the following format
	// hhmm
	// hhmmss
	// dddddhhmmss, or separated by :

	Int vals[4], ndays;
	int disp;
	char cb[3];
	const char *p = sd;

	if (strlen(p) >= (sizeof("dddddhhmmss") - 1)) {	// contains year
		char cdays[6];
		memcpy(cdays, p, 5);
		cdays[5] = '\0';
		ndays = toInt(cdays);

		if (p[5] == ':') p += 6;
		else p += 5;
	}
	else ndays = 0;
		

	if (p[2] == ':') disp = 1;
	else disp = 0;

	int j;
	for (j = 0; j < 3; j++) {
		cb[0] = *p++;
		cb[1] = *p++;
		cb[2] = '\0';
		vals[j] = toInt(cb);
		if (*p == '\0') break;
		if (disp) p++;	// skip the :'s
	}

	switch (j) {
	case 1:
		return _dhms_to_time(ndays, vals[0], vals[1], 0);
	case 2:
		return _dhms_to_time(ndays, vals[0], vals[1], vals[2]);
	default:
		return ERR;
	}
}

const char *_time_to_str(LTIME l, char *buff, TimeFmt tfmt)
{
	if (l == INT_MIN) return LOW_VALUE;
	if (l == INT_MAX) return HIGH_VALUE;

	Time t = toTime(l);
	if (tfmt & SECONDS_TFMT)
		sprintf(buff, "%02ld%s%02ld%s%02ld", 
					  t.hour(), tfmt & SEPAR_TFMT ? ":" : "" ,
					  t.minute(), tfmt & SEPAR_TFMT ? ":" : "",
					  t.second());
	else
		sprintf(buff, "%02ld%s%02ld", 
					  t.hour(), tfmt & SEPAR_TFMT ? ":" : "",
					  t.minute());
	return (const char*) buff; 
}

// end of time

/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: datestr.cc,v 1.2 2004/09/29 15:09:20 damianm Exp $
*
* DESCRIPTION
* Format a date/time according to a mask
*********************************************************************/
#include <string.h>

#include <ifound.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>
#include <ifound/chset.h>


String Date::dayOfWeekName() const
{
	recalc_values();
	return String(::dayOfWeekName(dayOfWeek()));
}

String Date::monthName() const
{
	recalc_values();
	return String(::monthName(month() - 1));
}

const char *Date::format(const char *mask)
{ 	
	return form(mask); 
}

String Date::format(const String &mask) const
{ 
	return String(form(toCharPtr(mask))); 
}

const char *Time::format(const char *mask)
{ 
	return toDate(*this).form(mask, itime); 
}

String Time::format(const String &mask) const
{ 
	return String(toDate(*this).form(toCharPtr(mask), itime)); 
}

//	Format the date according to a given mask

/*
	Masks for the year

	YYYYYY	print the full stored year
	YYYY	print the last four digits
	YY		print the last two digits of the year

	Quarter of the year

	QUART	print the quarter of the year in numbers

	Masks for the month 

	MM		print the month in numbers
	MON		print the month in letters, abbreviation
	MONTH	print the month in letters, full length

	Masks for the day

	DD		print the day of the month in numbers
	DN		print the day of the month in numbers (no leading 0)

	Yearday

	YDAY	print the day of the year in numbers

	Weekday

	DY		print the weekday in letters, three letter abbreviation
	DAY		print the weekday in letters, full name

	Week number

	WNUM	print the week number of the year

	Seconds

	SSS		seconds elapsed since 1/1/1984, [0, ~2^31]
	SS		seconds of the minutes, [0, 60)

	Minutes

	MIN		minutes elapsed since 1/1/1984, [0, ~35 10^6]
	MI		minutes of the hour, [0, 60)

	Hours

	HHH		hours elapsed since 1/1/1984, [0, ~600000]
	HH		hours of the day, [0, 24)
	H12		hours of the day, [0, 12)
	AM		replace with am/pm accordingly

	Days

	DDD		days elapsed since	1/1/1984, [0, ~25000]

	All other characters are copied to the output string without
	modification or translation. Masks can be copied to the output
	string without translation by preceeding them with a '\'.

	Note :
		Capitalization is observed in masks that produce a spelled
		out name.
		Example :
		DAY produces "MONDAY",
		Day produces "Monday" and
		day produces "monday".
*/


// Defines for the switch according to the mask found

enum {

// Date masks

	DM_YYYYYY, DM_YYYY, DM_YY,
	DM_QUART,
	DM_MM, DM_MONTH, DM_MON,
	DM_DDD, DM_DD, DM_DN,
	DM_YDAY, DM_DY, DM_DAY,
	DM_WNUM,

// Time masks

	DM_SSS, DM_MIN,
	DM_HHH,
	DM_SS, DM_MI,
	DM_HH, DM_H12, DM_AM
};

/*	If new masks are added, DM_ANYTHING should always point to the
	index of the last mask in the table plus one because it's used
	as upper limit in the mask lookup function
*/

#define	DATE_LIMIT		14
#define	TIME_LIMIT		22

static int DM_ANYTHING = 0;

#define	DM_ALL_UPPER	0	// all name in upper case
#define	DM_ALL_LOWER	1	// all name in lower case
#define DM_UPPER_LOWER	2	// first letter in upper & then lower case
#define DM_NO_ADJUST	3	// do not adjust the case

// Find out the type of the mask and determine it's case mix

static int mask_type(char * &p, int &w_case)
{
	static const char *mask_tab[] = {
		"YYYYYY", "YYYY", "YY", "QUART",
		"MM", "MONTH", "MON", "DDD", "DD", "DN", "YDAY", "DY", "DAY",
		"WNUM", "SSS", "MIN", "HHH", "SS", "MI", "HH", "H12", "AM"
	};

	// look-up for the mask, only if the char is :

	switch (toUpperCase(*p)) {
	case 'Y':
	case 'Q':
	case 'M':
	case 'D':
	case 'W':
	case 'S':
	case 'H':
	case 'A':
		{
			int i;
			for (i = 0; i < DM_ANYTHING; i++) {
				int mask_len = strlen(mask_tab[i]);
				if (!memCmpI(p, mask_tab[i], mask_len) ) {

					// find out the case type of the word

					if (i == DM_MONTH || i == DM_MON ||
						i == DM_DY || i == DM_DAY || i == DM_AM) {

						if (isupper(p[0])) {
							if (isupper(p[1]))
								w_case = DM_ALL_UPPER;
							else
								w_case = DM_UPPER_LOWER;
						} else 
							w_case = DM_ALL_LOWER;
					}
					else w_case = DM_NO_ADJUST;

					p += mask_len;
					break;
				}
			}
			return (i < DM_ANYTHING) ? i : -1;
		}
		break;
	}
	return -1;
}

// Adjust the given word to the required case

static void adjust_case(char *word, int w_case)
{
	switch (w_case) {
	case DM_ALL_UPPER:
		{
			char *p = word;
			while (*p)
				*p++ = toUpperCase(*p);
		}
		break;
	case DM_UPPER_LOWER:
		{
			char *p = word;
			if (p[0] != '\0') {
				*p++ = toUpperCase(*word);
				while (*p)
					*p++ = toLowerCase(*p);
			}
		}
		break;
	case DM_ALL_LOWER:	// strings are in lower case, do nothing
		{
			char *p = word;
			while (*p)
				*p++ = toLowerCase(*p);
		}
		break;
	case DM_NO_ADJUST:	// do nothing
	default :
		break;
	}
}

// Format a date according to a given mask

const char *Date::form(const char *mask, long seconds) const
{
	// format buffer
	static FmtBuff f_buf = { '\0' };
	char *pm = (char*)mask;
	register const char *q;
	register char *opf, *pf = (char*)f_buf;
	int w_case;
	bool trunc_to_three;
	Time t = toTime(seconds);

	if (idate == LONG_MIN) return LOW_VALUE;
	if (idate == LONG_MAX) return HIGH_VALUE;

	recalc_values();

	if (seconds == -1L)
		DM_ANYTHING = DATE_LIMIT;	
	else
		DM_ANYTHING = TIME_LIMIT;

	String tmp;
	while (*pm) {
		trunc_to_three = false;
		switch (mask_type(pm, w_case)) {
		case DM_YYYYYY:
			q = toCharPtr(year(), -6);
			break;
		case DM_YYYY:
			q = toCharPtr(year() % 10000L, -4);
			break;
		case DM_YY:
			q = toCharPtr(year() % 100L, -2);
			break;
		case DM_QUART:
			q = toCharPtr(quarter(), -1);
			break;
		case DM_MM:
			q = toCharPtr(month(), -2);
			break;
		case DM_MONTH:
			tmp = ::monthName(month() - 1);
			q = toCharPtr(tmp);
			break;
		case DM_MON:
			tmp = ::monthName(month() - 1);
			q = toCharPtr(tmp);
			trunc_to_three = true;
			break;
		case DM_DD:
			q = toCharPtr(day(), -2);
			break;
		case DM_DN:
			q = toCharPtr(day());
			break;
		case DM_YDAY:
			q = toCharPtr(dayOfYear(), -3);
			break;
		case DM_DY:
			tmp = ::dayOfWeekName(dayOfWeek());
			q = toCharPtr(tmp);
			trunc_to_three = true;
			break;
		case DM_DAY:
			tmp = ::dayOfWeekName(dayOfWeek());
			q = toCharPtr(tmp);
			break;
		case DM_WNUM:
			q = toCharPtr(weekOfYear(), -3);
			break;

		//	These are the time formatting functions, they only
		//	they only work (make sense) if the seconds parameter is != -1L
			
		case DM_SSS:
			q = toCharPtr(t.seconds(), -10);
			break;
		case DM_MIN:
			q = toCharPtr(t.minutes(), -8);
			break;
		case DM_HHH:
			q = toCharPtr(t.hours(), -6);
			break;
		case DM_DDD:
			q = toCharPtr(t.days(), -5);
			break;
		case DM_SS:
			q = toCharPtr(t.second(), -2);
			break;
		case DM_MI:
			q = toCharPtr(t.minute(), -2);
			break;
		case DM_HH:
			q = toCharPtr(t.hour(), -2);
			break;
		case DM_H12:
			q = toCharPtr(t.hour() > 12 ? t.hour() - 12 : t.hour(), -2);
			break;
		case DM_AM:
			q = (t.hour() > 11) ? "pm" : "am";
			break;
		default:
			// copy the no meaning chars from the mask
			if (*pm == '\\' && *++pm == '\0')
				continue;
			*pf++ = *pm++;
			continue;
		}

		// save old pointer
		opf = pf;

		if (trunc_to_three) {
			// copy just three chars
			for (int i = 0; i < 3; i++)
				*pf++ = *q++;
			*pf = '\0';
		}
		else {
			// copy all the string
			while ((*pf++ = *q++) != '\0')
				;
			pf--;
		}

		adjust_case(opf, w_case);
	}
	*pf = '\0';
	return (char *) f_buf;
}

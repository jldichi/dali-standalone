/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: tmglob.c,v 1.2 1998/04/24 15:33:15 mauro Exp $
*
* DESCRIPTION
* Global variables.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*********************************************************************/

#include <ideafix.h>

/* Public variables defined in this module                          */

/* Default date format is international DD/MM/AA */

DLLDEPEND short _datefmt = DFMT_INTERNAT;

/*
 * (DATE)TODAY_DATE <-------> (string)TODAY_STR
 * (TIME)HOUR_TIME  <-------> (string)HOUR_STR
 */

DLLDEPEND char *TODAY_STR = "TODAY";
DLLDEPEND char *HOUR_STR  = "HOUR";
DLLDEPEND char *DAY_STR  	= "DAY";
DLLDEPEND char *MONTH_STR = "MONTH";
DLLDEPEND char *YEAR_STR	= "YEAR";

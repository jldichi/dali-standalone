/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wisrch.cc,v 1.3 2004/09/29 17:07:30 damianm Exp $
*
* DESCRIPTION
* Descripcion del proposito general del modulo
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*_SearchRegexp	 perform searchs on WiHelp and PopUpMenu functions.
*********************************************************************/

#include <string.h>

#include <ifound.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>

int searchTxt(Int line, bool backward, const char *rxp,
			char *txt, CPFPCPI get_optx,Int flags = 0)
{
#	define nextLine() (backward ? line-- : line++)

	char* txt1 = NULL;
	
	if (rxp == NULL) return 0;                       
	if (!flags)
		flags = REXP_NOCASE|REXP_EMBEDDED;

	bool match = false;

	nextLine();

	while((txt1=(*get_optx)(txt, line))) {
		if((match = RexpMatch((UChar *)rxp,(UChar *)txt1,
							  flags)))
			break;
		nextLine();
	}

	return match ? line : 0;
}

/*
 * This function is used to perform searchs on WiHelp and
 * PopUpMenu functions. It supports nested calls.
 *
 * org_row & org_col: Coordinates of input field.
 * 'line': line to search from.
 * 'garg': argument to ...
 * 'get_optx': function to read text.
 *
 * RETURN: the line where the match was found, or 0 if !success.
 */

/*
 * Give some help about the search operations.
 */
static void search_help(void)
{
	WiMsg(_WiErrmsg(HELPTXT));
}

const char *_SearchRegexp(int org_row, int org_col, int ncols,
						 CPFPCPI, int &cmd)
{
	static char rxp[100] = { '\0' }; // previous reg. expr.

	/* The window label */
	static char label[100] = { '\0' };
	static short lab_len = 0; 
	char buf[100];

	/* Initialization stuff								*/
	if( !lab_len ) 
		(void)sprintf(label,_WiErrmsg(E_LABEL),
							WiKeyHelp(K_HELP,(UChar*)buf));
	else
		if ( (lab_len=(short)strlen(label)) > (short)ncols) 
			ncols = lab_len;

	/* Create the window								*/
	if( WiCreate(WiDefPar(), org_row, org_col, 1, ncols, 
		         SLINE_TYPE | ALL_BORDER, label, A_NORMAL) == ERR ) {
			w_beep();
			return NULL;
	}
	WiWrap(false);

	/* Ask for the regular expression					*/
	(void)strcpy(buf, rxp);
	do {
		WiMoveTo(0,1);
		if( (cmd=WiGetField(TY_STRING, (UChar*)buf, 0, (short)ncols-2, 
							(short)ncols-2, 0, "", ""))==K_HELP ) {
			WiMoveTo(0,ncols);
			search_help();
		}
	} while(cmd == K_HELP);
	if( buf[0] )
		(void) strcpy(rxp, buf);

	/* The END key cancels the search					*/
	if(cmd == K_END) {
		WiDelete(WICURRENT);
		return NULL;
	}

	/* Check if there is a regular expression to search	*/
	if( rxp[0] == 0 ) {
		WiMoveTo(0,1); 
		WiPrintf("\033[5m%s\033[0m", rxp[0] ? rxp : _WiErrmsg(E_NOPAT));
	 	WiRefresh();
		(void)WiGetc();
		WiDelete(WICURRENT);
		return NULL;
	}

	WiDelete(WICURRENT); /* Leave all things OK */

	return rxp;
}

int _HomeSearch(int org_row, int org_col, int ncols, int line,
		char *garg, CPFPCPI get_optx)
{
	int cmd = ERR;
	const char *rxp = _SearchRegexp(org_row, org_col, ncols, get_optx, cmd);

	if (rxp != NULL) {
		line = searchTxt(line, (cmd==K_CURS_UP), rxp, garg, get_optx);
		if(line == 0) w_beep();
	}

	return (rxp != NULL) ? line : 0;
}

fpHomeSearch HomeSearch = _HomeSearch;

void setHomeSearch(fpHomeSearch fp)
{
	HomeSearch = fp;
}

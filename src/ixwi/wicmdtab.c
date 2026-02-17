/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wicmdtab.c,v 1.5 2004/06/25 20:22:25 diegoa Exp $
*
*
* DESCRIPTION:
*   
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#include <ideafix.h>

#ifndef PCLIENT
#include <ideafix/priv/wmcom.h>

/*
	Parameter types in each call to the window manager.

	First column:
		Where the function pointer is stored,
		keep it in NULL.

	Second column (input parameters):

		I = Integer,
		L = Long integer,
		S = Character pointer,
		C = Character.
		NULL = No parameters.
	
	Third column (output parameters):

		same as above.

	Fourth column:

		Wether or not it is remote, keep it in 0, since
		it is not yet implemented.
*/
static struct _wm_commands commands[]={
	{ NULL,	NULL	  , NULL,	0 }, /* DUMMY			*/
	{ NULL,	NULL	  , NULL,	0 }, /* STOP	 		*/	
	{ NULL, "IIIIILLS", "I" ,	0 }, /* WICREATE		*/	
	{ NULL, "I"		  , NULL,	0 }, /* WIDELETE		*/	
	{ NULL,	NULL	  , NULL,	0 }, /* WIDELETEALL		*/	
	{ NULL,	"I"		  , "I" ,	0 }, /* WISWITCHTO		*/ 
	{ NULL,	NULL	  , NULL,	0 }, /* WIREFRESH		*/	
	{ NULL,	NULL	  , NULL,	0 }, /* WIREDRAW		*/ 
	{ NULL,	"LS"	  , NULL,	0 }, /* WISETBORDER		*/ 
	{ NULL,	"I" 	  , NULL,	0 }, /* WISTATUSLINE	*/	
	{ NULL,	NULL	  , "L" ,	0 }, /* WIGETATTR		*/ 	
	{ NULL, "I" 	  , "I" ,	0 }, /* WIORGCOL		*/
	{ NULL,	"I" 	  , "I" ,	0 }, /* WIORGROW		*/	
	{ NULL,	"I" 	  , "I" ,	0 }, /* WILINE			*/	
	{ NULL,	"I" 	  , "I" ,	0 }, /* WICOL			*/
	{ NULL,	"I" 	  , "I" ,	0 }, /* WIWIDTH			*/
	{ NULL,	"I" 	  , "I" ,	0 }, /* WIHEIGHT		*/
	{ NULL,	"II"	  , "I" ,	0 }, /* WIINCHAR		*/
	{ NULL,	"II"	  , "L" ,	0 }, /* WIINATTR		*/
	{ NULL,	NULL	  , "I" ,	0 }, /* WIGETTAB		*/	
	{ NULL,	"I" 	  , NULL,	0 }, /* WISETTAB		*/
	{ NULL,	"I" 	  , "I" ,	0 }, /* WIPARENT		*/
	{ NULL, "S"		  , NULL,	0 }, /* WIFLUSH			*/
	{ NULL, NULL	  , "C" ,	0 }, /* WIGETC			*/
	{ NULL, "S"		  , "S" ,	0 }, /* WIGETS			*/
	{ NULL,	"CS"	  , "S" ,	0 }, /* WIKEYHELP		*/
	{ NULL,	"I" 	  , NULL,	0 }, /* WIINTERRUPTS	*/
	{ NULL,	NULL	  , "I" ,	0 }, /* WICURRENT		*/
	{ NULL,	"LSSS"	  , "I" ,	0 }, /* WICREATEPRO		*/
	{ NULL,	"L"		  , "I" ,	0 }, /* WIDELETEPRO		*/
	{ NULL,	NULL	  , NULL,	0 }, /* WISETTTY		*/
	{ NULL,	NULL	  , NULL,	0 }, /* WIRESETTTY		*/
	{ NULL,	"ISIIIISS", "IS",	0 }, /* WIGETFIELD		*/
	{ NULL,	NULL 	  , NULL,	0 }, /* WIAPLHELP		*/
	{ NULL,	"SS" 	  , NULL,	0 }, /* WISETAPLHELP	*/
	{ NULL,	"I"	  	  , "I" ,	0 }, /* WIPROCESS		*/
	{ NULL,	"I"		  , NULL,	0 }, /* WISETRAW		*/
	{ NULL,	NULL	  , "I",	0 }, /* WIKEYREADY		*/
	{ NULL,	"I"		  , NULL,	0 }, /* WISETSERVICES	*/
	{ NULL,	"I"		  , "I",	0 }, /* WIMOVE			*/
	{ NULL,	"I"		  , "I",	0 }, /* WIRESIZE		*/
	{ NULL,	"S"		  , "I",	0 }, /* WISERVICE		*/
/* +++++ IXSCA +++++ */
	{ NULL,	"I"		  , "LLSSSSS",	0 }, /* WIGETSESSION		*/
	{ NULL,	"LLLSSSSS"	  , "I",	0 }, /* WISETSESSION		*/
/* ----- IXSCA ----- */
};
static int wm_ncmds = sizeof(commands)/sizeof(commands[0]);

struct _wm_commands *_WmCommands(int *n)
{
	if (n != NULL) *n = wm_ncmds;
	return commands;
}

#endif

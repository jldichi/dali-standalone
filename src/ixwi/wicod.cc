/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wicod.cc,v 1.3 2006/02/17 17:23:45 spark Exp $
*
* DESCRIPTION
*
*	 	Use: _EnCodeParameters.
*			 _DecodeParameters.			 
*
*********************************************************************/

#include <string.h>

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/wmcom.h>
#include <ifound/priv/gfuncs.h>

static struct _wm_commands *wm_commands = NULL;

int _EnCodeParameters(int cmd, char *buff, bool input)
{
	register char *pbuff = buff;
	char *par_types;
	union _WmPar *par = wm_par;
	if (wm_commands == NULL)
		wm_commands = _WmCommands(NULL);
	par_types   = input ? wm_commands[cmd].i_types :
						  wm_commands[cmd].o_types ; 
	/* codifico el comando */
	if (input) {
		stint (cmd, pbuff);
		pbuff += sizeof(short);
	}
	
	while (par_types && *par_types) {
		switch (*par_types++) {
		case 'C' :  *pbuff = (char) par->u ; pbuff++; break;
		case 'I' :  stint (par->i, pbuff) ;pbuff += sizeof(short) ;break;
		case 'L' :  stlong(par->l, pbuff) ;pbuff += sizeof(long)  ;break;
		case 'F' :  stdbl (par->f, pbuff) ;pbuff += sizeof(double);break;
		case 'S' : 
			if (par->s == NULL) *pbuff++ = 0;
			else { 
				strcpy(pbuff, (char*)par->s); 
				pbuff += strlen(pbuff)+1;
			}
			break;
		}
		par++;
	}
	return pbuff - buff;
}	

int _DecodeParameters(int *cmd, char *buff, bool input)
{
	int len;
	char *par_types, *pbuff = buff;
	static char str_buff[WM_COM_BUFF_SIZE] = {(char)NULL};
	char *psb = str_buff;
	union _WmPar *par = wm_par;
	if (wm_commands == NULL)
		wm_commands = _WmCommands(NULL);

	/* obtengo el comando */
	if (input) {
		*cmd = GetCmd(pbuff);
		pbuff += sizeof(short);
	}

	par_types  = input ? wm_commands[*cmd].i_types:
					     wm_commands[*cmd].o_types; 

	while (par_types && *par_types) {
		switch (*par_types++) {
		case 'C' : par->u = *pbuff++; break;
		case 'I' : par->i = lduint(pbuff);pbuff += sizeof(short); break;
		case 'L' : par->l = ldlong(pbuff);pbuff += sizeof(long);  break;
		case 'F' : par->f = lddbl (pbuff);pbuff += sizeof(double);break;
		case 'S' : 
			par->s = (UChar *) strcpy(psb, pbuff);
			len = strlen(pbuff) + 1;	
			pbuff += len;
			psb  += len;
			break;
		}
		par++;
	}
	return pbuff - buff;
}	

int GetCmd(char *buff)
{
	return lduint(buff);
}

int _WiComRem(int cmd)
{
	return wm_commands[cmd].wm_rem;
}

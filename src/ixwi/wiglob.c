/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wiglob.c,v 1.5 2006/02/17 17:30:25 spark Exp $
*
*
* DESCRIPTION:
*
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#ifndef PCLIENT

#include <ideafix.h>
#include <ideafix/priv/wmcom.h>


static union _WmPar _wm_par[16] = {{0}};

union _WmPar *wm_par = _wm_par;

FPI ExecWm = (FPI) NULLFP;
int reint = FALSE;
bool sig_rcv = FALSE;


#endif

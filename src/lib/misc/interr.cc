/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: interr.cc,v 1.2 1998/04/24 17:14:35 mauro Exp $
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*
*********************************************************************/

#include <ifound.h>
#include <ifound/stdc.h>
#include <ifound/priv/gfuncs.h>
#include <signal.h>

/* Private variables */
static FPI dis_handler = (FPI) NULL;

void setEnableIntHandler(FPI f)
{
	dis_handler = f;
}

#ifdef HAVE_FULL_SIGNALS
void enableInterrupts(FPI fp)
{
	bool flag;
	static SigHandlerT save_sigterm = SigHandlerT(NULL);
	static SigHandlerT save_sigint  = SigHandlerT(NULL);
	static SigHandlerT save_sigquit = SigHandlerT(NULL);

	if (SigHandlerT(fp) == SIG_IGN) {
		save_sigterm = SigSet(SIGTERM, SIG_IGN);
		save_sigint  = SigSet(SIGINT,  SIG_IGN);
		save_sigquit = SigSet(SIGQUIT, SIG_IGN);
		flag = false;
	} else if (SigHandlerT(fp) == SIG_DFL) {
		SigSet(SIGTERM, save_sigterm);
		SigSet(SIGINT,  save_sigint);
		SigSet(SIGQUIT, save_sigquit);
		flag = true;
	} else {
		SigSet(SIGTERM, SigHandlerT(fp));
		SigSet(SIGINT , SigHandlerT(fp));
		SigSet(SIGQUIT, SigHandlerT(fp));
		flag = true;
	}
	if (dis_handler)
		(*dis_handler)(flag);
}

#else /* HAVE_FULL_SIGNALS */

void enableInterrupts(FPI fp)
{
	bool flag=true;
	if (SigHandlerT(fp) == (SIG_IGN))
		flag=false;
	SigSet(SIGINT, SigHandlerT(fp));
	if (dis_handler)
		(*dis_handler)(flag);
}
#endif /* HAVE_FULL_SIGNALS */

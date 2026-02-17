/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* wimessge.cc,v 1.1.1.1 1996/04/16 18:52:17 gustavof Exp
*
* DESCRIPTION
* Message Library module - messages handler.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*				|
* DisplayMsg	| Put message in (the last line of) the screen.
* DisplayVMsg	| Same as above but for variable number of arg's.
* ClearMsg		| Clear message if it's displayed.
*				|
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/gndefs.h>
#include <ifound/priv/gfuncs.h>
#include <cracker/pserconn.h>


extern "C" void DisplayVMsg(bool wait_conf, char *fmt, va_list ap);
extern "C" DLLDEPEND void _DisplayVMsg(bool wait_conf, char *msg);

#define WARNINGMSG	"Warning"
#define ERRORMSG	"Error"

/*
 * Put message in (the last line of) the screen.
 */
extern "C" void DisplayMsg(bool wait_conf, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	DisplayVMsg(wait_conf, fmt, ap);
	va_end(ap);
}

/*
* Same as DisplayMsg but for variable number of arguments
*/
extern "C" void DisplayVMsg(bool wait_conf, char *fmt, va_list ap)
{
	String msg = toStr(fmt, ap);
	_DisplayVMsg(wait_conf, toCharPtr(msg));
}

// same as DisplayMsg but without variable arguments
extern "C" void DisplayMessage(bool wait_conf, char *msg)
{
	_DisplayVMsg(wait_conf, msg);
}

/* Global static variables                                         */
static window wd_msg = ERR;

//#include <syslog.h>
extern "C" void _DisplayVMsg(bool wait_conf, char *msg)
{
	if (crackerClient()){
		//syslog(LOG_NOTICE, "bool: %d\t%d\t[%s]", wait_conf, (Int)wait_conf, msg);
		prServerConn(PrCmd::displayMsg) << (Int)wait_conf << msg;
		prServerConn(PrCmd::flush);
		//syslog(LOG_NOTICE, "%d", PrCmd::flush);
	}
	else {
#ifndef HAVENT_UI
		window act_wd;

		act_wd		   = WiCurrent(); /* Preserve the active Window */

		/*
		 * The window overlaps the help area, but do not erase
		 * the help area border.
		 */
		if (wd_msg == ERR) { /* New window for messages */
			wd_msg = WiCreate(SCREEN, footer_orig(), 0, footer_height(),
							  WiWidth(SCREEN), NO_BORDER, "", A_NORMAL);
		}
		else  /* The created window for messages become active */
			(void) WiSwitchTo(wd_msg);

	#ifndef HAVE_TTY
		/* Clear the keyboard buffer		*/
		ClrKeybBuff();
	#endif

		WiMoveTo(0,0);
		WiErase();
		WiPrintf("%s", msg);
		if (wait_conf) {
			WiPutc('\n');
			WiPrintf(_GnErrmsg(HITANYKEY));
			(void) WiGetc();
		}
		(void) WiSwitchTo(act_wd);
#endif
	}
}

/*
 * Clear message if it's displayed.
 */
extern "C" void ClearMsg(void)
{
	if (crackerClient())
		prServerConn(PrCmd::clearMsg).flush();
	else {
		window act_wd;

		if (wd_msg == ERR) return;

		act_wd = WiCurrent(); /* Preserve the active Window */

		/* The message window become active */
		(void) WiSwitchTo(wd_msg);
		WiDelete(WICURRENT); /* Bye bye for the message window */

		/* Now switch to the preserved window */
		(void) WiSwitchTo(act_wd);

		wd_msg = ERR; /* No message window */
	}
}

#ifndef __NT__
extern "C" int WiVMsg(int flag, char * fmt, va_list ap)
{
	wdflag wf = WD_OK;
	switch (flag & (MSG_WAIT|MSG_RELEASE|MSG_PRESERVE))
	 {
		case MSG_WAIT :
			wf = WD_OK;
			break;
		case MSG_PRESERVE :
			wf = WD_PRESERVE;
			break;
		case MSG_RELEASE :
			wf = WD_RELEASE;
	 }
	WiVDialog(flag & MSG_ERROR ? wf|WD_ERROR : wf, wf,
				flag & MSG_ERROR ? ERRORMSG :
				flag & MSG_WARNING ? WARNINGMSG : "", fmt, ap);
	return K_ENTER;
}
#endif

/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dalipref.cc,v 1.1.1.1 1996/04/16 18:52:52 gustavof Exp $
*
* DESCRIPTION: Implementation of dali preference
*********************************************************************/

#include <winmgr.h>
#include <main.h>
#include <dprefm.h>

int main(int argc, char **argv)
{
	startUp("dalipref", argc,	argv);

	//	Window manager initialization calls
	Window::setRaw(WM_PURERAW);

	PrefMenu menu("Dali Preferences");

    /* Put current values here
    	menu.xxx->value() = xxx_value;
    */

	menu.execute();
	if (menu.isAccepted() != TRUE)
		return FALSE;

	/*  Get values here
	   xxx = menu.getXxx();
	*/	

	stop(0);
	return 0;
}


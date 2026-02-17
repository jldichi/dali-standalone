/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: syscomm.h,v 1.1.1.1 1996/04/16 18:52:54 gustavof Exp $
*
* DESCRIPTION: Definitions for system commands
*********************************************************************/

#ifndef SYSCOMM_H
#define SYSCOMM_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

// comandos para find y search de texto
#ifdef USES_OLD_HELP_MENU_STYLE
BeginControlCommandNoViews(HelpHelp)
EndControlCommandNoViews(HelpHelp);

BeginControlCommandNoViews(HelpIndex)
EndControlCommandNoViews(HelpIndex);

BeginControlCommandNoViews(Tutorial)
EndControlCommandNoViews(Tutorial);

BeginControlCommand(FindHelp)
EndControlCommand(FindHelp);

BeginControlCommandNoViews(OpenHelp)
EndControlCommandNoViews(OpenHelp);
#endif

BeginControlCommandNoViews(About)
EndControlCommandNoViews(About);

BeginControlCommandNoViews(Calculator)
EndControlCommandNoViews(Calculator);

BeginControlCommandNoViews(PrintScreen)
EndControlCommandNoViews(PrintScreen);

BeginControlCommandNoViews(ShowCharSet)
EndControlCommandNoViews(ShowCharSet);

BeginControlCommandNoViews(ExecOSCommand)
EndControlCommandNoViews(ExecOSCommand);

BeginControlCommandNoViews(ExecWMCommand)
EndControlCommandNoViews(ExecWMCommand);

BeginControlCommandNoViews(GoToShell)
EndControlCommandNoViews(GoToShell);

BeginControlCommandNoViews(RepaintDesktop)
EndControlCommandNoViews(RepaintDesktop);

#endif

/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: drawcomm.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION:	Definitions for draw commands.
*********************************************************************/

#ifndef DRAWCOMM_H
#define DRAWCOMM_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

// comandos del modo draw

BeginControlCommand(SetDrawMode)	
	Int editmode;
	Int drawmode;
EndControlCommand(SetDrawMode);

BeginAnyCommand(DrawCursRight)
	char c1;
	char c2;
EndAnyCommand(DrawCursRight);

BeginAnyCommand(DrawCursLeft)
	char c1;
	char c2;
EndAnyCommand(DrawCursLeft);

BeginAnyCommand(DrawCursUp)
	char c1;
	char c2;
EndAnyCommand(DrawCursUp);

BeginAnyCommand(DrawCursDown)
	char c1;
	char c2;
EndAnyCommand(DrawCursDown);

BeginControlCommand(EndDrawMode)
	Int drawmode;
EndControlCommand(EndDrawMode);

#endif

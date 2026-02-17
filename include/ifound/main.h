/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: main.h,v 1.1.1.1 1996/04/16 18:51:32 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#define Main(prog_name)											\
																\
		int prog();												\
																\
		int main(int argc, char *argv[])						\
		{														\
			startUp(prog_name, argc, argv);						\
			stop(prog());										\
			return 0;											\
		}														\
		int prog()

#endif // MAIN_H

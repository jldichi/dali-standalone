/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: start.h,v 1.1.1.1 1996/04/16 18:51:33 gustavof Exp $
*
* DESCRIPTION
* Configuration information for "C" version
*********************************************************************/

#ifndef START_H
#define START_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

extern "C" void start(int argc, char **argv);

#endif // START_H

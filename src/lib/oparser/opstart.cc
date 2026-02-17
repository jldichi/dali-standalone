/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: opstart.cc,v 1.1.1.1 1996/04/16 18:52:11 gustavof Exp $
*
* DESCRIPTION
* Configuration information
*********************************************************************/

#include <ifound/oparser.h>
#include <ifound/priv/gfuncs.h>

void startUp(const String &applName, int &argc, char **&argv,
		OptionParser &op)
{
	startUp(applName, argc, argv);
	op.parse(argc, argv);
	setArgs(argc, argv);
}


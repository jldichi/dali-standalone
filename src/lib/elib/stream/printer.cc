/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: printer.cc,v 1.1.1.1 1996/04/16 18:52:43 gustavof Exp $
*
*
* DESCRIPTION:
*				Printer class
*
*********************************************************************/


#include <local/strfile.h>

// Printer class

Printer::Printer() : OutputPipe(String("PRINTER").getEnv())
{ }

Printer::~Printer()
{ }

// end of Printer

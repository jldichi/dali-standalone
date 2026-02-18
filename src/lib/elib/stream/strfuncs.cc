/********************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: strfuncs.cc,v 1.1.1.1 1996/04/16 18:52:43 gustavof Exp $
*
* DESCRIPTION: Functions to pass as arguments to OutputStreams
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <local/strfile.h>

#include <errno.h>

// flush the OutputStream

OutputStream &flush(OutputStream &s)
{
	s.flush();
	return s;
}

// flush the OutputStrea

OutputStream &endl(OutputStream &s)
{
	s << NEW_LINE;
	s.flush();
	return s;
}

// force the display of a message

OutputStream &endMsg(OutputStream &s)
{
	s.endMsg();
	return s;
}

// end of strfuncs

/* *******************************************************************
* Copyright (c) 2026 Master_Solutions S.R.L.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Master_Solutions S.R.L.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* dirname.cc,v 1.1.1.1 1996/04/16 18:52:41 gustavof Exp
*
* DESCRIPTION
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <ifound/priv/gfuncs.h>
#include <local/strfile.h>
#include <ifound/itypes.h>
#include <local/dirname.h>

// For function isAbsolutePath()
#include <local/osfile.h>

DirName::DirName(const String &d, const bool c)
{
	current = d;
	if (c)
		compress();
}

void DirName::chDir(const String &p, const bool comp)
{
	if (isAbsolutePath(p) && !comp) {
		current = p;
		return;
	}

	StrCursor dc(p, PATH_SLASH);

	// If compress flag avoid treating first part of directory
	if (comp)
		++dc;

	for (; dc.ok() ; ++dc) {
		String d = dc.value();

		// Nothing changes
		if (d == "." || d == NULL_STRING)
			continue;

		// Back one dir
		if (d == "..") {
			current = String(current(0, current.backIndex(PATH_SLASH[0])));
			continue;
		}

		// Add whatever comes
		current += String(PATH_SLASH) + d;
	}
}

void DirName::compress()
{
	String tmp = current;

	// Keep first part of directory
	current = String(current(0, current.index(PATH_SLASH[0])));

	// Now change directory
	chDir(tmp, true);
}

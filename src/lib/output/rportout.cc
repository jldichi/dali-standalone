/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rportout.cc,v 1.2 1999/08/02 17:03:48 eduardoc Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ideafix.h>

#include <ideafix/priv/rportout.h>
#include <ideafix/priv/opers.h>
#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/reprep.h>

static char dummy[1] = { '\0' };

ReportOutput::ReportOutput(const String &rpname, Int nc, const String &arg)
:	fmt(arg)
{
	items = 0;
	ncols = 0;
	hlines = -1;
	// Open the report.
	rp  = tr_new ReportRep(rpname, Report::Flag(Report::nodb|Report::nobegin));
	if (!rp->ok()) {
		char *rname = (char*)toCharPtr(rpname);
		warning(_RpErrmsg(E_RP_NOT_OPEN), rname);
		return;
	}

	ncopies  = nc;
}

ReportOutput::~ReportOutput()
{
	tr_delete(rp);
}

void ReportOutput::setDest(Report::OutputType out_to, char *dest)
{
	rp->setOutput(out_to, dest);
}

Int ReportOutput::operator<<(const char *bp)
{
	assert(rp != NULL);
	if (lines == NULL) {
		if (rp->begin(ncopies, fmt) == ERR) {
			warning(_RpErrmsg(E_RP_NO_OUTPUT), toCharPtr(rp->output()));
    		tr_delete(rp);
			rp = NULL;
			return ERR;
		}
		lines = (UnChar**)dummy;
	}

	if (currcol < rp->nFields()) {
		RpFieldRep *f = rp->orderField(currcol);
		f->set(bp);
	}
	if (++currcol >= ncols) {
		currcol = 0;
		items++;
		for (Int i = 0; i < rp->nZones(); ++i) {
			if (rp->zone(i)->isAlways())
				rp->execute(i);
		}
	}
	return OK;
}


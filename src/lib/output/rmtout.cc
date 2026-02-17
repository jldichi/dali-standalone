/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: rmtout.cc,v 1.1.1.1 1996/04/16 18:52:07 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ideafix/priv/rmtout.h>
#include <ifound/str.h>
#include <ifound/parray.h>


declare(PtrArray, BaseOutputColumn);

#define columns (*columns_)

RemoteOutput::RemoteOutput(Int dst, const String &arg, const String &sep)
: DelimitedOutput(dst, arg, sep, 1)
{
	currline = 0;
}

RemoteOutput::~RemoteOutput()
{
	fprintf(stdout, "%c%c", EOSF, rsep);
	fflush(stdout);
}

void RemoteOutput::outputHeader()
{
	// Print the output's header just once (at the beginning)
	if (currline == 0) {
		currline = 1;
	
		fprintf(fp, "#%d%c", ncols == 0 ? 1 : ncols, rsep);
		for (Int i = 0; i < ncols; i++) {
			if (i != 0)
				fputc(fsep, fp);
			fprintf(fp, "%d %d %d %s", columns[i]->ty, columns[i]->width,
									   columns[i]->ndec, 
									   toCharPtr(columns[i]->head));
		}
		fputc(rsep, fp);
	}
}

Int RemoteOutput::operator<<(const char *bp)
{
	outputHeader();
	return DelimitedOutput::operator<<(bp);
}



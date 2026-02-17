/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: gridout.cc,v 1.3 2004/09/29 17:19:48 damianm Exp $
*
* DESCRIPTION
*********************************************************************/

#include <string.h>

#include <ideafix.h>
#include <ifound/stdc.h>
#include <ideafix/priv/gridout.h>
#include <ifound/str.h>
#include <ifound/parray.h>
#ifdef IDEAWIN
#include <mswi/iwwi.h>
#endif

declare(PtrArray, BaseOutputColumn);

#define columns	(*columns_)

Int GridOutput::_InitOutput()
{
	Int i, x, y;
	UnChar *aux;

	// Calculate output width

	if (ncols == 0) {
		x = UnShort(pwidth + 1);
		setColumn(NULL_STRING, x, TY_STRING);
	}
	else {
		for (x = i = 0; i < ncols; i++)
			x += columns[i]->width;
		x += ncols;
	}

	// Allocate an array of pointers plus a set of buffers (one per line)
	y = pheight; assert(y);
#ifdef IDEAWIN
	y = _IRGetMaxYPage();
	if (y < pheight)
		y = pheight;
#endif
	aux = (UnChar *) tr_new char[sizeof(UnChar *) * y + x*y];

	lines = (UnChar **) aux;
	aux += sizeof(UnChar *) * y;
	twidth = UnShort(x-1);
	for (i = 0; i < y; i++) {
		lines[i] = aux;
		aux += x;
		_BlankLine(i);
	}

	pageno = 0;
	_Heading();

	return OK;
}

void GridOutput::_BlankLine(Int n)
{
	register Int i;
	register UnChar blank, vert, *p = lines[n];

	if (n == hlines) {
		blank = C_HORIZ;
		vert  = C_CEN;
	} else {
		blank = ' ';
		vert  = C_VERT;
	}

	for (i = 0; i < ncols; i++) {
		if (i)
			*p++ = vert;
		mSet(p, blank, columns[i]->width);
		p += columns[i]->width;
	}
}

void GridOutput::_Heading()
{
	UnChar *p, *q;
	Int col, i, j, k, org;

	for (i = col = 0; col < ncols; col++) {
		if (col) i++;
		p = (UnChar *) toCharPtr(columns[col]->head);
		for (k = 0; k < hlines; k++) {
			q = p ? (UnChar *) strchr((char *) p, '\n') : NULL;
			org = (columns[col]->width - (q ? q-p : strlen((char*)p))) / 2;
			for (j=0; j<columns[col]->width; j++, i++) {
				if (j>=org && *p && *p != '\n') lines[k][i] = *p++;
				if (!k) lines[hlines][i] = C_HORIZ;
			}
			if (*p++ == 0) break;
			i -= columns[col]->width;
		}
	}
	currline = hlines + 1;
}

GridOutput::GridOutput()
{
}

GridOutput::~GridOutput()
{
	tr_delete_vec(lines);
}

void GridOutput::setList()
{
	list = false;
}

Int GridOutput::operator<<(const char *bp)
{
	if (lines == NULL) 
		if (_InitOutput() == ERR)
			return ERR;

        if (currline >= pheight) {
                if (_OutPage(true, pageno > 1) == ERR)
                        return ERR;
                if (_GetCmd(true) == ERR)
                        return ERR;
                currline = hlines+1;
                for (Int i = currline; i < pheight; i++)
                        _BlankLine(i);
        }

	UnChar *p, *orig;
	orig = p = &lines[currline][currpos];

	for (Int i = 0; i < twidth && *bp; bp++, i++) {
		if (UnChar(*bp) >= ' ' || *bp == '\033')
			*p++ = *bp;
		else if (*bp == '\t') {
			if (list) {
				*p++ = '\\';
				if (++i < twidth)
					*p++ = 't';
			}
			else {
				do
					*p++ = ' ';
				while (++i < twidth && (p - orig) % 4);
			}
		}
		else {
			*p++ = '^';
			if (++i < twidth)
				*p++ = *bp + '@';
		}
	}

	currpos += columns[currcol]->width+1;

	if (++currcol == ncols) {
		_LogInFile();
		// Reset counters
		currcol = 0;
		currpos = 0;
		currline++;
		items++;
		if (items % (pheight-hlines-1) == 1)
			pageno++;

		// Check if I need to output the page
		if (currline < pheight)
			_Refresh();
	} 
	return OK;
}


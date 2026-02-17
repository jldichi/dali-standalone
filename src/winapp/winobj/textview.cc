/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: textview.cc,v 1.1.1.1 1996/04/16 18:52:46 gustavof Exp $
*
* DESCRIPTION: Text viewing facility routines
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <winapp/textview.h>
#include <ifound/list.h>

#include <winapp/strlist.h>
implement(List, String)
implement(ListCursor, String)

// TextView Class

#define TVWIDTH		-1
#define TVHEIGHT 	16

bool TextView::display(Int fil, Int col)
{
	idCheck();

	if (!(wi->hidden())) {
		wi->front();
		return true;
	}

	String helpText1, s1;
	Int lineLength, pos;

	// Crear descripcion de comandos
	wi->keyHelp(K_END, s1);
	lineLength = s1.length();

	helpText1 = libMsg("HELPMSG1");
	lineLength += helpText1.length();

	// Display the actual window
	nfil = TVHEIGHT;
	ncol = TVWIDTH;
	(void)WinObj::display(fil, col);

	wi->wrap(false);

	wi->cursor(false);

	// Right justify
	pos = ncol - lineLength - 2;
	wi->goTo(nfil, pos);

	// Write bottom line
	wi->setAttr(A_REVERSE);
	*wi << s1;
	wi->setAttr(A_NORMAL);
	*wi << helpText1;

	// Prepare window
	wi->goTo(0, 0);
	wi->setScroll(0, nfil - 1, 0, ncol-1);
	return true;
}

TextView::TextView(SFPI fp, const String &lbl, Int fil, Int col) :
		WinObj(fil, col)
{
	idCheck();

	border = SLINE_BORDER;
	label = lbl;
	nattr = A_NORMAL;

	fpRead = fp;
}

const Int MAX_VIEW = 256;
const Int HORIZ_INC = 10;

Int TextView::execute(UnChar)
{
	idCheck();

	bool undisplay = wi->hidden();

	(void)display();

	// fake command to begin
	UnChar c = K_PAGE_DOWN;
	Int i;
	String text;

	column = 0;
	line = 1;

	do {
		switch (c) {
		case K_CURS_LEFT:
		case K_PAGE_LEFT:
			if (column == 0) {
				wi->beep();
				continue;
			} else {
				column -= HORIZ_INC;
				if (column < 0)
					column = 0;
				line -= nfil;
				for (i = 0; i < nfil; i++, line++) {
					text = fpRead(line);
					displayLine(i, text);
				}
			}
			break;
		case K_CURS_RIGHT:
		case K_PAGE_RIGHT:
			if (column == MAX_VIEW) {
				wi->beep();
				continue;
			} else {
				column += HORIZ_INC;
				if (column > MAX_VIEW)
					column = MAX_VIEW;
				line -= nfil;
				for (i = 0; i < nfil; i++, line++) {
					text = fpRead(line);
					displayLine(i, text);
				}
			}
			break;
		case K_CURS_UP:
			text = fpRead(line-nfil-1);
			if (text == HIGH_VALUE) {
				wi->beep();
				continue;
			} else {
				wi->scroll(-1);
				displayLine(0, text);
				line--;
			}
			break;

		case K_CURS_DOWN:
		case K_ENTER:
			// more opts?
			text = fpRead(line);
			if (text == HIGH_VALUE) {
				wi->beep();
				continue;
			} else {
				wi->scroll(1);
				displayLine(nfil-1, text);
				line++;
			}
			break;				

		case K_PAGE_UP:
			if (line <= nfil+1) {
				wi->beep();
				continue;
			} else {
				line -= (nfil*2);
				if (line <= 0)
					line = 1;
			}
			// Fall on purpose to K_PAGE_DOWN!
#if 0
disp_page:
#endif
		case K_PAGE_DOWN:
			if (fpRead(line) == HIGH_VALUE) {
				wi->beep();
				continue;
			} else
				for (i = 0; i < nfil; i++, line++) {
					text = fpRead(line);
					displayLine(i, text);
				}
			break;

#if 0
		case K_META:
			Int aux; // Line to search from & result of search

			if (aux = _SearchRegexp(WiOrgRow(WICURRENT)+nfil,
									WiOrgCol(WICURRENT),
									ncol,line,garg,fpRead)) {
				line = aux;
				goto disp_page;
			}
			break;
#endif
		default:
			wi->beep();
		}

		updateScrBars();

	} while ((*wi >> c, c) != K_END &&
			c != K_TAB);

	if (undisplay)
		unDisplay();

    lastkey = c;
    
    return (lastkey == K_END) ? REJECT_EXIT : ACCEPT;
}

void TextView::updateScrBars()
{
	idCheck();

	UnChar c;
	if (fpRead(line-nfil-1) != HIGH_VALUE)
		c = C_UARROW;
	else
		c = C_VERT;

	wi->goTo(0, -1);
	*wi << c;
	wi->goTo(0, ncol);
	*wi << c;

	if (fpRead(line) != HIGH_VALUE)
		c = C_DARROW;
	else
		c = C_VERT;

	wi->goTo(nfil-1, -1);
	*wi << c;
	wi->goTo(nfil-1, ncol);
	*wi << c;

	// Position the cursor in a reasonable position
	wi->goTo(nfil, ncol);
}

void TextView::displayLine(Int i, const String &st)
{
	idCheck();

	wi->goTo(i,1);
	wi->eraseEol();
	if (st != NULL_STRING && st != HIGH_VALUE)
		*wi << st(column, INT_MAX);
	wi->goTo(nfil-1,1);
}

// End of TextView Class

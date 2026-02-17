/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* wimsg.cc,v 1.1.1.1 1996/04/16 18:52:16 gustavof Exp
*
* DESCRIPTION:
*		This module contains routines for displaying (and accepting
*		response from the user) a binary (only two possible answers)
*		dialog window.
*
* ROUTINE				|  PURPOSE
*-----------------------+-------------------------------------------
* WiDialog					Displays the window with the passed options
* WiMsg			 			Create a window and show a messg.
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>

#include <ideafix/priv/gndefs.h>
#include <ifound/priv/gfuncs.h>
#include <cracker/pserconn.h>

static String centerMessage(char *msg, int width, int &height);

// +++ CONFIG +++

#define ORGR_POPUP_DG  14
#define BUTTON_WIDTH   16
#define IMAGE_WIDTH	   (BUTTON_WIDTH - 4)
#define BUFFSIZE  		2048
#define WD_CANT_FLAGS	8

// --- CONFIG ---

#define max(a,b) ((a)>(b)) ? (a) : (b)

extern "C" wdflag _WiVDialog(wdflag flags, wdflag def,
							 int wih, int wiwidth, const char *w_title,
							 char *msg);


extern "C" int WiMsg(char *fmt, ...)
{
	va_list ap;
	int ret;
	va_start(ap, fmt);
	ret = (int) WiVDialog(WD_OK,(wdflag) NULL,(const char *)NULL, (char *) fmt, ap);
	va_end(ap);
	return ret;
}

extern "C" int WiHandler(int inflag, const char *label, const char *msg)
{
	wdflag flags = 0;
	if (inflag & (MSG_ERROR|MSG_FATALERROR))
		flags |= WD_ERROR;
	if (inflag & MSG_RELEASE)
		flags |= WD_RELEASE;
	return (int) WiVDialog((inflag & MSG_WAIT) ? flags|WD_OK: flags , 0,
						   label, (char*)msg, NULL);
}

extern "C" wdflag WiDialog(wdflag flags, wdflag def, const char *w_title, char * fmt,...)
{
	va_list ap;
	wdflag ret;
	va_start(ap,fmt);
	ret = WiVDialog(flags,def, w_title, fmt, ap);
	va_end(ap);
	return ret;
}


static int nchars  = 0;
static int cols    = 0;
static int lines   = 0;
static int wiwidth = 0;

static int spsub(char c, char **str)
{
	if (nchars < BUFFSIZE ) {
		++nchars;
		if (c == '\n') {
			lines++;
			cols = 0;
		} else if (c == '\t')
			cols +=4;
		else
			cols++;
		wiwidth = max(cols +3 ,wiwidth);
		return (*(*str)++ = c) & 0xff;
	}
	return 0;
}

extern "C" wdflag WiVDialog(wdflag flags, wdflag def, const char *w_title,
							char *fmt, va_list ap)
{
	char buffer[BUFFSIZE];

	lines   = 1;
	nchars  = cols = 0;
	wiwidth = BUTTON_WIDTH;

	/* Parse message */

	char *msg = fmt;
	if (msg==NULL)
		msg="";
	else if (msg[0]) {
		char *p = buffer;

		doFormat((IFPICP) spsub, (char *) &p, (const char *)fmt, ap);
		*p = '\0';
		msg = buffer;
	}

	return _WiVDialog(flags, def, lines, wiwidth, w_title, msg);
}

static void Rectangle(int orgr, int orgc, int nrows, int ncols,bool selected)
{
	int i;
	static char bord[] = {
		C_UL, C_HORIZ, C_UR, C_VERT, C_LR, C_HORIZ,
		C_LL, C_VERT
	};
	static char bord2[] = {
		C_UL, C_HORIZ, C_UR, C_D_VERT, C_D_LR, C_D_HORIZ,
		C_LL, C_VERT
	};
	char *borders = (selected) ? bord2 : bord;
	WiMoveTo(orgr++, orgc);
	WiPutc(borders[0]);
	for (i=0; i<ncols-2; i++)
		WiPutc(borders[1]);
	WiPutc(borders[2]);
	for (i=0; i<nrows-2; i++) {
		WiMoveTo(orgr, orgc);
		WiPutc(borders[7]);
		WiMoveTo(orgr++, orgc+ncols-1);
		WiPutc(borders[3]);
	}
	WiMoveTo(orgr, orgc);
	WiPutc(borders[6]);
	for (i=0; i<ncols-2; i++)
		WiPutc(borders[5]);
	WiPutc(borders[4]);
}


static void DrawButton(int pos_x, int pos_y, char *text, bool active,
		int backgr)
{
	Rectangle(pos_x,pos_y,3,BUTTON_WIDTH-2,active);
	WiMoveTo(pos_x + 1,pos_y + 1);
	if (active)	WiSetAttr(A_REVERSE);
	String t = text;
	t = t.center(IMAGE_WIDTH);
	WiPuts(toCharPtr(t));
	WiSetAttr(A_NORMAL|backgr);
	WiRefresh();
}

extern "C" wdflag _WiVDialog(wdflag flags, wdflag def,
							 int wih, int wiwidth, const char *w_title,
							 char *msg)
{
	if (crackerClient()) {
		prServerConn(PrCmd::wiDialog) << Int(flags) << Int(def) << Int(wih)
									  << Int(wiwidth) << w_title << msg;

		return prServerConn.result();
#if 0
		if (prServerConn.currCmd() == ERR) {
			prServerConn(PrCmd::wiDialog) << Int(flags) << Int(def) << wih
										  << wiwidth << w_title << msg;

			return prServerConn.result();
		}
		else {
			fprintf(stderr, "Trying to display message :\n\t%s\n", msg);
			fprintf(stderr, "While executing command %ld\n",
							prServerConn.currCmd());
			return WD_ABORT;
		}
#endif
	} else {
		static window wi = ERR;	  // To hold messages on screen

	    window wcurr, defpar = SCREEN;
		int j, cantopt = 0, option[WD_CANT_FLAGS];
		Int select = 0, backgr = 0;
		char *text[WD_CANT_FLAGS];
		bool done = false, center = true;
		char msgtxt[16];

		// Save current window
		wcurr = WiCurrent();

		// Clear the keyboard buffer
#if !defined(HAVE_TTY) && !defined(HAVENT_UI) && !defined(IDEAWIN)
		ClrKeybBuff();
#endif

		// Parse options

		backgr = (flags & WD_ERROR) ? A_RED_BG|A_BOLD : A_BLUE_BG;
		flags &= ~WD_ERROR;
		center = (flags & WD_HERE) || (flags == WD_PRESERVE);
		flags &= ~WD_HERE;

		if (wi != ERR) {
			WiSwitchTo(wi);
			WiDelete(WICURRENT);
			if (wi != wcurr)
				WiSwitchTo(wcurr);
			WiRefresh();
			wi = ERR;
		}

		if (flags == WD_PRESERVE)
			wih += 2;
		else {
			if (flags & WD_RELEASE)
				return OK;

	        wih += 5;
			for (j = 0; j < WD_CANT_FLAGS; j++) {

				Int i = 1 << j;

				if (flags & i) {
					sprintf(msgtxt, "DLG_MSG%d", j + 1);
					text[cantopt] = __WiErrmsg(msgtxt);
					option[cantopt++] = j;

					if (!select && (def & i))
						select = cantopt;
				}

			}
			if (!select)
				select = 1;
		}

		/* Calculate and create main window */

		wiwidth = max((BUTTON_WIDTH * cantopt + 3), wiwidth);

		window parent = (flags == WD_PRESERVE) ? WiDefPar() : SCREEN;

		if (wiwidth > WiWidth(parent) - 1)
			wiwidth = WiWidth(parent) - 1;

		wiwidth = (wiwidth / 2) * 2;

		String message;
		if (msg)
			message  = centerMessage(msg, wiwidth, wih);

		if (wih > WiHeight(parent) - 2)
			wih = WiHeight(parent) - 2;

		wi = WiCreate( parent,
				center ? WiLine(WiDefPar()) + 1 : ORGR_POPUP_DG - wih / 2,
				center ? WiCol(WiDefPar()) + 2 : WiWidth(SCREEN)/2 - wiwidth/2,

				wih, wiwidth, DLINE_BORDER|backgr, (char *) w_title,
				A_NORMAL|backgr);

		/* Draw buffer and buttons */

		WiWrap(false);
		WiCursor(false);
		WiSetAttr(backgr);

		if (msg) {
			WiMoveTo(0,0);
			int j = 0;
			for (StrCursor st(message,"\1"); st; st++, j++) {
				WiMoveTo(j, 0);
				String line = st.value();
				WiPuts(toCharPtr(line));
			}
		}

		WiRefresh();
		if (flags == WD_PRESERVE) /* I don't switch to father window !!! */
			return OK;


		for (j = 0; j < cantopt; j++)
			DrawButton(wih-3,
					(wiwidth-BUTTON_WIDTH*cantopt+3)/2+BUTTON_WIDTH*j,
					text[j], (j + 1 == select), backgr);

		defpar = WiDefPar();
		WiSetDefPar(SCREEN);

		/* Main loop */
		do {
			switch (j = (int) WiGetc()) {
			case K_ENTER:
				done = true;
				break;
			case K_CURS_LEFT:
			case K_CURS_RIGHT:
				DrawButton(wih-3,
					(wiwidth-BUTTON_WIDTH*cantopt+3)/2+BUTTON_WIDTH*(select-1),
					text[select-1], false, backgr);
				if (j == K_CURS_LEFT) {
					select--;
					if(select < 1)
						select = cantopt;
				} else {
					select++ ;
					if (select > cantopt)
						select = 1;
				}
				DrawButton(wih-3,
					(wiwidth-BUTTON_WIDTH*cantopt+3)/2+BUTTON_WIDTH*(select-1),
					text[select-1], true, backgr);
				break;
			case K_END:
				break;
			default:
				{
					char i = toupper(j);
					for (j=0; (j < cantopt) && !done; j++) {
						char *p = text[j];
						while (*p == ' ')
							p++;
						if (toupper(*p) == i) {
							select = j+1;
						     done = true;
						}
					}

				}
			}
		} while (!done);
		WiDelete(wi);
		wi = ERR;
		WiSwitchTo(wcurr);
		WiSetDefPar(defpar);
		WiCursor(true);
		WiRefresh();
		return (1 << option[select-1]);
	}
}

static String centerMessage(char *msg, int width, int &height)
{
	char *begin = msg;
	String result;
	int lines = 1;

	for (char *p =begin; *p; p++) {
		if (p - begin >= width ) {
			while (*p != ' ' && p > begin)
				p--;
			if (*p == ' ') {
				result += String(begin,  p-begin).center(width) + "\1";
				begin = p + 1;
			}
			else {
				result += String(begin,  width) + "\1";
				begin = p;
			}
			lines++;
		}
		else if (*p == '\n') {
		   	result += String(begin,  p-begin).center(width) + "\1";
			begin = p + 1;
			lines++;
		}
	}
   	result += String(begin).center(width);
	height = max(height,lines);
	return result;
}

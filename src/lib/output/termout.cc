/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: termout.cc,v 1.3 2002/03/15 21:30:06 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ideafix.h>
#include <cfix.h>
#include <ideafix/priv/termout.h>
#include <ideafix/priv/opers.h>
#include <ideafix/priv/gndefs.h>

#include <ifound/parray.h>

#ifdef IDEAWIN
#include <mswi/iwwi.h>
#	define NORM_HDR_ATTR  A_BLUE|A_WHITE_BG|LT_BKCOLOR
#	define SEL_HDR_ATTR   A_ITALIC|A_RED|A_WHITE_BG|LT_BKCOLOR
#endif

declare(PtrArray, BaseOutputColumn);

#define columns	(*columns_)

#ifdef IDEAWIN
	static bool bScroll = FALSE;
	static UChar byCmd;
#endif

Int TerminalOutput::_InitOutput()
{
	assert(GridOutput::_InitOutput() == OK);

	// If terminal create the output window
	UnShort width = pwidth < twidth ? pwidth : twidth;

#ifndef IDEAWIN
	wp = WiCreate(WiDefPar(), topmarg, leftmarg + (pwidth - width) / 2,
			   	  pheight+2, width+2, NO_BORDER, "", backgr);
	width = (UnShort) WiWidth(wp) - 2;
	WiSetScroll(hlines+2, WiHeight(WICURRENT)-2, 0, WiWidth(WICURRENT)-1);
#else
	wp = WiCreate(WiDefPar(), topmarg,
					   leftmarg + (_IRGetMaxXPage() - width) / 2,
					   pheight, width+1, NO_BORDER|B_REPORT_WND_CLASS, "",
					   backgr);
	width = (UShort) WiWidth(wp) - 1;
    _IRSetOutput(this);
	WiSetScroll(hlines+2, WiHeight(WICURRENT)-2, 0,	WiWidth(WICURRENT)-1);
#endif
	if (width < pwidth)
		pwidth = width;
	WiWrap(false);
	WiCursor(false);

	// Open the temp file
	if ((fp = tmpfile()) == NULL)
		Error(_RpErrmsg(E_RP_NOT_TMPDIR));

	return wp;
}

void TerminalOutput::_LogInFile()
{
	if (fp) {
		fseek(fp, (long) items * twidth, 0);
		fwrite(lines[currline], twidth, 1, fp);
	}
}

void TerminalOutput::_ReadPage(Int when, Int from, Int n)
{
	fseek(fp, (long) from * twidth, 0);
	while (n--) {
		if (fread(lines[when], twidth, 1, fp) != 1)
			_BlankLine(when);
		when++;
	}
}

Int TerminalOutput::__GetCmd(bool more)
{
	Int i = 0, j;
	UnChar cmd;
	UnChar *p;
	Int top_line = items - currline + hlines+1;
	Int lines_page = pheight - (hlines+1);
                      
	if(wp == ERR)
		return ERR;
                      
	forever {
		WiCursor(true);
		cmd = (UnChar) (*getcmd)();
		WiCursor(false);

#ifdef IDEAWIN
		bool bChngY = _IRChngSizeY();

		if (_IRChngSizeX() && !bChngY) {
			_OutPage(FALSE,FALSE);
           	continue;
	   	}
	   	if (bChngY) {
		   int oldpheight = pheight;
		   pheight = _IRGetNewYPage();
		   pageno  =  items/(pheight-hlines-1);
		   lines_page = pheight - (hlines+1);
			if ( pheight <= oldpheight ) {
					currline = pheight;
					_OutPage(FALSE,FALSE);
            		continue;
			}
			else if (pheight > oldpheight &&
				      (top_line+lines_page) <= items) {
					 currline = pheight;
					_ReadPage(hlines+1, top_line, lines_page);
				    _OutPage(FALSE,FALSE);
			        continue;
	        } else if ( pheight > oldpheight &&
				(top_line+lines_page) > items && more) {
				int i;
				if ((top_line+currline) < items)
					currline = items - top_line + hlines+1;
				_ReadPage(hlines+1,top_line, currline);
				for(i=currline;i<pheight;i++)
					_BlankLine(i);
				return TRUE;
			} else if ( pheight > oldpheight &&
			     (top_line+lines_page) > items && !more) {
				_ReadPage(hlines+1, top_line, lines_page);
				_OutPage(FALSE,FALSE);
				continue;
			}
		}

#endif

		switch (cmd) {
		case K_END :
			WiDelete(WICURRENT);
			wp = ERR;
			return ERR;
		case K_CURS_DOWN :
			if (!more && top_line + lines_page >= items) {
				w_beep();
				continue;
			}
#ifndef IDEAWIN
			WiScroll(1);
#endif			
			p = lines[hlines + 1];
			for (i = hlines + 2; i <  pheight; i++)
				lines[i-1] = lines[i];
			i = pheight-1;
			lines[i] = p;
			_BlankLine(i);
		case K_PAGE_DOWN:
		case K_ENTER:
			if (fp == NULL || top_line + lines_page >= items) {
				if (!more) {
					w_beep();
					continue;
				}
				/*
				if (RunMode == MODE_IQL_INT) SetWorkingMsg(true);
				*/
				if (cmd == K_CURS_DOWN)
					currline--;
#ifdef IDEAWIN
				bScroll = true;
				byCmd = cmd;
#endif
				return OK;
			}
			if (cmd == K_CURS_DOWN)
				_ReadPage(i, top_line++ + lines_page, 1);
			else {
				top_line += lines_page;
				_ReadPage(hlines + 1, top_line, lines_page);
			}
			break;

		case K_CURS_UP:
		case K_PAGE_UP:
			if (fp == NULL || top_line <= 0) {
				w_beep();
				continue;
			}
			if (cmd == K_CURS_UP) {
				i = pheight - 1;
				p = lines[i];
				for (; i > hlines + 1; i--)
					lines[i] = lines[i-1];
				lines[i] = p;
				_ReadPage(i, --top_line, 1);
			}
			else {
				top_line -= lines_page;
				if (top_line < 0)
					top_line = 0;
				_ReadPage(hlines+1, top_line, lines_page);
			}
			break;
			
		case K_CURS_LEFT:
			if (offset <= UnShort(base)) {
				w_beep();
				continue;
			}
			i = offset-1;
			for (; i > base && lines[0][i - 1] != C_VERT; i--)
				if (offset - i >= pwidth)
					break;
			offset = UnShort(i);
			break;
		case K_CURS_RIGHT:
			if (offset + pwidth >= twidth) {
				w_beep();
				continue;
			}
			for (i = offset + 1; lines[0][i - 1] != C_VERT; i++)
				if (i - offset >= pwidth)
					break;
			offset = UnShort(i);
			if (offset + pwidth > twidth)
				offset = twidth - pwidth;
			break;
		case K_PAGE_LEFT:
			if (offset <= UnShort(base)) {
				w_beep();
				continue;
			}
			i = offset - pwidth;
			offset = i < base ? base : UnShort(i);
			break;
		case K_PAGE_RIGHT:
			if (offset + pwidth >= twidth) {
				w_beep();
				continue;
			}
			offset += pwidth;
			if (offset + pwidth > twidth)
				offset = twidth - pwidth;
			break;
		default :
			if (isdigit(cmd)) {
				i = cmd - '0';
				if (i < ncols) {
					j = 3 * (pwidth + base) / 2;
					Int bs;
					for (bs = 0; i > 0 && bs < j; i--)
						bs += columns[i - 1]->width + 1;
					if (i == 0) {
						pwidth += base - bs;
						offset += (UnShort) bs - base;
						base    = (UnShort) bs;
						if (offset < UnShort(bs))
							offset = UnShort(bs);
						break;
					}
				}
			}
			w_beep();
			continue;
		}
#ifdef IDEAWIN
		bScroll = true; byCmd = cmd; 
#endif
		_OutPage(more ? true : (top_line + lines_page < items),
				top_line > 0);
	}
}

Int TerminalOutput::_GetCmd(bool)
{
	if (__GetCmd(true) == ERR) 
		return ERR;
	last_refresh = _global_time(NULL);
	return OK;
}

void TerminalOutput::_Refresh()
{
	if(wp == ERR)
		return;
	Int act_time = _global_time(NULL);

	if (!last_refresh || act_time > last_refresh + WAIT_TIME) {
		_OutPage(true, pageno > 1);
		WiRefresh();
		last_refresh = act_time;
	}
}

Int TerminalOutput::_OutPage(bool more, bool less)
{

	if(wp == ERR)
		return OK;

#ifdef IDEAWIN
	attr_type oldAttr = WiGetAttr();
	_IRClear();
#endif

	bool cont = (offset + pwidth) < twidth;
	Int width = cont ? pwidth : twidth - offset;

	// Print the top and the bottom
	Int n;
#ifndef IDEAWIN
	for (n = 0; n <= pheight + 1; n += pheight + 1) {
		WiMoveTo(n, 0);
		WiPutc(n ? C_D_LL : C_D_UL);

		UnChar *p = &lines[0][0];
		Int i;
		for (i = 0; i < base; i++)
			WiPutc(*p++ != C_VERT ? C_D_HORIZ : n ? C_DS_BOT : C_DS_TOP);

		i = 0;
		if (offset > UnShort(base)) {
			WiPutc(C_LARROW);
			i++;
		}
		p = &lines[0][offset + i];
		while (i++ < width)
			WiPutc(*p++ != C_VERT ? C_D_HORIZ : n ? C_DS_BOT : C_DS_TOP);
		if (cont) {
			WiPutc('\b');
			WiPutc(C_RARROW);
		}
		WiPutc(n ? C_D_LR : C_D_UR);
	}
#endif // !IDEAWIN

	// Print the body
	for (n = 0; n < pheight; n++) {
#ifdef IDEAWIN
		WiMoveTo(n, 1);
		if (n < hlines)
			WiSetAttr(NORM_HDR_ATTR);
		if (n == hlines) {
			WiMoveTo(n, 0);
			WiPutc(C_HORIZ);
		}
#else
		WiMoveTo(n + 1, 0);
		bool sep = n == hlines;
		WiPutc(sep ? C_DS_LEFT : C_D_VERT);
#endif
		UnChar *p =  &lines[n][0];
		bool attr = false;
		Int i;
		for (i = 0; i < base; i++, p++) {
			if (n < hlines) {
				if (!attr) {
#ifdef IDEAWIN
					WiSetAttr(SEL_HDR_ATTR);
#else
					WiSetAttr(A_BOLD);
#endif					
					attr = true;
				} else if (*p == C_VERT) {
#ifdef IDEAWIN
					WiSetAttr(NORM_HDR_ATTR);
#else
					WiSetAttr(A_NORMAL);
#endif					
					attr = false;
				}
			}
			WiPutc(*p);
		}
		p = &lines[n][offset];
#ifdef IDEAWIN
		i = 0;
		Int j = offset;
		while (i < width && j < twidth) {
			if( *p == '\033' ) {
			 	char c;
				int k=i;
				j++;
				WiPutc(*p++);
				while (((c=*p)=='[' || isdigit(c) || c==';' || c=='-') && k < width ) {
			              WiPutc(*p++) ; k++;j++;
				}
		    }
			else if (*p == C_VERT && n < hlines ) {
					WiSetAttr(oldAttr);
					WiPutc(*p++);
					WiSetAttr(NORM_HDR_ATTR);
					i++;j++;
			}
		   	else {
				WiPutc(*p++);
				i++;j++;
			}
		}
#else
		for (i = 0; i < width; i++, p++)
			WiPutc(*p);
#endif
		WiMoveTo(n + 1, base + width + 1);
#ifdef IDEAWIN
 		WiSetAttr(oldAttr);
		WiPutc('\033');WiPutc('M');				// Reset invalid esc seq.
#else
		WiPutc(sep ? C_DS_RIGHT : C_D_VERT);
#endif		
	} 
#ifdef IDEAWIN
	    if (bScroll) {
			_IRScroll(byCmd);
           	 bScroll = FALSE;
		}
#else
	if (less) {
		WiMoveTo(hlines + 2, 0);
		WiPutc(C_UARROW);
		WiMoveTo(hlines + 2, base + width + 1);
		WiPutc(C_UARROW);
	}
	if (more) {
		WiMoveTo(n, 0);
		WiPutc(C_DARROW);
		WiMoveTo(n, base + width + 1);
		WiPutc(C_DARROW);
	}
#endif
	return OK;
}

TerminalOutput::TerminalOutput(Int f_org, Int c_org, Int height, Int width, 
			   				   attr_type bgr, IFP gcmd)
{
	// connect to WM
#ifndef IDEAWIN
	WiConnect(NULL, ProcArgs()[0], "");
#endif

	last_refresh = 0;
	items = 0;
	ncols = 0;
	base = 0;
	hlines = -1;
	topmarg = (UnShort) f_org;
	leftmarg = (UnShort) c_org;
	pwidth = (UnShort) (width ? width : WiWidth(WiDefPar())) - 2;
	pheight = (UnShort) (height ? height : WiHeight(WiDefPar()) - 1) - 2;
	wp = ERR;
	backgr = bgr;
	getcmd = gcmd ? gcmd : (IFP) WiGetc;
	list = true;
}

TerminalOutput::~TerminalOutput()
{
	if (items) {
		if (wp != ERR) {
			_OutPage(false, pageno > 1);
			if (__GetCmd(false) == OK)
				WiDelete(WICURRENT);
		}
		if (fp) {
			fclose(fp);
			fp = NULL;
		}
#ifndef HAVE_TMPFILE
		if (tempfile[0]) {
			unlink(tempfile);
			tempfile[0] = 0;
		}
#endif
	}
}


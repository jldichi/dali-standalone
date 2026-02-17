/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: wioutput.c,v 1.1.1.1 1996/04/16 18:52:16 gustavof Exp $
*
*
* DESCRIPTION:
*      Window function Library. Sends the request to WM through 
*      the pipes.
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*WiCursor		 Turn on and off cursor.
*WiDelChar		 Delete a character.
*WiDelLine		 Delete a line.
*WiDelLines		 Delete 'n' lines.
*WiEraEol		 Erase to the end of line.
*WiEraEop		 Erase to the end of page.
*WiEraLine		 Erase the current line.
*WiErase		 Erase the current window .
*WiInsChar		 Insert a character.
*WiInsLine		 Insert a blank line.
*WiInsLines		 Insert 'n' blank lines.
*WiKeyHelp		 Retorna un puntero a string con la descripcion
*WiKHelp		 Print a help for a key.
*WiMoveTo		 Absolute move to the coord.
*WiRMoveTo		 Relative move to the coord.
*WiScroll		 Do 'n' scrolls
*WiSetAttr		 Set current window attribute.
*WiSetScroll	 Sets the scroll region.
*WiWrap			 Disable and enable wraping.
*WiSetBackGr	 Setea el background de la window corriente.
*********************************************************************/

#include <ideafix.h>

/*

*/
void WiScroll(int num)
{
	WiPrintf("%\033[%dy", num);
}

void WiSetAttr(attr_type attr)
{
	/* Split the long in two shorts */
	short low  = (short) (attr & 0x7FFF);
	short high = (short) (attr >> 15);

	WiPrintf("%\033[%d;%dz", high, low);
}

void WiSetBackGr(attr_type attr)
{
	/* Split the long in two shorts */
	short low  = (short) (attr & 0x7FFF);
	short high = (short) (attr >> 15);

	WiPrintf("%\033[%d;%dF", high, low);
}

void WiWrap(bool f)
{
	WiPrintf("\033[7%c",f==TRUE? 'h' :'l');
}

void WiMoveTo(int f, int c)
{
	WiPrintf("\033[%d;%dH", f+1,c+1);
}

void WiRMoveTo(int f, int c)
{
	WiPrintf("\033[%d;%df", f,c);
}

void WiEraEol(void)
{
	WiPrintf("\033[K");
}

void WiEraEop(void)
{
	WiPrintf("\033[J");  /*  erase to end of page */
}

void WiErase(void)
{
	WiPrintf("\033[2J");  /*  erase display */
}

void WiEraLine(void)
{
	WiPrintf("\033[2K");

}

void WiDelLines(int nlin, int pos)
{
	/*delete 'nlin' lines at pos    */

	WiPrintf("\033[%d;%dM",nlin, pos);
}

void WiDelLine(int pos)
{
	/*deletes a line at pos    */

	WiPrintf("\033[%d;%dM",1, pos);
}

void WiInsLines(int nlin, int pos)
{
	/*insert 'nlin' lines at pos    */

	WiPrintf("\033[%d;%dL",nlin, pos);
}

void WiInsLine(int pos)
{
	/*insert a line at pos    */

	WiPrintf("\033[%d;%dL",1, pos);
}


void WiDelChar(int nchar, int col)
{
	/*delete 'nchar' chars at col    */

	WiPrintf("\033[%d;%dP",nchar, col);
}

void WiInsChar(int nchar, int col)
{
	/*insert 'nchar' chars at col    */

	WiPrintf("\033[%d;%d(",nchar, col);
}

void WiCursor(int f)
{
	/*cursor mode :on(h) - off(l) */

	WiPrintf("\033[5%c", f ? 'h':'l');
}

void WiSetScroll(int top, int bot, int left, int right)
{
	/*	set scroll region   */

	WiPrintf("\033[%d;%d;%d;%dr",top+1,bot+1,left+1,right+1);
}

void WiKHelp(UChar key)
{
	WiPrintf( "\033[%dk", (int)key);
}

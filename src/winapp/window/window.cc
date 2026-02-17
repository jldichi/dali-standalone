/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: window.cc,v 1.3 2004/09/29 21:07:07 damianm Exp $
*
* DESCRIPTION
* Implementation of window class
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <string.h>

#include <ifound.h>
#include <winapp/window.h>
#include <ifound/stdc.h>

// static methods and variables

Window *Window::defWinPar = SCREEN;
Window *Window::headwin = NULL;
bool Window::wmConnection = false;
Int Window::currwin = ERR;
Window *Window::rootWindow_ = NULL;

// private methods 

void Window::connect()
{
#ifndef __NT__
	assert(!wmConnection && !rootWindow_);
#endif
	
	String tty = getTty();
	String module = argVal(0);

	WiConnect((char *) toCharPtr(tty), (char *) toCharPtr(module), NULL);

	wmConnection = true;

	rootWindow_ = tr_new Window;
	rootWindow_->win = SCREEN;
	currwin = SCREEN;
}

void Window::createWindow(short wi, short f_org, short c_org, short nfils, 
						  short ncols, Int border, const char *label, 
						  Int backgr)
{
	idCheck();

	if (!wmConnection) 
		connect();
	
	currwin = win = WiCreate(wi, f_org, c_org, nfils, ncols, 
							 border, (char *)  label, attr_type(backgr));
    
	// insert the window
	next = headwin;
	headwin = this;
}

void Window::destroyWindow()
{
	idCheck();

	WiDelete(win);

	// remove window from list
	if (headwin == this)
		headwin = next;
	else {
		Window *aux;
		for (aux = headwin; aux->next && aux->next != this;
				aux = aux->next)
			;
		aux->next = aux->next->next;
	}	

	// I have more windows obtain the current
	if (headwin != NULL) 
		currwin = WiCurrent();

	// we are no longer active
	win = ERR;
}

void Window::display(Int fo, Int co, Int nf, Int nc, Int border, 
			 const String &label, Int backgr)
{
	idCheck();

	if (win == -1) {
		createWindow(defWinPar ? defWinPar->win : SCREEN,
					 (short)fo, (short)co, (short)nf, (short)nc, 
					 border, toCharPtr(label) , backgr);
	}
}

void Window::display(Window *parent, Int fo, Int co, Int nf, Int nc, 
	Int border, const String &label, Int backgr)
{               
	idCheck();

	assert(parent != NULL);
	
	if (win == -1)
		createWindow(parent->win, short(fo), short(co), short(nf), 
					short(nc), border, toCharPtr(label) , backgr);
}

void Window::unDisplay()
{
	idCheck();

	if (win != -1)
		destroyWindow();
}

// public methods

Window::Window(Window &wi, Int f_org, Int c_org, Int nfils, Int ncols,
			   Int border, String label, Int backgr)
{
	idStart();
	
	createWindow(wi.win, (short) f_org, (short) c_org, 
			(short) nfils, (short) ncols, border, toCharPtr(label), backgr); 
}

Window::Window(Int f_org, Int c_org, Int nfils, Int ncols,
			   Int border, String label, Int backgr)
{
	idStart();
		
	createWindow(defWinPar ? defWinPar->win : SCREEN,
			(short) f_org, (short) c_org,
			(short) nfils, (short) ncols, border, 
			toCharPtr(label), backgr);
}

Window::Window(Int nfils, Int ncols, Int border, String label, 
		Int backgr)
{   
	idStart();
	
	createWindow(defWinPar ? defWinPar->win : SCREEN, -1, -1,
			(short) nfils, (short) ncols, border, toCharPtr(label), 
			backgr);
}

Window::Window()
{
	idStart();

	win = ERR;
}

Window::~Window()
{
	idCheck();

	if (win > 0)
		destroyWindow();

	idStop();
}

window Window::switchTo(window wi)
{
	window prevwin = WiSwitchTo(wi);
	if (prevwin == ERR)
		return ERR;
	assert(prevwin == currwin);
	currwin = wi;
	return prevwin;
}

bool Window::front()
{
	idCheck();

	return switchTo(win) == ERR ? false : true;
}	

void Window::flush()
{
	WiFlush();
}

void Window::refresh()
{
	assert(wmConnection);
	
	WiFlush();

	WiRefresh();
}

void Window::redraw()
{
	assert(wmConnection);

	WiRedraw();
}

UnChar Window::getKey()
{
	idCheck();

	if (currwin != win)			
		if (!front()) return 0;

	WiFlush();

	return WiGetc();
}

Window & Window::operator>>(UnChar &c)
{
	idCheck();
	c = getKey();
	return *this;		
}

Window &Window::operator<<(const String &s)
{
	if (s)
		return operator<<(toCharPtr(s));
	return operator<<("");
}

Window &Window::operator<<(const Num &n)
{
	if (currwin != win)			
		if (!front()) return *this;

	WiPrintf("%N", &n);

	return *this;
}

Window &Window::operator<<(const char *s)
{
	if (currwin != win)			
		if (!front()) return *this;

	WiPrintf("%s", s);

	return *this;
}

Window &Window::operator<<(char c)
{
	if (currwin != win)			
		if (!front()) return *this;

	WiPrintf("%c", c);

	return *this;
}

Window &Window::operator<<(UnChar c)
{
	if (currwin != win)			
		if (!front()) return *this;

	WiPrintf("%c", c);

	return *this;
}

Window &Window::operator<<(int i)
{
	return operator<<(Int(i));
}

Window &Window::operator<<(Int l)
{
	if (currwin != win)			
		if (!front()) return *this;

	WiPrintf("%ld", l);

	return *this;
}

bool Window::keyReady()
{
	idCheck();

	return bool(WiKeyReady());
}

void Window::setBorder(Int border, const String &label)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	// characters that may be left in the buffer
	WiFlush();

	WiSetBorder(border, (char *) toCharPtr(label));
}

#define MAX_KEY_DESC	16
String &Window::keyHelp(UnChar key, String &b)
{
	static char buff[MAX_KEY_DESC];
	
	if (!wmConnection) 
		connect();

	b = String((char*)WiKeyHelp(key, (UnChar*)buff));

	return b;
}

void Window::scroll(Int num)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiScroll(num);
}

void Window::setAttr(Int attr)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiSetAttr(attr);
}

void Window::setBackGr(Int attr)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiSetBackGr(attr);
}


void Window::goTo(Int f, Int c)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiMoveTo(f, c);
}

void Window::eraseEol()
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiEraEol();
}

void Window::erase()
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiErase();
}

void Window::delLines(Int nlin, Int pos)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiDelLines(nlin, pos);
}

void Window::insLines(Int nlin, Int pos)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiInsLines(nlin, pos);
}

void Window::delChar(Int nchar)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiDelChar(nchar, 0);
}

void Window::insChar(Int nchar)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiInsChar(nchar, 0);
}
	
void Window::cursor(bool f)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiCursor(f);
}

void Window::wrap(bool f)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiWrap(f);
}

void Window::setScroll(Int top, Int bot, Int left, Int right)
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiSetScroll(top, bot, left, right);
}

Window *Window::setDefPar(Window *wi)
{
	idCheck();

	Window *aux = defWinPar;
	defWinPar = wi;
	return aux;
}

void Window::beep()
{
	idCheck();

	if (currwin != win)			
		if (!front()) return;

	WiBeep();
}

Int Window::height()
{
	idCheck();

	return WiHeight(win);
}

Int Window::width()
{
	idCheck();

	return WiWidth(win);
}

Int Window::line()
{
	idCheck();

	return WiLine(win);
}	

Int Window::column()
{
	idCheck();

	return WiCol(win);
}	

Int Window::orgCol()
{
	idCheck();

	return WiOrgCol(win);
}

Int Window::orgRow()
{
	idCheck();

	return WiOrgRow(win);
}

Window *Window::rootWindow()
{
	if (! wmConnection) connect();
	assert(rootWindow_ != NULL);
	return rootWindow_;
}

// core field reading method

#define MAX_INPUT_LENGTH	512

Int Window::getField(Int t, String &datum, Int opt, Int length,
				Int olength, Int ndec, const String &tstmask, 
				const String &omask)
{
	idCheck();
	static char buff[MAX_INPUT_LENGTH];
	
	if (currwin != win)
		if (!front()) 
			return ERR;

	assert(datum.length() < MAX_INPUT_LENGTH && length < MAX_INPUT_LENGTH);
		
	if (datum != NULL_STRING)
		strcpy(buff, (char*)toCharPtr(datum));
	else
		buff[0] = '\0';
			
	char *om = "";
	char *tm = "";
	
	if (omask != NULL_STRING)
		om = (char*)toCharPtr(omask);		
	if (tstmask != NULL_STRING)
		tm = (char*)toCharPtr(tstmask);		

	Int ret = WiGetField(t, (UnChar*)buff, opt, length, olength, ndec, 
						 tm, om);
	
	datum = buff;
	return ret;
}

/*
	Interactive move method
*/
bool Window::move()
{
	idCheck();

	return WiMove(win);
}

/*
	Interactive resize method
*/
bool Window::resize()
{
	idCheck();

	return WiResize(win);
}

void Window::setRaw(Int val)
{
	if (!wmConnection) 
		connect();
    
    WiSetRaw(val);
}

void Window::setServices(bool val)
{
	if (!wmConnection) 
		connect();
    
    WiSetServices(val);
}

int Window::execCommand(char *titulo, char *cmd)
{
	if (!wmConnection) 
		connect();

	return WiExecCmd(titulo, cmd);
}
	
bool Window::execService(const String &name)
{
	if (!wmConnection) 
		connect();

	return WiService((char*)toCharPtr(name));
}

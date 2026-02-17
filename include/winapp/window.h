/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: window.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION
* Class definitions for window type.
*********************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

// toDo: sacar estos dos include's!!!
#include <ideafix.h>
#include <cfix.h>

#include <ixwi/wi.h>
#include <ifound/itypes.h>

#ifdef __GNUC__
#pragma interface
#endif

class Window {
	useIdCheck(Window_id);

	Window(const Window &);
	
	window win;						// window descriptor

	static Window *defWinPar;		// default parent window
	static Window *headwin;			// head used for window list
	static bool wmConnection;		// if I'm already connected with the wm
	static Int currwin;				// current window
	static Window *rootWindow_;		// screen window

	// pointer to the next window
	Window *next;

	// crea window 
	void createWindow(short wi, short f_org, short c_org, 
					  short nfils, short ncols, Int border, 
					  const char *label, Int backgr);
	void destroyWindow();

	// connect to the wm
	static void connect();
	static window switchTo(window wi);

public:

	Window(Window &wi, Int f_org, Int c_org, Int nfils, Int ncols,
		   Int border, String label, Int backgr);
	Window(Int f_org, Int c_org, Int nfils, Int ncols, Int border, 
		   String label, Int backgr);
	Window(Int nfils, Int ncols, Int border, String label, Int backgr);
	Window();
	virtual ~Window();			


	// get key method
	UnChar getKey();
	Window &operator>>(UnChar &c);

	// output methods
	Window &operator<<(const String &s);
	Window &operator<<(const Num &n);
	Window &operator<<(const char *s);
	Window &operator<<(char c);
	Window &operator<<(UnChar c);
	Window &operator<<(int i);
	Window &operator<<(Int l);
		
	bool front();

	void scroll(Int num);
	void setAttr(Int attr);
	void setBackGr(Int attr);
	void goTo(Int f, Int c);
	Window &operator()(Int f, Int c) { idCheck(); goTo(f, c); return *this; }
	void eraseEol();
	void erase();
	void delLines(Int nlin=1, Int pos = ERR);
	void insLines(Int nlin=1, Int pos = ERR);
	void delChar(Int nchar=1);
	void insChar(Int nchar=1);
	Window *setDefPar(Window *wi);
	void beep();
	void cursor(bool f);
	void wrap(bool f);
	void setBorder(Int border, const String &label);

	Int height();
	Int width();
	Int orgCol();
	Int orgRow();
	Int line();
	Int column();

	static String &keyHelp(UnChar key, String &b);

	static Window *rootWindow();

	static void flush();
	static void refresh();
	static void redraw();

	void display(Int fo, Int co, Int nf, Int nc, Int border, 
				 const String &label, Int backgr);
	void display(Window *parent, Int fo, Int co, Int nf, Int nc, 
		Int border, const String &label, Int backgr);

	void unDisplay();
	bool hidden()		{ idCheck();  return (win == ERR); }

	Int getField(Int, String &, Int, Int, Int, Int, 
				 const String &, const String &);

	bool move();
	bool resize();
	bool keyReady();
	
	static void setServices(bool value);
	static bool execService(const String &name);
	static int execCommand(char *titulo, char *cmd);
	static void setRaw(Int value);

	void setScroll(Int top, Int bot, Int left, Int right);
};

#endif

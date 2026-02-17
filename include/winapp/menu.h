/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: menu.h,v 1.2 1998/05/19 20:56:22 eduardoc Exp $
*
* DESCRIPTION
* Class definitions for menu type.
*********************************************************************/

#ifndef MENU_H
#define MENU_H

#ifdef __GNUC__
#	pragma interface
#endif

#include <winapp/winobj.h>
#include <winapp/infopan.h>

// max number of items for a menu
#define MAX_ITEMS			20
#define MIN_INFO_LENGTH		5

// character that represents a sub-menu
#define SUBMENU	C_RARROW

class PoppableMenu;
#ifdef USES_OLD_HELP_MENU_STYLE
class Help;
#endif

class MenuItem {

	useIdCheck(MenuItem_id);

// private:

	String text;			// texto de la opcion
	String stext;			// texto secundario de la opcion
	Int attr;				// display attribute

	Int nhkey;				// subindice de la hot key
    UnChar lastkey;
    
	enum {
		MENU,
		RCOD,
		FUNC,
		LINE
	} t;

	union {					// union para los <> tipos
		PoppableMenu *menu;
		Int  rcod;
		LFP  func;
	};

	void setHotKey(char hk);

public :
	
	// constructores

	MenuItem(const MenuItem &mi);
	MenuItem(const String &s, char hk = HKSEQ, Int a = A_NORMAL);
	MenuItem(const String &s, Int cod, char hk = HKSEQ, Int a = A_NORMAL);
	MenuItem(const String &s, PoppableMenu &m, char hk = HKSEQ, Int a = A_NORMAL);
	MenuItem(const String &s, LFP f, char hk = HKSEQ, Int a = A_NORMAL);

	~MenuItem() { idCheck();  idStop(); }
	
    MenuItem &operator=(const MenuItem &i);
                              
	String &getText()		{ idCheck(); return text;					   }
	String &getSecText()	{ idCheck(); return stext;					   }
	Int	   getDspAttr()	const { idCheck(); return attr;					   }
	Int	   getNHKey() const	{ idCheck(); return nhkey;				 	   }

	PoppableMenu *subMenu()	{ idCheck(); return t == MENU ? menu : (PoppableMenu*)NULL;   }
	Int retCod()			{ idCheck(); return t == RCOD ? rcod : ERR;  }
	LFP retFunc()			{ idCheck(); return t == FUNC ? func : NULL;   }

	bool isMenu()			{ idCheck(); return t == MENU;				   }
	bool isCod()			{ idCheck(); return t == RCOD;				   }
	bool isFunc()			{ idCheck(); return t == FUNC;				   }
	bool isLine()			{ idCheck(); return t == LINE;				   }

	Int execute();			// funcion de ejecucion
	Int lastKey() const		{ idCheck(); return lastkey; }

};

class Menu : public WinObj {

// private:

	Menu(const Menu &);
	
protected:

	MenuItem *items[MAX_ITEMS];

	// selected option
	Int opt;
	// number of items
	Int nitems;
	// current item
	Int curritem;
	// current item when leaving menu
    Int previtem;
    // whether last item is to remain selected next time we execute
    bool stayOnOption;
    // display even when there are no options
	bool dspalways;

	// menu attributes
	// option attribute
	Int oattr;
	// hot key attribute
	Int hkattr;
	// associated help
#ifdef USES_OLD_HELP_MENU_STYLE
	Help *assocHelp;		// help asociado
#endif	

public:

	// constructors & destructors
	Menu(Int fo=ERR, Int co=ERR);
	virtual ~Menu();

	// methods to add items
	virtual Menu &addItem(const MenuItem &i);
	Menu &operator<<(const MenuItem &i);
	Menu &operator<<(const String &s);

	// attribute setting methods
	void setOAttr (Int a)			{ idCheck(); oattr  = a; }
	void setHKAttr(Int a)			{ idCheck(); hkattr = a; }

	// misc methods
	Int  currOpt() const			{ idCheck(); return opt; }
	void setDspAlways(bool f)		{ idCheck(); dspalways = f; }

	// assign secondary text
	void setSecText(const String &s, Int c);

#ifdef USES_OLD_HELP_MENU_STYLE
	// asigno el help dado al menu
	void setHelp(Help *hlp);
#endif	

	// current or pre-selected menu option
	void setCurrent(Int co = 0, bool stay = false);
	
	// virtual methods
	virtual void unDisplay();
	virtual Int execute(UnChar c = 0)=0;
	virtual void newCurrItem(Int i);
};

class PoppableMenu: public Menu {

// private:

	PoppableMenu(const PoppableMenu &);

protected:

	bool pulldown;			// executed from pulldown menu
	bool scursor;			// exit if cursor(left|right) is pressed
	bool hilite;			// Should highlight chars preceeded by ~
	bool leavehilit;		// Should leave selected option hilit at exit

	virtual void calcDim(Int &nf, Int &nc);
	virtual bool incCurrItem(Int i = 1);
	virtual bool decCurrItem(Int i = 1);
	virtual void fillWindow(bool leaving = false);
	
	// incremental find mode
	virtual bool incFind()	{ idCheck(); return false; } 

public :

	// constructors & destructors

	PoppableMenu(Int fo=ERR, Int co=ERR, bool hi = true);
	PoppableMenu(const String &l, Int fo=ERR, Int co=ERR, bool hi = true);
	virtual ~PoppableMenu();

	// misc methods

	void setPullDown(bool f)	{ idCheck(); pulldown = f; }
	bool getPullDown()			{ idCheck(); return pulldown; }
	void allowMovedKeys()		{ idCheck(); scursor = true; }
	void leaveHilit()			{ idCheck(); leavehilit = true; }

	// virtual methods

	virtual bool display(Int fo=ERR, Int co=ERR);
	virtual Int execute(UnChar c = 0);

};


bool canRead(Int mode, Int fileuid, Int filegid, Int ouruid,
		GidT *ourgids, Int nourgids);
bool canWrite(Int mode, Int fileuid, Int filegid, Int ouruid,
		GidT *ourgids, Int nourgids);
bool canSearch(Int mode, Int fileuid, Int filegid, Int ouruid,
		GidT *ourgids, Int nourgids);

class PullDownMenu : public Menu {

// private :

	bool right;

	void fillWindow();
	PullDownMenu(const PullDownMenu&);
	
public :

	// constructors & destructors

	PullDownMenu(Int fo=ERR, Int co=ERR, Int nf=1, Int nc=0);
	virtual ~PullDownMenu();

	// misc methods

	void setRight(bool f)		{ idCheck(); right = f;    }
	bool getRight()				{ idCheck(); return right; }

	// virtual methods

	virtual Menu &addItem(const MenuItem &i);

	virtual bool display(Int fo=ERR, Int co=ERR);
	virtual Int execute(UnChar c = 0);

};

class ScrollMenu : public PoppableMenu {

// private:

	// routine that feeds this menu
	SFPI fpread;
	// offset of first item displayed
	Int floor;

	void newSMenu(SFPI f, Int nf, Int nc);

	virtual void calcDim(Int &nf, Int &nc);
	virtual bool incCurrItem(Int i=1);
	virtual bool decCurrItem(Int i=1);
	virtual void fpRead(Int i, String &str, String &secStr, Int &attr);

	// metodos ocultados por que no tienen sentido
	virtual Menu &addItem(const MenuItem &i) 
		{ idCheck(); return PoppableMenu::addItem(i);    }
	Menu &operator<<(const MenuItem &i)
		{ idCheck(); return PoppableMenu::operator<<(i); }
	Menu &operator<<(const String &s)		 
		{ idCheck(); return PoppableMenu::operator<<(s); }
	
	ScrollMenu(const ScrollMenu&);
	
protected:

	// fill item array
	void fillItems();
	void refreshItems();

	// fill window with items
	virtual void fillWindow(bool leaving = false);
	bool incFind();

public :

	// constructors & destructors

	ScrollMenu(const String &s, SFPI f, Int nf, Int nc, 
			Int fo=ERR, Int co=ERR, bool hi = false);
	ScrollMenu(SFPI f, Int nf, Int nc, Int fo=ERR, Int co=ERR,
			bool hi = false);
	virtual ~ScrollMenu();

	// misc methods

	void textOpt(Int i, String &str, String &secStr);
	void reFill();                                   
	void refresh();
	
	// No me gusta, por que no utilizar currOpt() ? (bruno)
	Int prevItem() const { return floor + previtem; }

	// virtual methods

	virtual bool display(Int fo=ERR, Int co=ERR);
	virtual void unDisplay();
};


class StringMenu : public ScrollMenu {

// private:

	virtual void fpRead(Int, String&, String&, Int &attr);  // funcion de busqueda
	StringMenu(const StringMenu&);
protected:
	Int dim;
	const String *strArray;

public:

	StringMenu(	const String &l, const String *s, Int n,
				Int fo=ERR, Int co=ERR, Int width = ERR);
	virtual ~StringMenu() { idCheck(); }
	virtual bool display(Int fo=ERR, Int co=ERR);
};


class ExtStringMenu : public StringMenu {
	InfoPanel *info_;
	bool dispinfo_;
	virtual void fpRead(Int nro, String &str, String &secStr, Int &attr);
public:
	ExtStringMenu(const String &l, const String *s, Int hz, 
					Int ni = MIN_INFO_LENGTH,
					Int fo=ERR, Int co=ERR, Int width = ERR);
	~ExtStringMenu() { idCheck(); delete info_; }
  	void dispinfo();
  	virtual bool display(Int fo = ERR, Int co = ERR);	
	virtual void newCurrItem(Int i);
	void showInfo(Int n);
};

inline void ExtStringMenu::dispinfo()
{
	idCheck();
	dispinfo_ = dispinfo_ == true ? false : true;
	if (!dispinfo_)	info_->unDisplay();
	else
		info_->display();
}

// default for rows & columns
#define	DFIL		 10
#define	DCOL		 16

// max current working directory length
#define	LONG_CWD	300


class DirView;

class FileMenu : public ScrollMenu {

	friend class DirMenu;

// private:

	String cwd;
	String dir;						   // directorio de busqueda
	String file;					   // expresion regular de busqueda
	DirView &dv;

	virtual void fpRead(Int, String&, String&, Int &attr);  // funcion de busqueda
	FileMenu(const FileMenu&);
	
public:

	// constructors & destructors

	FileMenu(const String &l, const String &f, Int nf=DFIL, 
			 Int nc=DCOL, Int fo=ERR, Int co=ERR, 
			 const String &thecwd=NULL_STRING);
	FileMenu(const String &l, Int nf=DFIL, Int nc=DCOL, 
			 Int fo=ERR, Int co=ERR,
			 const String &thecwd=NULL_STRING);
	FileMenu(Int nf=DFIL, Int nc=DCOL, Int fo=ERR, Int co=ERR,
			 const String &thecwd=NULL_STRING, 
			 const String &pfile=NULL_STRING);

	virtual ~FileMenu();

	// metodos varios

	virtual Int execute(UnChar c = 0);
	void setDir (const String &s);
	void setFile(const String &s);

	const String retDir()	    { idCheck(); return dir;		}
	const String retCwd()	    { idCheck(); return cwd;		}
};

class DirMenu : public ScrollMenu {

// private:
                       
	String cwd;                       
	String dir;					  // directorio de busqueda
	FileMenu *fmenu;			  // FileMenu asociado	
	DirView &dv;

	virtual void fpRead(Int, String&, String&, Int &attr);  // funcion de busqueda
	DirMenu(const DirMenu&);
	
public:

	// constructores y destructores

	DirMenu(const String &l, FileMenu &fm, Int nf=DFIL, Int nc=DCOL,
			Int fo=ERR, Int co=ERR, const String &thecwd=NULL_STRING);
	DirMenu(FileMenu &fm, Int nf=DFIL, Int nc=DCOL, 
			Int fo=ERR, Int co=ERR, const String &thecwd=NULL_STRING);
	DirMenu(Int nf=DFIL, Int nc=DCOL, Int fo=ERR, Int co=ERR,
			const String &thecwd=NULL_STRING);
	virtual ~DirMenu();

	// metodos varios
    
    String 	buildDirStr(const String &s);
	void 	setDir (const String &s);
	const 	String retDir()		{ idCheck(); return dir; }
	const 	String retCwd()	    { idCheck(); return cwd; }

	// metodos virtuales

	virtual Int execute(UnChar c = 0);
};

#endif

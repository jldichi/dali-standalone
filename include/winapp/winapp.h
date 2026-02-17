/********************************************************************
* Copyright	(c)	1994 InterSoft Co.	All	Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright	notice above does not evidence any
* actual or	intended publication of	such source	code.
*
* $Id: winapp.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Definition of WinApp class
*********************************************************************/
#ifndef WINAPP_H
#define WINAPP_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/oparser.h>
#include <winapp/wincmd.h>
#include <winapp/cmdbind.h>
#include <winapp/strhis.h>
#include <winapp/msgbox.h>

#define MAX_COMMAND	255

// Message files
#define WINAPPMSG		"winapp"

class PullDownMenu;
class PoppableMenu;
class IqlApp;

class WinApp {

	useIdCheck(WinApp_id);

// Attributes
private:
	static MsgTable	*wappMsg_;			// WinApp's message table
	static MsgTable	*wappHlpMsg_;		// WinApp's error message table
	static MsgTable	*wappHlpLabelMsg_;	// WinApp's display message table
	static MsgTable	*wappErrMsg_;		// WinApp's error message table
	static MsgTable	*wappDispMsg_;		// WinApp's display message table

	Window *baseWindow;	  			// Parent window
	Desktop	*desktop;				// Application's desktop
	String workingDir;				// Current working directory of the app
	
	CmdId getCmdId(EventId evid);

	void operator=(const WinApp &);
		
protected:
	PullDownMenu *menu;				// Main Menu
	CmdBinding *appBinding;			// View-independent command binding
	CmdBinding *defViewBinding;		// Default command binding of a view
	CmdBinding *helpBinding;		// Command binding for help views
	CmdBinding *specialKeyBinding;	

public:
	static gtype(List, String) *clipboard;

	// Constructor and destructor
		WinApp(const String &appname);
		virtual ~WinApp();

	// Initializes resources used by the application
		bool init();

	// Creates the menu and loads submenues
		virtual void loadMenus();

	// Executes application
		virtual Int execute();
	
	// Key binding methods
		EventId sense(UnShort &c);

		CmdBinding *getAppBinding(void);
		CmdBinding *getDefViewBinding(void);
		CmdBinding *getSpecialKeyBinding(void);
		CmdBinding *getHlpBinding(void);

		virtual void bindCmds();

	// Help
		void addHelpView(const String &key);
		virtual String getHlpContents(const String &key);
		virtual String getHlpLabel(const String &key);

	// Desktop related methods
		void addView(View* v, CmdBinding *cb = NULL, Int pfo = ERR, 
						Int pco = ERR, Int pfil = ERR, Int pcol = ERR);

		Desktop* getDesktop(void);

	// Creates a new view
		virtual void newFile(const String &) {}
		
	// Opens an existing file in a new view
		virtual void openFile(const String &) {} 
		
	// Loads an existing file into the current view
		virtual void loadFile(const String &) {}
		
	// Saves the current view
		virtual void saveFile(const String &) {} 

	// Miscellaneous methods
		Int dialogLine(const String &l, String &v, StrHistory *h = NULL);
		const String &getWorkingDir() const { idCheck(); return workingDir; }
		UnChar getKey();
		void beep();

	// Static methods to access messages
		static String wappMsg(const String &s);
		static String wappErrMsg(const String &s);
		static String wappDispMsg(const String &s);
		static String wappHlpMsg(const String &s);
		static String wappHlpLabelMsg(const String &s);

	// Dynamic-cast for known derived applications
		virtual IqlApp* iqlApp() { return NULL; } 
	// virtual DaliApp* daliApp() { return NULL; } 
};

inline CmdBinding *WinApp::getAppBinding()
{
	idCheck(); 
	
	assert(appBinding != NULL); 
	return appBinding;
}

inline CmdBinding *WinApp::getDefViewBinding()
{
	idCheck(); 
	
	assert(defViewBinding != NULL); 
	return defViewBinding;
}

inline CmdBinding *WinApp::getSpecialKeyBinding()
{
	idCheck(); 
	
	assert(specialKeyBinding != NULL); 
	return specialKeyBinding;
}

inline CmdBinding *WinApp::getHlpBinding()
{
	idCheck(); 
	
	assert(helpBinding != NULL); 
	return helpBinding;
}

inline String WinApp::wappMsg(const String &s)
{
	if (wappMsg_ == NULL)
		wappMsg_ = new MsgTable(WINAPPMSG);
	return wappMsg_->find(s); 
}

inline String WinApp::wappDispMsg(const String &s)
{
	if (wappDispMsg_ == NULL)
		wappDispMsg_ = new MsgTable(WINAPPMSG, "disp");
	return	wappDispMsg_->find(s);
}

inline String WinApp::wappErrMsg(const String &s)
{
	if (wappErrMsg_ == NULL)
		wappErrMsg_ = new MsgTable(WINAPPMSG, "error");
	return	wappErrMsg_->find(s);
}

inline String WinApp::wappHlpMsg(const String &s)
{
	if (wappHlpMsg_ == NULL)
		wappHlpMsg_ = new MsgTable(WINAPPMSG, "help", true);
	return	wappHlpMsg_->find(s);
}

inline String WinApp::wappHlpLabelMsg(const String &s)
{
	if (wappHlpLabelMsg_ == NULL)
		wappHlpLabelMsg_ = new MsgTable(WINAPPMSG, "disp", true);
	return	wappHlpLabelMsg_->find(s);
}

#endif

/********************************************************************
* Copyright	(c)	1994 InterSoft Co.	All	Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright	notice above does not evidence any
* actual or	intended publication of	such source	code.
*
* $Id: winapp.cc,v 1.2 2004/09/29 21:07:33 damianm Exp $
*
* DESCRIPTION: Implementation of WindowApp class
*********************************************************************/

#ifdef __GNUG__
#	pragma implementation
#endif

#include <ifound.h>
#include <stdio.h>
#include <unistd.h>
#include <local/strfile.h>

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <ifound/str.h>

// WinApp includes
#include <winapp/winapp.h>
#include <winapp/strhis.h>
#include <winapp/window.h>
#include <winapp/msgbox.h>
#include <winapp/cell.h>
#include <winapp/label.h>
#include <winapp/cmdbind.h>
#include <winapp/text.h>
#include <winapp/menu.h>
#include <winapp/view.h>
#include <winapp/hlpview.h>
#include <winapp/filecmd.h>
#include <winapp/blkcmd.h>
#include <winapp/helpcmd.h>
#include <ifound/main.h>

void bindPosCmds(CmdBinding *cb);
void bindEditCmds(CmdBinding *cb);
void bindSysCmds(CmdBinding *cb);

MsgTable	*WinApp::wappMsg_ 		 = NULL;
MsgTable	*WinApp::wappErrMsg_ 	 = NULL;
MsgTable	*WinApp::wappDispMsg_ 	 = NULL;
MsgTable	*WinApp::wappHlpMsg_ 	 = NULL;
MsgTable	*WinApp::wappHlpLabelMsg_= NULL;

List(String) *WinApp::clipboard	= NULL;

class libwapp {
public:
	static void foo();
};

static void foolink()
{
	libwapp::foo();
}

WinApp::WinApp(const String &appname)
{
	idStart();

	foolink();
	appBinding			= tr_new CmdBinding();
	helpBinding			= tr_new CmdBinding();
	defViewBinding		= tr_new CmdBinding();
	specialKeyBinding	= tr_new CmdBinding();
	desktop				= tr_new Desktop();
	menu 				= tr_new PullDownMenu(0, 0);
	clipboard 			= tr_new List(String)(NULL_STRING);

	// Window manager initialization calls
	Window::setRaw(WM_PURERAW);

	Int baseWinHeight	= Window::rootWindow()->height() - 2;
	Int baseWinWidth  	= Window::rootWindow()->width();
	baseWindow			= tr_new Window(1, 1,	baseWinHeight, baseWinWidth,
									NO_BORDER,	NULL_STRING, A_NORMAL);


    // Build and display fancy background
	String deskLine(baseWinWidth, char(C_SHADOW));

	for (Int k = 1; k < baseWinHeight; k++)
		*baseWindow << deskLine;

	baseWindow->wrap(false);
	*baseWindow << deskLine;
	baseWindow->wrap(true);

	baseWindow->goTo(0, 0);
    Window::refresh();

	// Set view's default parent window
	View::setViewPar(baseWindow);

	// Get the current working directory
	char aux[LONG_CWD];
	getcwd(aux, LONG_CWD -	1);
	workingDir = aux;

	idCheck();
}

bool WinApp::init()
{          
	idCheck();

	bindCmds();
	loadMenus();
	menu->display(0,0);
		
	return true;
}

WinApp::~WinApp()
{
	idCheck();

	tr_delete(baseWindow);
	tr_delete(clipboard);
	tr_delete(desktop);
	tr_delete(menu);

	tr_delete(helpBinding);
	tr_delete(appBinding);
	tr_delete(defViewBinding);
	tr_delete(specialKeyBinding);

	idStop();
}

Desktop* WinApp::getDesktop(void)
{
	idCheck();
	
	assert(desktop != NULL);
	
	return desktop;
}

Int WinApp::execute()
{
	idCheck();

	UnShort chr;
	EventId evid;
	CmdId cmdId = ERR;

	forever {
		// Get the event from the sensor
		evid = sense(chr);

		if (evid == ERR)
			break;

		if ((cmdId = getCmdId(evid)) == ERR) {
		    beep();
		    continue;                                      
		}				    
		// Execute the command
		assert(WinCmd::getCmd(cmdId) != NULL);

		WinCmd::getCmd(cmdId)->execute(UnChar(chr), this);
	}
    
	return OK;
}   

EventId WinApp::sense(UnShort &c)
{ 
	EventId evid = ERR;
	bool helpMode = false;
	c = 0;
	forever {                     //Eventualmente : getKeyType()
		evid = specialKeyBinding->getCmdId(c = COMPOSE(c, getKey()));
		
		switch (evid) {
			case HELP_KEY:
				beep();   
				helpMode = !helpMode;
				if (desktop->getCurrView())
					desktop->getCurrView()->chgHelpMode();
				c = 0;
				break;
				
			case MENU_KEY:
				// Here we assume that on error cmd will be set to ERR !
				if (menu->execute(RCHAR(c)) != REJECT_EXIT) {
					evid = menu->currOpt();
					if (menu->lastKey() != K_HELP) {
						return (evid != END_KEY) ? evid : ERR;
					}
					else {
						Int cmdId = getCmdId(evid);
						if (cmdId != ERR) {
						    addHelpView(WinCmd::getName(cmdId));
						}   
					}
				}								
				c = 0;
				break;

			case END_KEY:  
					c  = 0;
					return ERR;

			case ALTER_KEY:  
					break;                              

			default: {
				if(!helpMode) 
					return c;
				
				if (desktop->getCurrView())
					desktop->getCurrView()->chgHelpMode();

				Int cmdId = getCmdId(c);
				if (cmdId != ERR) 
				    addHelpView(WinCmd::getName(cmdId));

				helpMode = false;
			    c = 0;
			    break;
			}
		}
	} 

	assert(0);
	
	return ERR;
}

CmdId WinApp::getCmdId(EventId evid)
{
	CmdId cmdId = ERR;

	if (desktop->getActiveViews())
		cmdId = desktop->getCurrView()->getCmdBinding()->getCmdId(evid);
	
	if (cmdId == ERR)
		cmdId = appBinding->getCmdId(evid);

	return cmdId;
}   

void WinApp::loadMenus()
{
	idCheck();
}

void WinApp::bindCmds()
{
	idCheck();

	bindSysCmds(appBinding);	
    
    // Set command binding for default views
		bindPosCmds(defViewBinding);
		bindEditCmds(defViewBinding);
		bindBlockCmds(defViewBinding);

    // Set command binding for the help views
		specialKeyBinding->bind(K_HELP, HELP_KEY);
		bindPosCmds(helpBinding);
		helpBinding->bind(BlockBegin::eventId, 		BlockBegin::cmdId);
		helpBinding->bind(BlockEndMode::eventId, 	BlockEndMode::cmdId);
		helpBinding->bind(BlockCopy::eventId,		BlockCopy::cmdId);
		helpBinding->bind(FileClose::eventId, 		FileClose::cmdId);
		helpBinding->bind(HelpOnCmd::eventId, 		HelpOnCmd::cmdId);
		helpBinding->bind(XRefNext::eventId, 		XRefNext::cmdId);
		helpBinding->bind(XRefPrev::eventId, 		XRefPrev::cmdId);
		helpBinding->bind(XRefGo::eventId, 			XRefGo::cmdId);

	// Exit App keys
		specialKeyBinding->bind(COMPOSE(K_CTRLX, 'x'), END_KEY);
		specialKeyBinding->bind(COMPOSE(K_CTRLX, 'X'), END_KEY);

	// Special Keys
		specialKeyBinding->bind(K_CTRLX, ALTER_KEY);
		specialKeyBinding->bind(K_META,  ALTER_KEY);
}

void WinApp::beep()
{
	//desktop->hasViews() ? desktop->beep() : baseWindow->beep();
	
	idCheck();
	
	baseWindow->beep();
}

void WinApp::addView(View* v, CmdBinding *cb, 
					Int pfo, Int pco, Int pfil, Int pcol)
{
	idCheck();
	v->setCmdBinding(cb ? cb : defViewBinding);
	desktop->add(v, pfo, pco, pfil, pcol);
}

void WinApp::addHelpView(const String &key)
{
	// toDo: Check if the help view for this key is already displayed.
	
	idCheck();

    // Get help label
	String label = getHlpLabel(key);
	String contents = getHlpContents(key);
	if (label == HIGH_VALUE || contents == HIGH_VALUE) {
		beep();
    	MsgBox msgbox(libMsg("ERROR"), 
    			toString(wappDispMsg("HELPNOTAVAILABLE"),toCharPtr(key)), 
    			5,50);
				msgbox.execute();
    	return;
	}    	

	HelpView *auxView = (HelpView *) View::existText(label);
	if (auxView == NULL) 
		addView(tr_new HelpView(label, contents, key, this), helpBinding);//,19,2,4,78
	else {
		desktop->setAsCurrent(auxView);
//		auxView->goToBegOfFile();
	}
}

String WinApp::getHlpContents(const String &key)
{   
	idCheck();
	
	return wappHlpMsg(key);
}   

String WinApp::getHlpLabel(const String &key)
{   
	idCheck();
	
	return wappHlpLabelMsg(key);
}   

Int WinApp::dialogLine(const String &l, String &v, StrHistory *h)
{
	idCheck();
	
	// Create & exec Label
	Label dLabel(l, Window::rootWindow()->height(), 1);
	dLabel.display(ERR, ERR);

	// Create & exec StrCell
	Int length = l.length() + 1;
	Int width = Window::rootWindow()->width() - length - 4;
	StrCell dLine(160, width, 0, Window::rootWindow()->height(), length + 3, h);
	
	Int rv = dLine.execute();
	
	if (rv == ACCEPT)
		v = dLine.value(); 

	return rv;
}

UnChar WinApp::getKey()
{
	idCheck();
	UnChar ch;
	return desktop->getActiveViews()
			? desktop->getKey()
			: (*baseWindow >> ch, ch);
}

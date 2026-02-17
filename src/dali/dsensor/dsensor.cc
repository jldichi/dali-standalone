/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dsensor.cc,v 1.3 1998/05/04 14:53:07 mauro Exp $
*
* DESCRIPTION: Implementation of sensor class
*
*********************************************************************/

#include <dsensor.h>
#include <winapp/menu.h>
#include <ifound/hashtab.h>
#include <command.h>
#include <editor.h>
#include <ifound/stdc.h>
#include <ifound/str.h>

#ifdef ISDEBUG
#include <ifound/iddebug.h>
#endif

#ifdef USES_OLD_HELP_MENU_STYLE
#include <edhelp.h>
#endif

// rutinas para la hashtab

static Int cmp(UnShort us1, UnShort us2);
static Int hashVal(UnShort us, Int mc);

declare2(HashTab,UnShort,Int);
implement2(HashTab,UnShort,Int);

#define HASHT(t)	((HashTab(UnShort,Int)*)(t))

_cmds *DaliSensor::cmds = NULL;

// metodos privados

Int DaliSensor::findCmdId(UnShort c)
{
	Int *i = HASHT(tcmds)->find(c);
	return i ? *i : ERR;
}

Int cmp(UnShort us1, UnShort us2)
{
	return us1 - us2;
}

Int hashVal(UnShort us, Int mc)
{
	return us % mc;
}

// metodos publicos

DaliSensor::DaliSensor()
{
	tcmds =	tr_new HashTab(UnShort,Int)(MAX_COMMAND);
	menu =	tr_new PullDownMenu(0, 0);

	sm =	tr_new PoppableMenu;
	mf =	tr_new PoppableMenu;
	me =	tr_new PoppableMenu;
	ms =	tr_new PoppableMenu;
	mo =	tr_new PoppableMenu;

	mt =	tr_new PoppableMenu;
	mw =	tr_new PoppableMenu;

	mrd =	tr_new PoppableMenu;
	mdd =	tr_new PoppableMenu;

#ifdef USES_OLD_HELP_MENU_STYLE
	mh = tr_new PoppableMenu;
	loadHelpMenu();

	sm->setHelp(Editor::editorHelp);
	mf->setHelp(Editor::editorHelp);
	me->setHelp(Editor::editorHelp);
	ms->setHelp(Editor::editorHelp);
	mo->setHelp(Editor::editorHelp);
	mt->setHelp(Editor::editorHelp);
	mrd->setHelp(Editor::editorHelp);
	mdd->setHelp(Editor::editorHelp);
	mw->setHelp(Editor::editorHelp);
	mh->setHelp(Editor::editorHelp);
#endif

	loadSysMenu();
	loadFileMenu();
	loadEditMenu();
	loadSearchMenu();
	loadOptionsMenu();
	loadToolMenu();
	loadRunDebuggerMenu();
	loadDspDebuggerMenu();
	loadWindowMenu();
	loadMenu();

	initCommands();
	menu->display(0,0);
}

DaliSensor::~DaliSensor()
{
	tr_delete(HASHT(tcmds));
	tr_delete(menu);
	tr_delete(sm);
	tr_delete(mf);
	tr_delete(me);
	tr_delete(ms);
	tr_delete(mo);
	tr_delete(mt);
	tr_delete(mw);
	tr_delete(mrd);
	tr_delete(mdd);

#ifdef USES_OLD_HELP_MENU_STYLE
	tr_delete(mh);
#endif

	if (cmds)
		tr_delete_vec(cmds);
}

Int DaliSensor::getEvent(UnShort &c)
{
	Int cmd;
	c = UnShort(Editor::getMode());
	do {
		cmd = findCmdId(c = COMPOSE(c, Editor::getKey()));
		switch (cmd) {
		case ERR:
			Editor::beep();
			c = UnShort(Editor::getMode());
			break;
		case MENU_KEY:
			// Here we assume that on error cmd will be set to ERR !
			if ((cmd = menu->execute(RCHAR(c))) == REJECT_EXIT
					|| (cmd = menu->currOpt()) != END_KEY) {
				c = UnShort(Editor::getMode());
				break;
			}
			// Intentionally fall down
		case END_KEY:
			if (!(Editor::exit())) {
				c = UnShort(Editor::getMode());
				break;
			}
			return ERR;
		case ALTER_KEY:
			break;
		}
	} while (cmd < 0);
	return cmd;
}

Command* DaliSensor::getCmd(Int cmd)
{
	return cmd >= 0 && cmd < MAX_COMMAND ? cmds[cmd].cmd : NULL;
}

const String DaliSensor::getCmdName(Int cmd)
{
	if (cmd >= 0 && cmd < MAX_COMMAND)
		return cmds[cmd].name;
	else
		return NULL_STRING;
}

Int DaliSensor::getCmdId(const char *name)
{
	for (Int i=0; i < MAX_COMMAND && cmds[i].name; i++)
		if (strcmp(cmds[i].name, name)==0) return i;
	return ERR;
}

void DaliSensor::bind(UnShort c, Int ncmd)
{
	HASHT(tcmds)->add(c, ncmd);
}

Int DaliSensor::makeCommand(const char *s, Command *c)
{
	Int i;
	if (!cmds) {
		cmds = tr_new _cmds[MAX_COMMAND];
		for (i=0; i < MAX_COMMAND ; i++)
			cmds[i].name = NULL;
	}

	for (i=0; i < MAX_COMMAND && cmds[i].name; i++)
		if (strcmp(cmds[i].name,s)==0) return i;
	if (i < MAX_COMMAND) {
		cmds[i].name = s;
		cmds[i].cmd  = c;
		return i;
	}
	return ERR;
}

void DaliSensor::initMenuKeys()
{
	String ekey;		// end key
	String hkey;		// home key
	String skey;		// suspend key
	String xkey;		// ctrlx key
	String ukey;		// undo key
	String hlpkey;		// help key
	String ahlpkey;		// appl. help key
	String pkey;		// process key
	String rmvkey;		// remove key
	String attkey;		// attention key
	String inskey;		// insert key
	String enterkey;	// enter key
	String delkey;		// delete key
	String ignkey;		// ignore key

	// descripcion de las teclas de Ideafix
	Window::keyHelp(K_END,		ekey);
	Window::keyHelp(K_META,		hkey);
	Window::keyHelp(K_SUSPEND,	skey);
	Window::keyHelp(K_CTRLX,	xkey);
	Window::keyHelp(K_UNDO,		ukey);
	Window::keyHelp(K_HELP,		hlpkey);
	Window::keyHelp(K_APL_HELP,	ahlpkey);
	Window::keyHelp(K_PROCESS,	pkey);
	Window::keyHelp(K_REMOVE,	rmvkey);
	Window::keyHelp(K_ATTENTION,attkey);
	Window::keyHelp(K_INS,		inskey);
	Window::keyHelp(K_ENTER,	enterkey);
	Window::keyHelp(K_DEL,		delkey);
	Window::keyHelp(K_IGNORE,	ignkey);

	{
		String space =	" ";
		ekey =			space+ekey;
		hkey =			space+hkey;
		skey =			space+skey;
		xkey =			space+xkey;
		ukey =			space+ukey;
		hlpkey =		space+hlpkey;
		ahlpkey	=		space+ahlpkey;
		pkey =			space+pkey;
		rmvkey =		space+rmvkey;
		attkey =		space+attkey;
		inskey =		space+inskey;
		enterkey =		space+enterkey;
		delkey =		space+delkey;
		ignkey =		space+ignkey;
	}

	setSysKeys(hkey, skey);
	setFileKeys(attkey, ahlpkey, hkey, xkey);
	setEditKeys(ukey, hkey, enterkey, inskey, delkey, xkey);
	setSearchKeys(hkey, xkey);
	setOptionsKeys(hkey, xkey);
	setToolKeys(pkey,hkey);
	setWindowKeys(xkey, ignkey, rmvkey, hkey);

#ifdef USES_OLD_HELP_MENU_STYLE
	setHelpKeys(hlpkey, xkey, hkey);
#endif

	setRunDebuggerKeys(xkey, inskey, delkey);
	setDspDebuggerKeys();
}

/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dsensor.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION: Class definitions for sensor type.
*********************************************************************/

#ifndef SENSOR_H
#define SENSOR_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif


class Command;
class PullDownMenu;
class PoppableMenu;
class Window;
class String;

// maxima cantidad de comandos
#define MAX_COMMAND	255

// teclas para la tabla de traduccion
#define ALTER_KEY		-2
#define	MENU_KEY		-3
#define	END_KEY			-4

// compone dos teclas 
#define COMPOSE(a,b)	(((a) << 8) | (b))

// descompone teclas
#define LCHAR(c)		((c) & 0xff00)
#define RCHAR(c)		((c) & 0x00ff)

struct _cmds {
	const char *name;
	Command *cmd;
};

class DaliSensor
{

private:

	// tabla de traduccion de key
	void *tcmds;

	// tabla de commandos
	static _cmds *cmds;

	// menu para el help
	PullDownMenu *menu;			// Main Menu
	PoppableMenu *sm;				// System Menu
	PoppableMenu *mf;				// File Menu
	PoppableMenu *me;				// Edit Menu
	PoppableMenu *ms;				// Search Menu
	PoppableMenu *mo;				// Option Menu
	PoppableMenu *mt;				// Tool Menu
	PoppableMenu *mw;				// Window Menu

#ifdef USES_OLD_HELP_MENU_STYLE
	PoppableMenu *mh;				// Help Menu
#endif

   	// submenus 
   	PoppableMenu *mrd;				// Run - Debugger Menu
   	PoppableMenu *mdd;				// Display - Debugger Menu
   	
	// rutinas que arman el menu;
	void loadMenu();
	void loadSysMenu();
	void loadFileMenu();
	void loadEditMenu();
	void loadSearchMenu();
	void loadOptionsMenu();
	void loadToolMenu();
	void loadWindowMenu();

#ifdef USES_OLD_HELP_MENU_STYLE
	void loadHelpMenu();
#endif

    void loadRunDebuggerMenu();
    void loadDspDebuggerMenu();
    
	// busca la key el la tcmds
	Int findCmdId(UnShort );

	void setSysKeys(const String &hkey, const String &skey);
	void setFileKeys(const String &attkey, const String &ahlpkey, 
					 const String &hkey, const String &xkey);
	void setEditKeys(const String &ukey, const String &hkey, 
					 const String &enterkey, const String &inskey, 
					 const String &delkey, const String &xkey);
	void setSearchKeys(const String &hkey, const String &xkey);
	void setOptionsKeys(const String &hkey, const String &xkey);
	void setToolKeys(const String &pkey, const String &hkey);

	void setWindowKeys( const String &xkey, const String &ignkey,
						const String &rmvkey, const String &hkey);

#ifdef USES_OLD_HELP_MENU_STYLE
	void setHelpKeys(const String &hlpkey, const String &xkey,
			const String &hkey);
#endif

	void setRunDebuggerKeys(const String &xkey, const String &inskey, 
						 const String &delkey);
	void setDspDebuggerKeys();
	
	// inicializa teclas de menues
	void initMenuKeys();             
	
    void operator=(const DaliSensor&) ;
    		DaliSensor(const DaliSensor&) ;

public:

	DaliSensor();
	~DaliSensor();

	// leer un evento
	Int getEvent(UnShort &);

	// obtener un comando
	Command* getCmd(Int cmd);
	const String getCmdName(Int cmd);
	Int 	 getCmdId(const char *);

	// mapeo de comandos
	void bind(UnShort c , Int ncmd);

	// inserta un comando dentro de cmds
	Int makeCommand(const char *, Command *);

	// inicializa el vector tcmds
	void initCommands();
};

#endif

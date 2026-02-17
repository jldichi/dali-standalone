/*********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: wincmd.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Definition of class WinCmd 
*
*********************************************************************/

#ifndef WINCMD_H
#define WINCMD_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#define MAX_COMMAND	255
//#include <winapp/view.h> creo que ya no corre
#include <winapp/textcur.h>
#include <winapp/desktop.h>
#include <winapp/strhis.h>
#include <winapp/cmdbind.h>

class WinApp;
class WinCmd;

struct CmdItem {
	const char *name;
	WinCmd *cmd;
};

class WinCmd  {

    useIdCheck(WinCmd_id);
    
protected:
    
	char chr;							// char para el insChar
    WinApp *appi;				
    static CmdItem *commands;    		// Table of available commands
	static Int addCmd(const char *, WinCmd *c);	// Adds a command to the command table

public:

	WinCmd();
	virtual ~WinCmd(); 

	// Executes the command
		virtual void execute(char i, WinApp *app)=0;

	// Virtual methods
		virtual bool _redo()=0;				// (Re) does the command
		virtual void clear() { idCheck(); } // Initializes the command
		virtual Int id()=0;					// Returns command id
		virtual void print()=0;				// Method to debug class

    // Gets a command from the command table
    	static WinCmd *getCmd(const Int cmd);
		static String getName(const Int cmd);
};

class ViewCmd : public WinCmd {

protected:
	Int urow, ucol, indent;					// row and column for undo/redo
	
public:
	ViewCmd() : WinCmd() { idCheck(); }
	virtual ~ViewCmd();

	virtual bool _redo()=0;			// rehace el comando
	virtual void _undo()=0;			// deshace al comando
	virtual WinCmd *clone()=0;		// genera nuevo comando
	virtual void print()=0;			// Impresion para debugging
	virtual Int id()=0;				// id del comando
	virtual void execute(char c, WinApp *app);
};

class PosCmd : public ViewCmd {

public:
	PosCmd() : ViewCmd() { idCheck();}
	virtual ~PosCmd() { idCheck();}

	virtual bool _redo();				// rehace el comando
	virtual void _undo();				// deshace al comando
	virtual WinCmd *clone()=0;			// genera nuevo comando
	virtual bool redo()=0;				// hace el verdadero trabajo
	virtual void undo()  { idCheck(); } // hace el verdadero trabajo
	virtual void clear() { idCheck(); } // inicializa la estructura
	virtual void print() { idCheck(); } // Impresion para debugging
	virtual Int id()=0;
};

class EdPosCmd : public PosCmd {

public:
	EdPosCmd() : PosCmd() { idCheck();}
	virtual ~EdPosCmd() { idCheck(); }

	virtual bool _redo();				// rehace el comando
	virtual void _undo();				// deshace al comando
	virtual WinCmd *clone()=0;			// genera nuevo comando
	virtual bool redo()=0;				// hace el verdadero trabajo
	virtual void undo()  { idCheck(); } // hace el verdadero trabajo
	virtual void clear() { idCheck(); } // inicializa la estructura
	virtual void print() { idCheck(); } // Impresion para debugging
	virtual Int id()=0;
};

class BlockCmd : public ViewCmd {

public:

	BlockCmd() : ViewCmd() { idCheck();}
	virtual ~BlockCmd() { idCheck(); }

	virtual bool _redo();						// rehace el comando
	virtual void _undo();						// deshace al comando
	virtual WinCmd *clone()=0;					// genera nuevo comando
	virtual bool redo(TextCursor &tc)=0;		// rehace el comando
	virtual void undo()=0;						// deshace el comando
	virtual void clear() { idCheck();  }		// inicializa la estructura
	virtual void print() { idCheck();  }		// Impresion para debugging
	virtual Int id()=0;
};

class EditCmd : public ViewCmd {

public:

	EditCmd() : ViewCmd() { idCheck();}
	virtual ~EditCmd() { idCheck(); }

	virtual bool _redo();				// rehace el comando
	virtual void _undo();				// deshace al comando
	virtual WinCmd *clone()=0;			// genera nuevo comando
	virtual bool redo()=0;				// rehace el comando
	virtual void undo()=0;				// deshace el comando
	virtual void clear() { idCheck(); }	// inicializa la estructura
	virtual void print() { idCheck(); }	// Impresion para debugging
	virtual Int id()=0;
};

class DesktopCmd : public WinCmd {
public:
	DesktopCmd() : WinCmd() { idCheck(); }
	virtual ~DesktopCmd() {	idCheck(); }
	
	virtual bool _redo()=0;				// rehace el comando
	virtual void _undo()=0;				// deshace al comando
	virtual WinCmd *clone()=0;			// genera nuevo comando
	virtual void print()=0;				// Impresion para debugging
	virtual Int id()=0;					// id del comando
	virtual bool allowEmptyDesktop() = 0; 
	virtual void execute(char c, WinApp *app);
};

// Commands that need the desktop not to be empty.
class NoEmptyDesktopCmd: public DesktopCmd {

public:

	NoEmptyDesktopCmd();
	virtual ~NoEmptyDesktopCmd();

	virtual bool _redo();				// rehace el comando
	virtual void _undo();				// deshace al comando
	virtual WinCmd *clone() = 0;		// genera nuevo comando
	virtual bool redo()=0;				// rehace el comando
	virtual void undo()=0;				// deshace el comando
	virtual void clear() { idCheck(); } // inicializa la estructura
	virtual void print() { idCheck(); } // Impresion para debugging
	virtual Int id()=0;
	bool allowEmptyDesktop() { idCheck(); return false; }; 
};

// Commands that not need the desktop or accept that it may be empty.
class EmptyDesktopCmd : public DesktopCmd {

public:

	EmptyDesktopCmd() : DesktopCmd() { idCheck();}
	virtual ~EmptyDesktopCmd() { idCheck(); }

	virtual bool _redo();				// rehace el comando
	virtual void _undo();				// deshace al comando
	virtual WinCmd *clone()=0;		    // genera nuevo comando
	virtual bool redo()=0;			    // rehace el comando
	virtual void undo()=0;			    // deshace el comando
	virtual void clear() { idCheck(); }	// inicializa la estructura
	virtual void print() { idCheck(); }	// Impresion para debugging
	virtual Int id()=0;           
	bool allowEmptyDesktop() { idCheck(); return true; }; 
};

// macro para la definicion de PosCommands
#define BeginPosCmd(name)											\
																	\
class name : public PosCmd {										\
																	\
public:																\
																	\
	name() : PosCmd() { idCheck(); }								\
	name(const name &n)												\
	: PosCmd() {idCheck(); *this = n; }								\
	bool redo();													\
	WinCmd *clone()	{ idCheck();  return new name(*this); }			\
	name(const char *n)												\
	{                                                               \
		idCheck(); 													\
		cmdId = addCmd(n, this);									\
		eventId = paste(name,_EVID)(cmdId);							\
	}																\
	Int id()			{ idCheck();  return cmdId; }				\
	static Int cmdId;												\
	static Int eventId;												\
private:

// macro para la definicion de PosCmd
#ifdef INITCOMM
#define EndPosCmd(name)												\
} name2(__,name)(quote(name));										\
Int name::cmdId = 0;												\
Int name::eventId = 0;
#else
#define EndPosCmd(name)												\
}
#endif


// macro para la definicion de BlockCmd
#define BeginBlockCmd(name)		 									\
																	\
class name : public BlockCmd {										\
																	\
public:																\
	name() : BlockCmd() { idCheck(); }		 	  					\
	name(const name &n) : BlockCmd() { idCheck(); *this = n; }		\
	void undo();													\
	bool redo(TextCursor &tc);										\
	WinCmd *clone()		{ idCheck();  return new name(*this); }		\
	name(const char *n)												\
	{																\
		idCheck();													\
		cmdId = addCmd(n, this);									\
		eventId = paste(name,_EVID)(cmdId);							\
	}																\
	Int id()				{ idCheck();  return cmdId; }			\
	static Int cmdId;												\
	static Int eventId;												\
private:

// macro para la definicion de funciones del editor
#ifdef INITCOMM
#define EndBlockCmd(name)										    \
} name2(__,name)(quote(name));										\
Int name::cmdId = 0;												\
Int name::eventId = 0;
#else
#define EndBlockCmd(name)										    \
}
#endif

// macro para la definicion de EditCmd
#define BeginEditCmd(name)											\
																	\
class name : public EditCmd {										\
																	\
public:																\
																	\
	name() : EditCmd() { idCheck(); }								\
	name(const name &n) : EditCmd() { idCheck(); *this = n; }		\
	void undo();													\
	bool redo();													\
	WinCmd *clone()		{ idCheck();  return new name(*this); }		\
	name(const char *n)	 											\
	{																\
		idCheck();													\
		cmdId = addCmd(n, this);									\
		eventId = paste(name,_EVID)(cmdId);							\
	}																\
	Int id()			{ idCheck();  return cmdId; }				\
	static Int cmdId;												\
	static Int eventId;												\
private:

// macro para la definicion de funciones del editor
#ifdef INITCOMM
#define EndEditCmd(name)											\
} name2(__,name)(quote(name));										\
Int name::cmdId = 0;												\
Int name::eventId = 0;
#else
#define EndEditCmd(name)											\
}
#endif


// macro para la definicion de ControlCmds
#define BeginNoEmptyDesktopCmd(name)								\
																	\
class name : public NoEmptyDesktopCmd {								\
																	\
public:																\
																	\
	name() : NoEmptyDesktopCmd() { idCheck(); }						\
	name(const name &n) : NoEmptyDesktopCmd() { idCheck(); *this = n; }\
	void undo();													\
	bool redo();													\
	WinCmd *clone()		{ idCheck();  return new name(*this); }		\
	name(const char *n)												\
	{																\
		idCheck();													\
		cmdId = addCmd(n, this);									\
		eventId = paste(name,_EVID)(cmdId);							\
	}																\
	Int id()				{ idCheck();  return cmdId; }		  	\
	static Int cmdId;												\
	static Int eventId;												\
private:

// macro para la definicion de ControlCmd
#ifdef INITCOMM
#define EndNoEmptyDesktopCmd(name)									\
} name2(__,name)(quote(name));										\
Int name::cmdId = 0;												\
Int name::eventId = 0;
#else
#define EndNoEmptyDesktopCmd(name)									\
}
#endif

// macro para la definicion de EmptyDesktopCmd
#define BeginEmptyDesktopCmd(name)									\
																	\
class name : public EmptyDesktopCmd {								\
																	\
public:																\
																	\
	name() : EmptyDesktopCmd() { idCheck();}						\
	name(const name &n) : EmptyDesktopCmd() {idCheck(); *this = n;} \
	void undo();													\
	bool redo();													\
	WinCmd *clone()		    { idCheck(); return new name(*this); }	\
	name(const char *n)												\
	{																\
		idCheck();													\
		cmdId = addCmd(n, this);									\
		eventId = paste(name,_EVID)(cmdId);							\
	}																\
	static Int cmdId;												\
	static Int eventId;												\
	Int id()				{ idCheck();  return cmdId; }		  	\
private:

// macro para la definicion de ControlCmd
#ifdef INITCOMM
#define EndEmptyDesktopCmd(name)									\
} name2(__,name)(quote(name));										\
Int name::cmdId = 0;												\
Int name::eventId = 0;
#else
#define EndEmptyDesktopCmd(name)									\
}
#endif

#define UseHistory(h)												\
	private: static StrHistory *h;									\
	public: static void setHistory(StrHistory *a) { h = a; }	\
			static StrHistory *history() { return h; }
#endif

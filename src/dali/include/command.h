/*********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: command.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION: Class definitions for command type.
*********************************************************************/

#ifndef COMMAND_H
#define COMMAND_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

class DaliText;
class DaliTextCursor;
class Command;

class History 
{
	useIdCheck(History_id);

private:

	Int maxsize;			// longitud maxima del history
	Int floor;				// subindice del ultimo elemento
	Int ind;				// subindice del comando corriente
	Int offset;				// usado para el next y prev
	Command **list;			// puntero a la lista de commandos

           History (const History&) ;
    void operator= (const History&) ;

public:

	History(Int len);	
	~History();			

	void insertCommand(Command *s);
	Command *retCommand();

	void next();
	void prev();

	bool reAlloc(Int len);	// realoca el history

	bool mayRedo() const;
	bool mayUndo() const;

	void clear();

	//	retorna el nivel actual dentro del history
	Int currLevel() const;

	//	retorna la cantidad de niveles actualmente en el history
	Int levels() const;

	Int size()				{ idCheck(); return maxsize; }
};


class Command 
{
    useIdCheck(Command_id);

protected:
    
	char chr;				// char para el insChar
	Int  rep;				// repeticion del comando

	// inserta un comando dentro de cmds
	Int makeCommand(const char *);

	// retorna el cursor del string corriente
	// es utilizado para obtener informacion para el undo
	DaliTextCursor &cursor();

    //Command(const Command&) ;
    //void operator=(const Command&) ;
    
public:

	Command() ;
	virtual ~Command() ; 

	// ejecuta el comando
	virtual void execute(char i, Int n);

	// funciones virtuales
	virtual bool _redo()=0;			// rehace el comando
	virtual void _undo()=0;			// deshace al comando
	virtual void clear() { idCheck(); } // inicializa la estructura
	virtual Command *clone()=0;		// genera nuevo comando
	virtual void print()=0;			// Impresion para debugging
	virtual Int id()=0;				// id del comando
	virtual bool allowNoViews();	// puede ejecutarse sin views ?

};


class PosCommand : public Command {

protected:

	Int cfil, ccol;					// fil, col actual

private:

     //PosCommand(const PosCommand&) ;
     //void operator=(const PosCommand&) ;
     
public:

	PosCommand() : Command() { }
	virtual ~PosCommand() { idCheck(); }

	virtual bool _redo();			// rehace el comando
	virtual void _undo();			// deshace al comando
	virtual Command *clone()=0;		// genera nuevo comando
	virtual bool redo()=0;			// hace el verdadero trabajo
	virtual void undo()  { idCheck(); } // hace el verdadero trabajo
	virtual void clear() { idCheck(); } // inicializa la estructura
	virtual void print() { idCheck(); } // Impresion para debugging
	virtual Int id()=0;
};


class BlockCommand : public Command {

protected:

	Int fil, col;					// coordenadas del bloque
	
private:                       
     
     //BlockCommand(const BlockCommand&) ;
     //void operator=(const BlockCommand&) ;

public:

	BlockCommand() : Command() { }
	virtual ~BlockCommand() { idCheck(); }

	virtual bool _redo();			// rehace el comando
	virtual void _undo();			// deshace al comando
	virtual Command *clone()=0;		// genera nuevo comando
	virtual bool redo(DaliTextCursor &tc)=0;	// rehace el comando
	virtual void undo()=0;			// deshace el comando
	virtual void clear() { idCheck();  }		// inicializa la estructura
	virtual void print() { idCheck();  }		// Impresion para debugging
	virtual Int id()=0;
};


class AnyCommand : public Command {

protected:

	Int fil, col;					// coordenadas del bloque

private:
	 
     //AnyCommand(const AnyCommand&) ;
     //void operator=(const AnyCommand&) ;

public:

	AnyCommand() : Command() { }
	virtual ~AnyCommand() { idCheck(); }

	virtual bool _redo();			// rehace el comando
	virtual void _undo();			// deshace al comando
	virtual Command *clone()=0;		// genera nuevo comando
	virtual bool redo()=0;			// rehace el comando
	virtual void undo()=0;			// deshace el comando
	virtual void clear() { idCheck(); }	// inicializa la estructura
	virtual void print() { idCheck(); }	// Impresion para debugging
	virtual Int id()=0;
};


class ControlCommand : public Command {

private:
     //ControlCommand(const ControlCommand&) ;
     //void operator=(const ControlCommand&) ;

public:

	ControlCommand() : Command() {  }
	virtual ~ControlCommand() { idCheck(); }

	virtual bool _redo();			// rehace el comando
	virtual void _undo();			// deshace al comando
	virtual Command *clone()=0;		// genera nuevo comando
	virtual bool redo()=0;			// rehace el comando
	virtual void undo()=0;			// deshace el comando
	virtual void clear() { idCheck(); } // inicializa la estructura
	virtual void print() { idCheck(); } // Impresion para debugging
	virtual Int id()=0;
};

class ControlCommandNoViews : public ControlCommand {

private:

     //ControlCommandNoViews(const ControlCommandNoViews&) ;
     //void operator=(const ControlCommandNoViews&) ;

public:

	ControlCommandNoViews() : ControlCommand() { }
	virtual ~ControlCommandNoViews() { idCheck(); }

	virtual Command *clone()=0;		// genera nuevo comando
	virtual bool redo()=0;			// rehace el comando
	virtual void undo()=0;			// deshace el comando
	virtual void clear() { idCheck(); }	// inicializa la estructura
	virtual void print() { idCheck(); }	// Impresion para debugging
	virtual Int id()=0;           
	virtual bool allowNoViews();	// puede ejecutarse sin views ?
	
};


// macro para la definicion de PosCommands
#define BeginPosCommand(name)										\
																	\
class name : public PosCommand {									\
																	\
public:																\
																	\
	name() : PosCommand() {  }										\
	name(const name &n)                                             \
	: PosCommand() {idStart(); *this = n; }							\
	virtual ~name() { idCheck();}									\
	bool redo();													\
	Command *clone()	{ idCheck();  return tr_new name(*this); }		\
	name(const char *n)	{ idStart();  cmdId = makeCommand(n); }		\
	Int id()			{ idCheck();  return cmdId; }		  		\
	static Int cmdId;												\
																	\
private:                                                            \

//void operator=(const name&);

// macro para la definicion de PosCommand
#ifdef INITCOMM
#define EndPosCommand(name)											\
} name2(__,name)(quote(name));										\
Int name::cmdId = 0;
#else
#define EndPosCommand(name)											\
}
#endif


// macro para la definicion de BlockCommand
#define BeginBlockCommand(name)		 								\
																	\
class name : public BlockCommand {									\
																	\
public:																\
	name() : BlockCommand() {  }		 	  						\
	name(const name &n) 											\
	: BlockCommand() { idStart(); *this = n; }						\
	virtual ~name() { idCheck();}			 						\
	void undo();													\
	bool redo(DaliTextCursor &tc);									\
	Command *clone()		{ idCheck();  return tr_new name(*this); }	\
	name(const char *n)		{ idStart();  cmdId = makeCommand(n); }	\
	Int id()				{ idCheck();  return cmdId; }			\
	static Int cmdId;		                                        \
	   																\
private:                                                            \
     //void operator=(const name&) ;

// macro para la definicion de funciones del editor
#ifdef INITCOMM
#define EndBlockCommand(name)										\
} name2(__,name)(quote(name));										\
Int name::cmdId = 0;
#else
#define EndBlockCommand(name)										\
}
#endif

// macro para la definicion de AnyCommand
#define BeginAnyCommand(name)										\
																	\
class name : public AnyCommand { 									\
																	\
public:																\
																	\
	name() : AnyCommand() {  }										\
	name(const name &n) : AnyCommand() { idStart(); *this = n; }	\
	virtual ~name() { idCheck(); } 									\
	void undo();													\
	bool redo();													\
	Command *clone()		{ idCheck();  return tr_new name(*this); }	\
	name(const char *n)		{ idCheck();  cmdId = makeCommand(n); }	\
	Int id()				{ idCheck();  return cmdId; }			\
	static Int cmdId;												\
																	\
private:                                                            \
     //void operator=(const name&) ;

// macro para la definicion de funciones del editor
#ifdef INITCOMM
#define EndAnyCommand(name)											\
} name2(__,name)(quote(name));										\
Int name::cmdId = 0;
#else
#define EndAnyCommand(name)											\
}
#endif

// macro para la definicion de ControlCommands
#define BeginControlCommand(name)									\
																	\
class name : public ControlCommand {								\
																	\
public:																\
																	\
	name() : ControlCommand() {  }									\
	name(const name &n) : ControlCommand() { idStart(); *this = n; }\
	virtual ~name() { idCheck(); }									\
	void undo();													\
	bool redo();													\
	Command *clone()		{ idCheck();  return tr_new name(*this); }	\
	name(const char *n)		{ idStart();  cmdId = makeCommand(n); }	\
	Int id()				{ idCheck();  return cmdId; }		  	\
	static Int cmdId;												\
																	\
private:                                                            \
     //void operator=(const name&) ;

// macro para la definicion de ControlCommand
#ifdef INITCOMM
#define EndControlCommand(name)										\
} name2(__,name)(quote(name));										\
Int name::cmdId = 0;
#else
#define EndControlCommand(name)										\
}
#endif

// macro para la definicion de ControlCommandsNoViews
#define BeginControlCommandNoViews(name)							\
																	\
class name : public ControlCommandNoViews {							\
																	\
public:																\
																	\
	name() : ControlCommandNoViews() { }							\
	name(const name &n) 											\
		: ControlCommandNoViews() { idStart();  *this = n; }		\
	virtual ~name() { idCheck();  }			 						\
	void undo();													\
	bool redo();													\
	Command *clone()		{ idCheck(); return tr_new name(*this); }	\
	name(const char *n)		{ idCheck(); cmdId = makeCommand(n); }	\
	Int id()				{ idCheck(); return cmdId; }			\
	static Int cmdId;												\
																	\
private:                                                            \
     //void operator=(const name&);

// macro para la definicion de ControlCommand
#ifdef INITCOMM
#define EndControlCommandNoViews(name)								\
} name2(__,name)(quote(name));										\
Int name::cmdId = 0;
#else
#define EndControlCommandNoViews(name)								\
}
#endif

#endif

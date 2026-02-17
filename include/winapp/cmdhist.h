/*********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: cmdhist.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION: Class definitions for class CmdHistory.
*
*********************************************************************/

#ifndef CMDHIST_H
#define CMDHIST_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

class CmdHistory 
{
	useIdCheck(CmdHistory_id);

private:

	Int maxsize;			// longitud maxima del history
	Int floor;				// subindice del ultimo elemento
	Int ind;				// subindice del comando corriente
	Int offset;				// usado para el next y prev
	Command **list;			// puntero a la lista de commandos

    CmdHistory (const CmdHistory&) ;
    void operator= (const CmdHistory&) ;

public:

	CmdHistory(Int len);	
	~CmdHistory();			

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

#endif

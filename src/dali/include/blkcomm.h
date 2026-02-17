/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: blkcomm.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION: Definitions for block commands.
*********************************************************************/

#ifndef BLKCOMM_H
#define BLKCOMM_H

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <ifound/str.h>
#include <winapp/log.h>

BeginBlockCommand(CopyBlock)
	Log theLog;
EndBlockCommand(CopyBlock);

BeginBlockCommand(CutBlock)
	Log theLog;
	Int cfil;				// fila para guardar la coordenada
	Int ccol;				// col para guardar la coordenada
EndBlockCommand(CutBlock);

BeginBlockCommand(DeleteBlock)	
	Log theLog;
	Int cfil;				// fila del cursor
	Int ccol;				// columna del cursor
EndBlockCommand(DeleteBlock);

BeginBlockCommand(ClearBlock)	
	Log theLog;
	Int tcfil;				// fila para guardar la coordenada
	Int tccol;				// col para guardar la coordenada
	Int cfil;				// fila del cursor
	Int ccol;				// columna del cursor
EndBlockCommand(ClearBlock);

BeginBlockCommand(SetAttribute)
	Int tcfil;				// fila para guardar la coordenada
	Int tccol;				// col para guardar la coordenada
	Int atrib;				// atributo seleccionado
public:
	virtual void clear() { atrib = ERROR; }
EndBlockCommand(SetAttribute);

BeginBlockCommand(DelAttribute)
	Int tcfil;				// fila para guardar la coordenada
	Int tccol;				// col para guardar la coordenada
	Int atrib;				// atributo seleccionado
public:
	virtual void clear() { atrib = ERROR; }
EndBlockCommand(DelAttribute);

BeginBlockCommand(ExecOSFilter)
	Log theLog;
	Int hfil;			// fila hasta donde tengo que borrar
	Int hcol;			// columna hasta donde tengo que borrar
	Int cfil;				// fila del cursor
	Int ccol;				// columna del cursor
	String cmd;			// comando a ejecutar
public:
	void clear() { cmd = NULL_STRING; }
EndBlockCommand(ExecOSFilter);

BeginBlockCommand(EndBlockMode)
EndBlockCommand(EndBlockMode);

#endif

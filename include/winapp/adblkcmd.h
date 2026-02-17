/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: adblkcmd.h,v 1.1.1.1 1996/04/16 18:51:42 gustavof Exp $
*
* DESCRIPTION: Definitions for Advanced block commands.
*********************************************************************/
#ifdef __GNUC__
#pragma interface
#endif

#ifndef ADBLKCMD_H
#define ADBLKCMD_H

#include <ifound/str.h>
#include <winapp/log.h>

#define	BlockOSFilter_EVID(dummy)		COMPOSE(K_META, '!')
#define	BlockSetAttr_EVID(dummy)		COMPOSE(K_META, '+')
#define	BlockDelAttr_EVID(dummy)		COMPOSE(K_META, '-')
#define	BlockShiftLeft_EVID(dummy)		COMPOSE(K_CTRLX, '[')
#define	BlockShiftRight_EVID(dummy)		COMPOSE(K_CTRLX, ']')

BeginBlockCmd(BlockSetAttr)
	Int tcfil;				// fila para guardar la coordenada
	Int tccol;				// col para guardar la coordenada
	Int atrib;				// atributo seleccionado
public:
	virtual void clear() { atrib = ERR; }
EndBlockCmd(BlockSetAttr);

BeginBlockCmd(BlockDelAttr)
	Int tcfil;				// fila para guardar la coordenada
	Int tccol;				// col para guardar la coordenada
	Int atrib;				// atributo seleccionado
public:
	virtual void clear() { atrib = ERR; }
EndBlockCmd(BlockDelAttr);

BeginBlockCmd(BlockOSFilter)
	Log theLog;
	Int hfil;			// fila hasta donde tengo que borrar
	Int hcol;			// columna hasta donde tengo que borrar
	Int cfil;				// fila del cursor
	Int ccol;				// columna del cursor
	String cmd;			// comando a ejecutar
public:
	void clear() { cmd = NULL_STRING; }
EndBlockCmd(BlockOSFilter);

BeginEditCmd(BlockShiftRight)
	Int		rowBlk;
	Int		colBlk;
	Int		rowCur;
	Int		colCur;
EndEditCmd(BlockShiftRight);

BeginEditCmd(BlockShiftLeft)
	Int		rowBlk;
	Int		colBlk;
	Int		rowCur;
	Int		colCur;
	bool	needRestore;
	bool	didSomething;
	Log		theLog;
EndEditCmd(BlockShiftLeft);

#endif

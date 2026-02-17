/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: blkcmd.h,v 1.1.1.1 1996/04/16 18:51:42 gustavof Exp $
*
* DESCRIPTION: Definitions for block commands.
*********************************************************************/

#ifndef BLKCMD_H
#define BLKCMD_H

#ifdef __GNUC__
#pragma interface
#endif

// IFound library
#include	<ifound/str.h>

// WinApp library
#include	<winapp/cmdbind.h>
#include	<winapp/log.h>
#include	<winapp/wincmd.h>

//#define BlockBeginMode_EVID(dummy)	COMPOSE(K_META,	K_META)
#define	BlockEndMode_EVID(dummy)	K_END
#define	BlockBegin_EVID(dummy)		COMPOSE(K_META, K_META)
#define	BlockClear_EVID(dummy)		COMPOSE(K_META,	' ')
#define	BlockDelete_EVID(dummy)		COMPOSE(K_META,	'D')
#define	BlockCut_EVID(dummy)		COMPOSE(K_META,	K_DEL)
#define	BlockPaste_EVID(dummy)		COMPOSE(K_META,	K_ENTER)
#define	BlockCopy_EVID(dummy)		COMPOSE(K_META,	K_INS)

void bindBlockCmds(CmdBinding *cb);

BeginNoEmptyDesktopCmd(BlockBegin)
EndNoEmptyDesktopCmd(BlockBegin);

BeginBlockCmd(BlockEndMode)
EndBlockCmd(BlockEndMode);

BeginBlockCmd(BlockCopy)
	Log theLog;
EndBlockCmd(BlockCopy);

BeginBlockCmd(BlockCut)
	Log theLog;
	Int cfil;				// fila para guardar la coordenada
	Int ccol;				// col para guardar la coordenada
EndBlockCmd(BlockCut);

BeginEditCmd(BlockPaste)	
	Int hfil;			// fila hasta donde tengo que borrar
	Int hcol;			// columna hasta donde tengo que borrar
EndEditCmd(BlockPaste);

BeginBlockCmd(BlockDelete)	
	Log theLog;
	Int cfil;				// fila del cursor
	Int ccol;				// columna del cursor
EndBlockCmd(BlockDelete);

BeginBlockCmd(BlockClear)	
	Log theLog;
	Int tcfil;				// fila para guardar la coordenada
	Int tccol;				// col para guardar la coordenada
	Int cfil;				// fila del cursor
	Int ccol;				// columna del cursor
EndBlockCmd(BlockClear);

#endif

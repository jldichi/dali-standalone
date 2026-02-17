/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: editcmd.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION: Definition of edition commands
*********************************************************************/

#ifndef EDITCMD_H
#define EDITCMD_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/str.h>
#include <winapp/log.h>

#define	EditEnter_EVID(dummy)			K_ENTER
#define	EditAddNewLine_EVID(dummy)		COMPOSE(K_META, 'o')
#define	EditInsNewLine_EVID(dummy)		COMPOSE(K_META, 'O')
#define	EditJoinLines_EVID(dummy)		COMPOSE(K_META, 'j')
#define	EditInsChar_EVID(dummy)			K_INS
#define	EditDelChar_EVID(dummy)			K_DEL
#define	EditBackSpace_EVID(dummy)		K_BACKSPACE
#define EditDelLine_EVID(dummy)			COMPOSE(K_META, 'd')
#define	EditDelWord_EVID(dummy)    		COMPOSE(K_META, 'w')
#define EditDelEndOfLine_EVID(dummy)	COMPOSE(K_META, 'e')
#define	EditTab_EVID(dummy)				K_TAB
#define	EditBackTab_EVID(dummy)			COMPOSE(K_META, K_BACKSPACE)
#define	EditInsAsciiChar_EVID(dummy)	COMPOSE(K_CTRLX, '#')
#define	ChgInsMode_EVID(dummy)			K_INS
#define	ChgCtrolMode_EVID(dummy)		COMPOSE(K_META,  'l')

void bindEditCmds(CmdBinding *cb);

BeginEditCmd(EditInsChar)
	bool insertMode; // modo en el que estaba cuando se ejecuto
	Int line;		// linea del EOF (o ERR si no hace falta)
	Int col;		// si line==ERR --> longitud de la linea (o ERR
					// si no hace falta) sino columna del EOF
	UnChar character; // caracter que reemplaza (modo overwrite)
	String replaced; // caracteres hasta previo tab.
EndEditCmd(EditInsChar);

BeginEditCmd(EditInsAsciiChar)
	bool insertMode; // modo en el que estaba cuando se ejecuto
	Int line;		// linea del EOF (o ERR si no hace falta)
	Int col;		// si line==ERR --> longitud de la linea (o ERR
					// si no hace falta) sino columna del EOF
	UnChar character; // caracter que reemplaza (modo overwrite)
	String replaced; // caracteres hasta previo tab.
	bool isCleared;	// Flag para ver si debo mostrar el popup o no
public:
	void clear();
EndEditCmd(EditInsAsciiChar);

BeginEditCmd(EditDelChar)
	Int line;		// nro. de linea en la cual se borra el char
	Int offset;		// offset donde fue borrado el caracter
	char theChar;	// caracter que fue borrado
	String replaced; // caracteres hasta previo tab.
EndEditCmd(EditDelChar);

BeginEditCmd(EditDelLine)
	String	text;	// texto borrado
	Int		offset; // offset donde estaba posicionado el cursor
EndEditCmd(EditDelLine);

BeginEditCmd(EditDelWord)
	String	text1;	// texto previo (linea 1)
	String	text2;	// texto previo (linea 2)
	bool	twoLines;	// si se juntaron dos lineas o no
	Int		offset; // offset donde estaba posicionado el cursor
EndEditCmd(EditDelWord);

BeginEditCmd(EditDelEndOfLine)
	String	text;	// texto previo
EndEditCmd(EditDelEndOfLine);
#if 0
BeginEditCmd(EditDelEndOfLineWithIns)
	String	text;	// texto previo
EndEditCmd(EditDelEndOfLineWithIns);
#endif
BeginEditCmd(EditBackSpace)
	char	theChar;	// caracter que fue borrado
	String	replaced;	// caracteres hasta previo tab.
	bool    isJoin;		// joined lines 
EndEditCmd(EditBackSpace);

BeginEditCmd(EditEnter)
	Int indent;
EndEditCmd(EditEnter);

BeginEditCmd(EditAddNewLine)
EndEditCmd(EditAddNewLine);

BeginEditCmd(EditInsNewLine)
EndEditCmd(EditInsNewLine);

BeginEditCmd(EditJoinLines)
EndEditCmd(EditJoinLines);

BeginEditCmd(EditTab)
EndEditCmd(EditTab);

BeginEditCmd(EditBackTab)
EndEditCmd(EditBackTab);

BeginEditCmd(ChgInsMode)
EndEditCmd(ChgInsMode);

BeginEditCmd(ChgCtrolMode)
EndEditCmd(ChgCtrolMode);

#endif

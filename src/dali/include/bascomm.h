/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: bascomm.h,v 1.1.1.1 1996/04/16 18:52:52 gustavof Exp $
*
* DESCRIPTION:	Definitions for commdef.
*********************************************************************/

#ifndef BASCOMM_H
#define BASCOMM_H

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound/str.h>

#include <winapp/log.h>

// comandos que modifican el archivo

BeginAnyCommand(InsChar)
	bool insertMode; // modo en el que estaba cuando se ejecuto
	Int line;		// linea del EOF (o ERROR si no hace falta)
	Int col;		// si line==ERROR --> longitud de la linea (o ERROR
					// si no hace falta) sino columna del EOF
	UnChar character; // caracter que reemplaza (modo overwrite)
	String replaced; // caracteres hasta previo tab.
EndAnyCommand(InsChar);

BeginAnyCommand(InsAsciiChar)
	bool insertMode; // modo en el que estaba cuando se ejecuto
	Int line;		// linea del EOF (o ERROR si no hace falta)
	Int col;		// si line==ERROR --> longitud de la linea (o ERROR
					// si no hace falta) sino columna del EOF
	UnChar character; // caracter que reemplaza (modo overwrite)
	String replaced; // caracteres hasta previo tab.
	bool isCleared;	// Flag para ver si debo mostrar el popup o no
public:
	void clear();
EndAnyCommand(InsAsciiChar);

BeginAnyCommand(DelChar)
	Int line;		// nro. de linea en la cual se borra el char
	Int offset;		// offset donde fue borrado el caracter
	char theChar;	// caracter que fue borrado
	String replaced; // caracteres hasta previo tab.
EndAnyCommand(DelChar);

BeginAnyCommand(DelLine)
	String	text;	// texto borrado
	Int		offset; // offset donde estaba posicionado el cursor
EndAnyCommand(DelLine);

BeginAnyCommand(DelWord)
	String	text1;	// texto previo (linea 1)
	String	text2;	// texto previo (linea 2)
	bool	twoLines;	// si se juntaron dos lineas o no
	Int		offset; // offset donde estaba posicionado el cursor
EndAnyCommand(DelWord);

BeginAnyCommand(DelEndOfLine)
	String	text;	// texto previo
EndAnyCommand(DelEndOfLine);

BeginAnyCommand(DelEndOfLineWithIns)
	String	text;	// texto previo
EndAnyCommand(DelEndOfLineWithIns);

BeginAnyCommand(BackSpace)
	char	theChar;	// caracter que fue borrado
	String	replaced;	// caracteres hasta previo tab.
	bool    isJoin;		// joined lines 
EndAnyCommand(BackSpace);

BeginAnyCommand(Enter)
	Int indent;
EndAnyCommand(Enter);

BeginAnyCommand(BraceBlock)
	String	text;	  	// texto borrado
	Int		offset; 	// offset donde estaba posicionado el cursor
EndAnyCommand(BraceBlock);

#endif

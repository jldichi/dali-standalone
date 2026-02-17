/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: eddebug.h,v 1.1.1.1 1996/04/16 18:52:54 gustavof Exp $
*
* DESCRIPTION:
*
*********************************************************************/
#error 0
#ifndef EDDEBUG_H
#define EDDEBUG_H

#ifdef ISDEBUG

// CLASSES
// #define	ClassName		_D_Class_Name
//
#define	DaliView			_D_DaliView
#define	DaliText     		_D_DaliText    
#define	DaliTextCursor		_D_DaliTextCursor
#define	DaliAttributeZones	_D_DaliAttributeZones    
#define	Command 			_D_Command   
#define History             _D_History
#define TagIndex            _D_TagIndex
#define DaliRc              _D_DaliRc 


// ID's
// #define ClassName	ClassId	
//
#define	DALIVIEW_ID				0xf1d23414
#define	DALITEXT_ID     		0xb213b82f
#define	DALITEXTCURSOR_ID		0x82b7312a
#define	DALIATTRIBUTEZONES_ID	0xc18251c3
#define	COMMAND_ID 				0x8712175d
#define HISTORY_ID				0x1283121c
#define TAGINDEX_ID				0xaf234244
#define DALIRC_ID				0xb23423de
#endif	// ISDEBUG

#endif	// EDDEBUG_H

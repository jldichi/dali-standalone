/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: poscomm.cc,v 1.1.1.1 1996/04/16 18:52:50 gustavof Exp $
*
* DESCRIPTION: Implementation of positioning commands
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM

#include <command.h>
#include <poscomm.h>
#include <editor.h>
#include <dview.h>
#include <dtext.h>
#include <winapp/window.h>
#include <ifound/rexp.h>
#include <edlbox.h>
#include <winapp/dbox.h>
#include <winapp/strhis.h>

declare(PtrList,DaliView);
declare(PtrListCursor,DaliView);

// clase CursDown

bool CursDown::redo()
{
	idCheck();
	cView.value()->cursDown();
	return true;
}

// end clase CursDown

// clase CursUp

bool CursUp::redo()
{
	idCheck();
	cView.value()->cursUp();
	return true;
}

// end clase CursUp

// clase CursorLeft

bool CursLeft::redo()
{
	idCheck();
	cView.value()->cursLeft();
	return true;
}

// end clase CursLeft

// clase CursRight

bool CursRight::redo()
{
	idCheck();
	cView.value()->cursRight();
	return true;
}

// clase CursRight

// clase PageDown

bool PageDown::redo()
{
	idCheck();
	cView.value()->pageDown();
	return true;
}

// end clase PageDown

// clase PageUp

bool PageUp::redo()
{
	idCheck();
	cView.value()->pageUp();
	return true;
}

// fin clase PageUp

// clase WordRight

bool WordRight::redo()
{
	idCheck();
	cView.value()->wordRight();
	return true;
}

// fin clase WordRight

// clase WordLeft

bool WordLeft::redo()
{
	idCheck();
	cView.value()->wordLeft();
	return true;
}

// fin clase WordLeft

// clase BegOfLine

bool BegOfLine::redo()
{
	idCheck();
	cView.value()->cursLeft(cursor().column());
	return true;
}

// fin clase BegOfLine

// clase EndOfLine

bool EndOfLine::redo()
{
	idCheck();
	Int offset = cursor().value().length() - cursor().column();
	cView.value()->cursRight(offset);
	return true;
}

// fin clase EndOfLine

// clase BegOfFile

bool BegOfFile::redo()
{
	idCheck();
	cView.value()->goToBegOfFile();
	return true;
}

// fin clase BegOfFile

// clase EndOfFile

bool EndOfFile::redo()
{
	idCheck();
	cView.value()->goToEndOfFile();
	return true;
}

// fin clase EndOfFile

// clase BegOfScreen

bool BegOfScreen::redo()
{
	idCheck();
	cView.value()->goToBegOfScreen();
	return true;
}

// fin clase BegOfScreen

// clase EndOfScreen

bool EndOfScreen::redo()
{
	idCheck();
	cView.value()->goToEndOfScreen();
	return true;
}

// fin clase EndOfScreen

// clase Match

static String getMatchItem(const String &context,Int col) 
{                  
   
	String tmp = context;                  
  
	Int 	from 		= col,
			to	 		= col;
	bool	multiChar 	= false;
    
    // Test if cursor is in a separator-type char
    String theChar;            
    if(isSeparator(tmp[from])) 
       theChar = tmp[from];     

	if (from >= tmp.length())
	 	from = to = ERR;                          

    // Try to recognize a multi-char expression
	while ( from >= 0 && !isSpace(tmp[from]) ) {
	    multiChar = true;
		from--;                               
	}	
	from++;

	while (to < tmp.length() && !isSpace(tmp[to]) )	{	  
	    multiChar = true;
		to++;                                       
	}	
            
	if (from == ERR || to == ERR)                   
		return NULL_STRING;
    
	if (multiChar)  { 
		tmp = tmp(from,to-from);
		if (Editor::isInMatchList(tmp)) {
		    col = from;
			return tmp;                  
		}	
	} 
	if (Editor::isInMatchList(theChar)) {
	    col = from;
		return theChar;
	}	
    else
    	return NULL_STRING;			
}

bool OuterBackMatch::redo() 	   
{   
	idCheck();
	String      res, auxStr;
    DaliTextCursor  tc	  		= cursor();
    Int         column		= cursor().column();

	// Find in text first MatchPair-token to the left
	bool first = true;    
	Int  stack = 1;                 
                              
    while(tc) {
                                                    
        if (!isSpace(tc.value()[column])) 
        	auxStr = getMatchItem(tc.value(), column);
        else 
            auxStr = NULL_STRING;	      
            
		switch(Editor::retMatch(auxStr, res)) {
		    case BACK_MATCH: 
				if (first)  
					return cView.value()->backMatch(auxStr,res);
				else {
				    stack++;	
				    --column;
				}    
				break;	
            case FORW_MATCH:
				if (!first && --stack == 0) {
					cView.value()->goCursor(tc.order(),column);
					return true;
				}
			case NO_MATCH:
			    --column;
				break;
			default:
			    assert(0);		
        }   
        
        // There is no more to do here
        if (tc.order() == 0 && column <= 0) 
             break;                   
             
        first = false;
        
		while (column > 0 && (!isSpace(tc.value()[column]) ) 
		                  && (!isSeparator(tc.value()[column]) ) )
		       --column;           
               
		while (column > 0 && (isSpace(tc.value()[column])))
		       --column;

		if (tc.order() > 0) {
			if (column < 0) {
				--tc; 
				if (tc)       
					column = tc.value().length()-1;
			}	       
		} 
		else
			if (column < 0) 
				break;
	}            
	cView.value()->beep();
	return false;
}

bool OuterForwMatch::redo()  
{   
	idCheck();
	String 			res, auxStr;
    DaliTextCursor  tc				= cursor();
    Int         	column 			= cursor().column();

	// Find in text first MatchPair-token to the right
	bool first = true;    
	Int  stack = 1;
    while (tc) {   
        auxStr = getMatchItem(tc.value(), column);
		switch(Editor::retMatch(auxStr, res)) {
		    case FORW_MATCH:
				if (first) 
					return cView.value()->forwMatch(auxStr,res);
				else 
				    stack++;	
				break;	
            case BACK_MATCH:
				if (!first && --stack == 0) {
					cView.value()->goCursor(tc.order(), column);
					return true;
				}
			case NO_MATCH:
				break;
			default:
			    assert(0);		
        }
        
        first = false;
        
        // replace with goFirstNonSp ?
        do
           ++column;
		while (column < tc.value().length() && (!isSpace(tc.value()[column]) ) 
		                  && (!isSeparator(tc.value()[column]) ) );
               
		while (column < tc.value().length() && (isSpace(tc.value()[column])))
		        ++column;

		if (column >= tc.value().length()) {
			++tc;        
			column = 0;
		}	
    }
	cView.value()->beep();
	return false;
}

// fin clase Match

// clase MiddleOfScreen

bool MiddleOfScreen::redo()
{
	idCheck();
	cView.value()->goToMiddleOfScreen();
	return true;
}

// fin clase MiddleOfScreen

// clase BackFuncBegin

bool BackFuncBegin::redo()
{
	idCheck();
	cView.value()->goToBackFuncBegin();
	return true;
}

// fin clase BackFuncBegin

// clase ForwFuncBegin

bool ForwFuncBegin::redo()
{
	idCheck();
	cView.value()->goToForwFuncBegin();
	return true;
}

// fin clase ForwFuncBegin

// clase GoFirstNonSp

bool GoFirstNonSp::redo()
{
	idCheck();
	cView.value()->goFirstNonSp();
	return true;
}

// fin clase GoFirstNonSp

// clase MarkPosition

bool MarkPosition::redo()
{              
	idCheck();
	cView.value()->setMark();       
	return true;
}
// fin clase GoMarkPosition

// clase GoMarkPosition

bool GoMarkPosition::redo()
{              
	idCheck();
	cView.value()->goMark();
	return true;
}

// fin clase GoMarkPosition

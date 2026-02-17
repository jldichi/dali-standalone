/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: schcomm.cc,v 1.1.1.1 1996/04/16 18:52:50 gustavof Exp $
*
* DESCRIPTION: Implementation of search command clases
*********************************************************************/

#ifdef __GNUG__
#	pragma implementation
#endif

// para que genere la entrada dentro de la tabla de comandos
#define INITCOMM

#include <ifound.h>
#include <ifound/stdc.h>
#include <winapp/msgbox.h>
#include <edlbox.h>
#include <winapp/cell.h>
#include <ifound/rexp.h>
#include <winapp/strhis.h>

#include <command.h>
#include <schcomm.h>
#include <schmcomm.h>
#include <editor.h>
#include <dview.h>
#include <dtext.h>
#include <log.h>

declare(PtrList, DaliView);
declare(PtrListCursor, DaliView);

// variable que marca los limites de la busqueda
static Int filfrom    = ERR;
static Int filto      = ERR;
static Int flgs       = 0;
static String text    = NULL_STRING;
static String newtext = NULL_STRING;

// clase FindText

void FindText::clear()
{
	idCheck();
	sFilfrom = sFilto = ERR;
	sFlgs = ERR;
	sText = NULL_STRING;
}

void FindText::undo()
{
	idCheck();
	filfrom = sFilfrom;
	filto = sFilto;
	flgs = sFlgs;
	text = sText;
}

bool FindText::redo()
{
	idCheck();
	MsgBox errbox(libMsg("ERROR"),
			Editor::editorMsg("TEXTNOTFOUND"), 5, 32, 6, 20);
	if (sFlgs == ERR) {
		PFindText menu(Editor::editorMsg("FindText"), 10, 36, 6, 21);

		// restauro el valor del ultimo find
		menu.patron->value() = text;

		menu.execute();      
		
		if(menu.isAccepted() != true) 
			return false;

		// Obtengo estos valores antes, para guardarlos en el history
		// por si la rexp es invalida.
		sText = text = menu.getPatron();
		*Editor::finds << text;

		String tmpStr = menu.getPatron();
		RegExp tmpRe(tmpStr);
		if (!tmpRe) {
			MsgBox invRegExp(libMsg("ERROR"),
					Editor::editorMsg("INVREGEXP"), 5, 32, 6, 20);
			(void) invRegExp.execute();
			return false;
		}

		sFilfrom = filfrom = menu.getFrom();
		sFilto = filto = menu.getTo();
		sFlgs = flgs = menu.getFlags();
	}
	else {
		filfrom = sFilfrom;
		filto = sFilto;
		flgs = sFlgs;
		text = sText;
	}

	RegExp re(text);

	Int res;

	if (flgs & BACKWARD) 
		res = cView.value()->findBackText(re, filfrom, filto, false);
	else
		res = cView.value()->findForwText(re, filfrom, filto, false);

	if (res == ERR) {
		errbox.execute();
		return false;
	}

	return true;
}

// end clase FindText

// clase FindAgain

bool FindAgain::redo()
{
	idCheck();
	MsgBox errbox(libMsg("ERROR"),
			Editor::editorMsg("TEXTNOTFOUND"), 5, 32, 6, 20);

	Int res = ERR;

	if (text != NULL_STRING) {

		RegExp re(text);

		if (flgs & FIND) {
			if (flgs & BACKWARD) 
				res = cView.value()->findBackText(re, filfrom,
						filto, true);
			else
				res = cView.value()->findForwText(re, filfrom,
						filto, true);
		}

	}

	if (res == ERR) {
		errbox.execute();
		return false;
	}

	return true;
}

// end clase FindAgain


// clase FindNext

bool FindNext::redo()
{
	idCheck();
	MsgBox errbox(libMsg("ERROR"),
			Editor::editorMsg("TEXTNOTFOUND"), 5, 32, 6, 20);

	Int res = ERR;

	if (text != NULL_STRING && flgs & FIND) {
		RegExp re(text);
		res	 = cView.value()->findForwText(re, filfrom, filto, true);
	 	flgs = FIND|FORWARD;
	}

	if (res == ERR) {
		errbox.execute();
		return false;
	}

	return true;
}

// end clase FindNext

bool FindPrev::redo()
{
	idCheck();
	MsgBox errbox(libMsg("ERROR"),
			Editor::editorMsg("TEXTNOTFOUND"), 5, 32, 6, 20);

	Int res = ERR;

	if (text != NULL_STRING && flgs & FIND) {
		RegExp re(text);
		res  = cView.value()->findBackText(re, filfrom, filto, true);
		flgs = FIND|BACKWARD;
	}

	if (res == ERR) {
		errbox.execute();
		return false;
	}

	return true;
}

// end clase FindPrev

// clase ReplaceText

void ReplaceText::clear()
{
	idCheck();
	which.realloc(0);
	row = col = ERR;
}

void ReplaceText::undo()
{
	idCheck();
	LogItem item;
	DaliTextCursor *tc = &cursor();
	Int prevLine = ERR, tmpRow, offset = 0;

	theLog.openInput();

	item = theLog.get();
	while (item.isValid()) {
		tmpRow = item.row();
		if (prevLine == tmpRow)
			offset += item.string().length()-item.length();
		else {
			prevLine = tmpRow;
			offset = 0;
		}
		tc->goTo(tmpRow, item.col()+offset);
		tc->delRange(item.col()+offset,
				item.col()+offset+item.length()-1);
		tc->insString(item.string());
		item = theLog.get();
	}

	theLog.closeInput();

	text = sText;
	newtext = sNewtext;
	flgs = sFlgs;
	filto = sFilto;
	filfrom = sFilfrom;

	cView.value()->goCursor(row, col, true);
	cView.value()->redraw();
}

#define BIT_REALLOC	(8*16)

bool ReplaceText::redo()
{
	idCheck();
	Int tmp = row;
	Int res;
	bool didIt, dontDoIt = false;
	String oldtext, nText;
	row = cursor().order();
	col = cursor().column();
	Int i = 0;
	
	//	Are we doing it for the first time or just redoing ?
	if (tmp != ERR) {
		RegExp re(sText);
		Int numTimes = which.dim();
		
		theLog.openOutput();
		do {
			nText = sNewtext;

			if (sFlgs & CONFIRM)
				dontDoIt = !which[i];
			bool falseVar = false;
			res = cView.value()->replForwText(re,
					nText, oldtext, sFilfrom, sFilto, 
					falseVar, didIt, dontDoIt);

			// At least one ocurrence 
			if (res != ERR && res != ERR-1) {
				if (didIt)
					theLog.put(LogItem(cursor().order(),
							cursor().column() - nText.length(),
							nText.length(), oldtext));
				i++;
			}

		} while (i < numTimes && res != ERR && res != ERR-1);

		theLog.closeOutput();

		cView.value()->findForwText(re, sFilfrom, sFilto,
				false);
	}
	else {
		MsgBox errbox(libMsg("ERROR"),
				Editor::editorMsg("TEXTNOTFOUND"),5,32,6,20);
		PReplaceText menu(Editor::editorMsg("ReplaceText"), 10, 37, 6, 21);

		// restauro el valor del ultimo find
		menu.patron->value() 	= text;
		menu.newtext->value() 	= newtext;

		menu.execute();
		
		if(menu.isAccepted() == true) {
			String tmpStr = menu.getPatron();
            
            // Obtengo estos valores antes, para guardarlos en el history
            // por si la rexp es invalida.
			sText = text = menu.getPatron();
			sNewtext = newtext = menu.getNewText();
			
            *Editor::finds << text;
            *Editor::replaces << newtext;

			RegExp tmpRe(tmpStr);
			if (!tmpRe) {
				MsgBox invRegExp(libMsg("ERROR"),
						Editor::editorMsg("INVREGEXP"), 5, 32, 6, 20);
				(void) invRegExp.execute();
				return false;
			}

			bool noocur = true;
			sFilfrom = filfrom = menu.getFrom();
			sFilto = filto = menu.getTo();
			sFlgs = flgs = menu.getFlags();
            
			RegExp re(text);

			which.realloc(0);
			theLog.openOutput();
			bool confirm = (bool)(flgs&CONFIRM);
			
			Window win((Window::rootWindow()->height()), 0, 1, 
						(Window::rootWindow()->width()),NO_BORDER, 
						NULL_STRING, A_BOLD);

			win.goTo(0,0);
			win << Editor::editorMsg("REPLHLP");
			win.flush();
			
			do {
				nText = newtext;
				res = cView.value()->replForwText(re,nText, oldtext, filfrom, 
													filto, confirm, didIt);

				// hubo al menos una ocurrencia
				if (res != ERR) {
					noocur = false;
					if (res != ERR-1) {
						if (flgs & CONFIRM) {
							if (i >= which.dim())
								which.realloc(which.dim()+BIT_REALLOC);

							which[i] = didIt;
						}
						if (didIt)
							theLog.put(LogItem(cursor().order(),
									cursor().column() - nText.length(),
									nText.length(), oldtext));
						i++;
					}
				}
			} while (flgs&ALL && res != ERR-1 && res != ERR);

			which.realloc(i);

			theLog.closeOutput();

			if (res == ERR && (!(flgs&ALL) || flgs&ALL && noocur)) {
				errbox.execute();
				return false;
			}
		}
	}
	return true;
}

// end clase ReplaceText

// clase ReplaceAgain

void ReplaceAgain::clear()
{
	idCheck();
	rRow = rCol = row = col = prevLen = ERR;
	str = NULL_STRING;
}

void ReplaceAgain::undo()
{
	idCheck();
	if (prevLen != ERR) {
		DaliTextCursor *tc = &cursor();
		tc->goTo(row, col);
		tc->delRange(col, col+prevLen-1);
		tc->insString(str);
	}
	cView.value()->goCursor(rRow, rCol, true);
	cView.value()->redraw();
}

bool ReplaceAgain::redo()
{
	idCheck();
	MsgBox errbox(libMsg("ERROR"),
			Editor::editorMsg("TEXTNOTFOUND"), 5, 32, 6, 20);
	bool didIt = false;

	Int res = ERR;

	if (text != NULL_STRING) {

		RegExp re(text);
		String nText = newtext, oldtext;
		bool firstTime = (rRow == ERR);

		rRow = cursor().order();
		rCol = cursor().column();

		if (flgs & REPLACE) {
			bool auxConf = (firstTime ? (bool) (flgs&CONFIRM): false);
			res = cView.value()->replForwText(re,
					nText, oldtext, filfrom, filto,
					auxConf,
					didIt, (!firstTime && prevLen == ERR));

			if (didIt) {
				row = cursor().order();
				col = cursor().column() - nText.length();
				prevLen = nText.length();
				str = oldtext;
			} else {
				row = col = prevLen = ERR;
				str = NULL_STRING;
			}
		}
	}

	if (res == ERR) {
		errbox.execute();
		return false;
	}

	return true;
}

// end clase ReplaceAgain

// clase GoToLine

void GoToLine::clear()
{
	idCheck();
	lineNo = 0;
}

bool GoToLine::redo()
{
	idCheck();
	if (lineNo == 0) {
		GoToLineDialog dlog;
        
        dlog.execute();
        
		if (dlog.isAccepted())
			lineNo = dlog.value()-1;
		else
			return false;
	}
	if (lineNo == cursor().order()+1 || lineNo == ERR)
		return false;

	DaliView *cview = cView.value();
	Int size = cview->sizeOfText();

	if (size == 0)
		size = 1;

	if (lineNo > size)
		cview->goToEndOfFile();
	else
		(void) cview->goCursor(lineNo, 0, true);

	return true;
}

// fin clase GoToLine

// clase IncFind

void IncFind::clear()
{
	idCheck();
	sFilfrom = sFilto = ERR;
	sFlgs = ERR;
}

void IncFind::undo()
{
	idCheck();
	filfrom = sFilfrom;
	filto = sFilto;
	flgs = sFlgs;

	// swap text & sText
	String temp = text;
	text = sText;
	sText = temp;
}

bool IncFind::redo()
{
	idCheck();
	DaliView *cview = cView.value();
	if (sFlgs == ERR) {
		sFilfrom = filfrom;
		sFilto = filto;
		sFlgs = flgs;
		sText = text;
		filfrom = ERR; // cursor().order()+1;
		text = cview->incSearch();
		row = cursor().order();
		col = cursor().column();
	} else {
		filfrom = ERR; // cursor().order()+1;
		text = sText;
		cview->goCursor(row, col, true);
	}

    // Force next search to end of view
	filto = ERR; 
	flgs = FIND|FORWARD;

	return true;
}

// fin clase IncFind
          
// clase FunctionBrowser

void FunctionBrowser::clear()
{
	idCheck();
	line = ERR;
}

bool FunctionBrowser::redo()
{
	idCheck();
	if (line == ERR) {
		String exp1, rep1, exp2, rep2, title;         
		String aux = cView.value()->getFileName();
		String fileType = String(aux.field(0," ")).baseFullName();
		
		if (Editor::getSourceBrowserExp(fileType, exp1, rep1, exp2,
				rep2, title)) {
			if (!cView.value()->tokenBrowser(line, col, false, 
											exp1, rep1, exp2, rep2, title))
				return false;
		}
		 else
		 	return false;
	}

	cView.value()->goCursor(line, col, true);
	return true;
}
// fin clase FunctionBrowser

// clase TokenBrowser

void TokenBrowser::clear()
{
	idCheck();
	line = ERR;
}

bool TokenBrowser::redo()
{           
	idCheck();
	Int pline = cursor().order();
	if (line == ERR) {
		String target;
		if (chr=='.') {
			if (!cView.value()->tokenBrowser(line, col,Editor::funcExpBrowser,target))
				return false;
		}	
		else {
			// Pido expresion
		 	EdLineDialog dlog(Editor::expressions, Editor::editorMsg("TokenBrowser"),
					"Expression : ", 3, 60, 5, 10);
			if (dlog.execute() == REJECT_EXIT)
				return false;
			target = dlog.value();
			*Editor::expressions << target;

			if (!cView.value()->tokenBrowser(line, col, Editor::funcExpBrowser, target))
				return false;
		}
		text = target;
		if (line < pline)
			 flgs = FIND|BACKWARD;
		else
			 flgs = FIND|FORWARD;	 
	}              
	
	cView.value()->goCursor(line, col, true);
	return true;
}

void TokenCompletion::clear()
{
	offset = ERR;	
}   

bool TokenCompletion::redo()
{           
	idCheck();
    
	DaliView *cview = cView.value();

    if (offset == ERR) {
		previousText = cursor().value();
		offset = cursor().column();
	    cview->tokenCompletion();
	    newText = cursor().value();
	    newOffset = cursor().column();
	} 
	else {
		(void) cview->setLine(newText);
		cview->goCursor(cursor().order(), newOffset);
	}
	
	return true;
}

void TokenCompletion::undo()
{
	idCheck();
	DaliView *cview = cView.value();
	(void) cview->setLine(previousText);
	cview->goCursor(cursor().order(), offset);
}   

// fin clase TokenBrowser


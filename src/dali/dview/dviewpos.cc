/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dviewpos.cc,v 1.4 2002/03/18 21:37:22 hardaiz Exp $
*
* DESCRIPTION: Implementation of dview movement class
*********************************************************************/

#include <ifound.h>
#include <ifound/msgtable.h>
#include <dview.h>
#include <dtext.h>
#include <ifound/stdc.h>
#include <ifound/rexp.h>
#include <winapp/window.h>
#include <edlbox.h>
#include <editor.h>
#include <winapp/strhis.h>

#include <sys/types.h>
#include <sys/stat.h>

#define cursor (*_cursor)
#define block  (*_block)
#define mark   (*_mark)

bool DaliView::cursDown(Int i)
{
	idCheck();
	Int size = text->size();

	if (size == 0) size = 1;

	// si me voy fuera de rango
	if (cursor.order() + i > size+nfils-2) {                                                 
		beep();
		return false;
	}

	actCurrLine(i);

	return false;
}

bool DaliView::cursUp(Int i)
{
	idCheck();
	// me fui de rango
	if (cursor.order()-i < 0) {
		beep();
		return false;
	}

	actCurrLine(-i);

	return false;
}

bool DaliView::cursLeft(Int i, bool wstln)
{
	idCheck();
	if (cursor.column() == 0 && cursor.order() == 0) {
		beep();
		return false;
	}

	cursor.moveCol(-i, true);

	if (!actColOrigin() && !actTopOrigin()) {
		if (wstln)  
		    writeStLine();       
		else
		    goToCursor();
		return false;
	}

	redraw();
	return true;
}

bool DaliView::cursRight(Int i, bool wstln)
{
	idCheck();
	cursor.moveCol(i);

	if (!actColOrigin()) {
		if (wstln) 
			writeStLine();  
		else
			goToCursor();	
		return false;
	}

	redraw();
	return true;
}

bool DaliView::wordRight(Int i)
{
	idCheck();
	Int row, col;

	findNextWord(i, row, col);

	if (row == ERR) {
		beep();
		return false;
	}

	return goCursor(row, col);
}
   
bool DaliView::wordLeft(Int i)
{
	idCheck();
	Int row, col;

	findPrevWord(i, row, col);

	if (row == ERR) {
		beep();
		return false;
	}

	return goCursor(row, col);
}

bool DaliView::goFirstNonSp()
{
	idCheck();
	cursor.goFirstNonSp();

	if (!actColOrigin()) {
		goToCursor();	
		return false;
	}

	redraw();
	return true;
}

void DaliView::goToBegOfFile()
{
	idCheck();
	cursor.goToFirst();
	c_orig = 0;
	f_orig = 0;
	assert(cursor.order() == 0);
	assert(cursor.column() == 0);
    redraw();
	goToCursor();	
}

void DaliView::goToEndOfFile()
{
	idCheck();
	cursor.goToLast();
    f_orig = cursor.order() - nfils + 1;                     
    c_orig = 0;
    redraw();
	goToCursor();	
}

void DaliView::goToEndOfScreen()
{
	idCheck();
	cursor.goTo(f_orig+nfils-1, cursor.column());
	goToCursor();
}

void DaliView::goToMiddleOfScreen()
{
	idCheck();
	cursor.goTo(f_orig+(nfils-1)/2, cursor.column());
	goToCursor();
}

void DaliView::goToBegOfScreen()
{
	idCheck();
	cursor.goTo(f_orig, cursor.column());
	goToCursor();
}

void DaliView::goToForwFuncBegin()
{
	idCheck();
	RegExp re("^{");

	if (findForwText(re, 1, sizeOfText(), cursor.getChar() == '{' ?
										  true : false) == ERR)
		goToEndOfFile();
}

void DaliView::goToBackFuncBegin()
{
	idCheck();
	RegExp re("^{");

	if (findBackText(re, 1, sizeOfText(), cursor.getChar() == '{' ?
										  true : false) == ERR)
		goToBegOfFile();
}

void DaliView::pageDown(Int i)
{
	idCheck();
	Int aux   = cursor.order()+(i*nfils);
	Int size  = text->size();

	if (size == 0) size = 1;

	Int limit = size+nfils-2;

	if (aux <= limit) {
		Int aux1 = f_orig + aux - cursor.order();
		f_orig = aux1 <= size-1 ? aux1 : size-1;
		cursor.goTo(aux);
	}
	else {
		f_orig = size-1;
		cursor.goTo(limit);
		beep();
	}

	redraw();
}

void DaliView::pageUp(Int i)
{
	idCheck();
	Int aux  = cursor.order()-(i*nfils);
		
	if (aux >= 0) {
		Int aux1 = f_orig - cursor.order() + aux;
		f_orig = aux1 >= 0 ? aux1 : 0;
		cursor.goTo(aux);
	}
	else {
		f_orig = 0;
		cursor.goTo(0);
		beep();
	}
		
	redraw();
}

void DaliView::adjustRanges(Int &from, Int &to)
{
	idCheck();
	if (to == ERR)
		to = text->size();

	if (from > to) {
		Int tmp = from;
		from = to;
		to = tmp;
	}

	if (from < 1)
		from = 1;
	else if (from > text->size())
		from = text->size();

	if (to < 1)
		to = 1;
	else if (to > text->size())
		to = text->size();
}

Int DaliView::findBackText(RegExp& re, Int filfrom, Int filto,
						bool addcol)
{
	idCheck();
	Int colto = -(int)addcol;

	// chequeo rangos
	adjustRanges(filfrom, filto);

	// creo el cursor para la recorrida
	DaliTextCursor aux(*text);

	// corrijo la busqueda con respecto al cursor
	if (cursor.order() <= filto-1)  {
		filto = cursor.order()+1;
	    colto = cursor.column() - addcol;
	}

	// now search
	Int col = ERR;
	
	for (aux.goTo(filto-1); aux.order() >= filfrom-1; aux--) {

		col = re.backSearch(aux.value(), colto);            
		
		// lo encontre
		if (col != ERR)
			break;

		colto = INT_MAX;
	}

	// go to text position
	if (aux.order() >= filfrom-1) {
		goCursor(aux.order(), col, true);
		return re.length();
	}

	return ERR;
}

Int DaliView::findForwText(RegExp &re, Int filfrom, Int filto,
					bool addcol, bool usecursor)
{
	idCheck();
	Int colfrom = addcol;

	// ajustar rangos
	adjustRanges(filfrom, filto);

	// creo el cursor para la recorrida
	DaliTextCursor aux(*text);

	// corrijo la busqueda con respecto al cursor
	if (usecursor && cursor.order() >= filfrom-1)  {
		filfrom = cursor.order()+1;
		colfrom = cursor.column() + addcol;
	}

	// now search
	Int col = ERR;
	
	for (aux.goTo(filfrom-1); aux.order() < filto; aux++) {
		col = re.search(aux.value(), colfrom);

		// lo encontre
		if (col != ERR)
		     break;
		     
		colfrom = 0;
	}

	// go to text position
	if (aux.order() < filto) {
		if (usecursor) 
			goCursor(aux.order(), col, true);
		return re.length();
	}

	return ERR;
}

bool DaliView::forwMatch(const String &b, const String &e, Int stack)
{
	idCheck();
	DaliTextCursor tc = cursor;
	if (tc) { 
		//Int stack = 1;
	    Int col = tc.column()+1;
		while (tc) {
		    Int len = tc.value().length();
			for (Int i = col ; i < len; i++) {
				if (tc.value().index(b,i) == i) 
					 stack++;
				if ((tc.value().index(e,i) == i) && --stack == 0) {
					goCursor(tc.order(), i);
					return true;
				}
			}            
			tc++;
			col = 0;
		}
    }
	beep();
	return false;
}

bool DaliView::backMatch(const String &e, const String &b, Int stack)
{
	idCheck();
	DaliTextCursor tc = cursor;

	if (tc) {        
		//Int stack = 1;
	 	Int len = tc.column()-e.length();
		while (tc) { 
			for (Int i = len; i >= 0; i--) { 
				if (tc.value().index(e,i) == i)
					 stack++;
				if ((tc.value().index(b,i) == i) && --stack == 0) {
					goCursor(tc.order(), i);
					return true;
				} 
				if (stack < 0) {
					beep();
				    return false;     
				}    
			} 
			tc--;
			if (tc) len = tc.value().length()-1;
		}
    }           
	beep();
	return false;
}

#define isFunctionChar(c) (isalpha(c) || c == '_' || c=='.' || c=='/' || c=='$')

bool DaliView::getFunction(String &function, bool optimize,
		bool stripType, Int startLine)
{   
	idCheck();
    
    // Test if we should be here. (toDo: Try to do it neater)
    String ext = String(getFileName().field(0," ")).baseFullName().extName();
    if (ext!="C" && ext!="c" && ext!="cc") 
    	return false;

	static Int	lines  	= ERR;
	static Int	uline  	= ERR;                               

	// Still in function ?
	if (optimize && cursor.order() >= uline && cursor.order() <= lines
		&& Editor::functionSensor != FS_OFF)	 
	   	return false;                                             

	function	= NULL_STRING;
	
	lines = (startLine!=ERR) ? startLine : cursor.order();       

	DaliTextCursor aux(*text);

	// ahora hago la busqueda
	Int saved;
  	for (aux.goTo(lines+2); aux.order() > 0; aux--) {
		if ((aux.order()<lines) && aux.value()[0] == '}')
           		break;  // Found end of another function

		if (aux.value()[0] == '{') {
		    String auxval;
			saved = aux.order();
			do {
			  aux--;       
			  auxval = aux.value();
			} while ( 	 aux.order() >= 0 
			 		  && !( 	auxval.index('(') != ERR
			 		  		&& (isalpha(auxval[0]) || auxval[0]=='_') )
				    );

			Int p = auxval.index('(');            
			do 
				p--;
	        while (p >= 0 && isSpace(auxval[p]));                  
	        p++;

	        uline = aux.order();

	        if (lines >= uline) {   
	        	if (!stripType) 
					function = auxval;
	        	else {
		        	function = String(auxval(0,p)).field(-1," ");
		        	Int c = 0;
		        	while ( !isFunctionChar(function[c]) && c < function.length())
	        	          c++;
		        	function = function(c);           
		        }	
				return true;
            }
            
			aux.goTo(saved);
		}
	}
	// No function found now, but check next time.                                                         
	lines   = ERR;
	uline  = ERR;                               
	return true; 
}

#define IFW_ORGFIL		(Window::rootWindow()->height())
#define IFW_ORGCOL		0
#define IFW_NFILS		1
#define IFW_NCOLS	    (Window::rootWindow()->width())

String DaliView::incSearch()
{   
	idCheck();

	Int msglen = msg("IncFind").length()+2;
	Int maxLen = IFW_NCOLS - msglen - 1;
	Int len;
	UnChar c;
	RegExp re;
	bool first = true;
    Int fsState = Editor::functionSensor;
    Editor::functionSensor = FS_NOWON;
    
	String schText, foundStr;
	DaliTextCursor tcFrom = cursor, tcTo = cursor, tcSave = cursor,
			   tcSwap = cursor, tcPrev = cursor;
  	Window wi(IFW_ORGFIL     , IFW_ORGCOL, 
  			   IFW_NFILS      , IFW_NCOLS,
  			   NO_BORDER, NULL_STRING, A_REVERSE);

	DaliView::setFullRedraw(false);

    wi.setAttr(A_NORMAL);	
	wi.goTo(0,0);
	wi << msg("IncFind")+": ";
    wi.setAttr(A_REVERSE);	
	wi.goTo(0,msglen);
	wi >> c;
	while (c != K_END && c != K_ENTER) {
	  switch (c) {
		case K_CURS_DOWN:
			if ((len = findForwText(re, cursor.order()+1, sizeOfText(),
					true)) == ERR)
				wi.beep();
			break;
		case K_CURS_UP:
			if ((len = findBackText(re, 1, cursor.order()+1, true)) ==
					ERR)
				wi.beep();
			break;

		case K_BACKSPACE:
			if (schText.length() == 0) {
				len = ERR;
				wi.beep();
			} else {     
				if (schText.length() == 1)
					schText = NULL_STRING;
				else
					schText = schText(0, schText.length()-1);

				wi.goTo(0, msglen);
				wi.eraseEol();
				if (schText.length() == 0) {
					len = 0;
					cursor = tcSave;
				} else {
					DaliTextCursor tcAux = cursor;
					cursor = tcSave;
					re = noMetaChars(schText);
					wi << schText;
					if ((len = findForwText(re, cursor.order()+1,
							sizeOfText(), false)) == ERR)
						cursor = tcAux;
				}
            }    
			break;	 
		default:
			if (!isprint(c) || schText.length() == maxLen) {
				len = ERR;
				break;
			}                                            

			schText += c;
			re = noMetaChars(schText);
			wi.goTo(0,msglen);
			wi << schText;
			if ((len = findForwText(re, cursor.order()+1, sizeOfText(),
					false)) == ERR) 
				wi.beep();            
	  } //switch

	  if (len != ERR) {

			if (!first)
				text->delAttr(tcFrom, tcTo, A_STANDOUT);
				
			if (len != 0) {
				first   = false;
				tcTo    = cursor;
				tcTo.moveCol(len-1);
				tcPrev  = tcFrom;
				tcFrom  = cursor;
				text->setAttr(tcFrom, tcTo, A_STANDOUT);
			}   
	
			tcSwap = cursor;
			if (tcPrev.column() > tcTo.column()) 
				goCursor(tcTo.order(),tcTo.column()-len+1, true);   
 			else
				goCursor(tcTo.order(),tcTo.column(), true);
	
			foundStr = schText;
			redraw();
			refresh();
			cursor = tcSwap;
	  }	
	  wi >> c;
	} //while

	if (!first) {
		text->delAttr(tcFrom, tcTo, A_STANDOUT);
		redraw();	// false means "Do not update other views"
		refresh();
	}  
	
	if( fsState == FS_OFF ) {
	    Editor::functionSensor = fsState;
	    clearUpperLine();   
	}     
	
	DaliView::setFullRedraw();
	return foundStr;
}

void DaliView::findNextWord(Int i, Int &row, Int &col)
{
	idCheck();
	if (!(cursor)) {
		row = col = ERR;
		return;
	} else {
		if (i < 0) {
			findPrevWord(-i, row, col);
			return;
		}

		DaliTextCursor tc = cursor;

		Int len = tc.value().length();

		Int j;

		while (i-- && tc) {

			// salteo los caracteres <> de blanco
			for (j = tc.column(); j < len && !isSpace(tc.value()[j]);
					j++);

			// recorro a partir de ahi y salteo lineas si hace falta
			// para encontrar el proximo <> de blanco
			for (;tc; tc++, j = 0) {
				len = tc.value().length();

				for (; j < len && isSpace(tc.value()[j]); j++)
					;

				if (j < len)
					break;
			}

			// lo encontre
			if (tc)
				col = j;
		}				

		// si no lo encontre entonces lo coloco al final del archivo
		if (!tc) {
			tc--;
			col = tc.value().length();
		}

		row = tc.order();
	}
}

void DaliView::findPrevWord(Int i, Int &row, Int &col)
{
	idCheck();
	if (!(cursor)) {
		row = col = ERR;
		return;
	} else {
		if (i < 0) {
			findNextWord(-i, row, col);
			return;
		}

		DaliTextCursor tc = cursor;

		Int j;

		while (i-- && tc) {

			j = tc.column()-1;

			while (tc) {

				// salteo los caracteres igual a blanco
				for (; j >= 0 && isSpace(tc.value()[j]); j--);

				if (j >= 0) {
					for (; j >= 0 && !isSpace(tc.value()[j]); j--);
					j++;
					break;
				}

				tc--;
				if (tc)
					j = tc.value().length()-1;
			}

			// lo encontre
			if (tc)
				col = j;
		}				

		// si no lo encontre entonces lo coloco principio del archivo
		if (!tc) {
			tc++;
			col = 0;
		}

		row = tc.order();
	}
}

struct Function {
	String nm;
	Int line,col;
	Function(const String &name, Int lin, Int co=0) : nm(name), line(lin),col(co) { }
};

#include <ifound/parray.h>
#include <winapp/menu.h>

declare(PtrArray,Function);

static PtrArray(Function) funcs;
#define MAX_HEIGHT	19
#define MAX_WIDTH	78


// Mientras se mejora ScrollMenu ...
static String expandTabs(const String &s)
{
  Int len = s.length();
  String rs;
  for (Int a = 0; a < len; a++)
  { 
    if (s[a] != FILL_TAB && s[a] != '\t')
    	rs += s[a];
    else 
        rs += " ";	
  }              
  return rs;
}

static const String fpReadLine(Int n)
{
	if (!funcs.isInRange(n)) return HIGH_VALUE;
	return expandTabs(funcs.elem(n)->nm);
} 
                
bool DaliView::tokenBrowser(Int &lines, Int &col, bool findFunc,
							  String &tgt1   , const String &r1, 
						const String &target2, const String &r2,
						const String &tit)
{
	idCheck();
	bool ret = false;
	DaliTextCursor aux(*text);
	String target1(tgt1);

	lines = cursor.order();
	col  = cursor.column();

	if (target1.isNull()) {
		target1 = getToken();
		if (target1.isNull()) {
			// Pido expresion
		 	EdLineDialog dlog(Editor::expressions, msg("TokenBrowser"),
					"Expression : ", 3, 60, 5, 10);

			dlog.execute();
			if (dlog.isAccepted() != true)
				return false;

			target1 = dlog.value();

			*Editor::expressions << target1;
		} else {
			aux.goTo(lines, col);
			Int ti = aux.value().index("::");
			if ((ti != ERR) && (col > ti+1) &&
					(target1.index("::") != ERR))
				// getToken my way !!
				target1 = target1.field(1, "::");
		}
	}

	// ahora hago la busqueda
	Int filto = text->size();
	Int rcol;
	funcs.reAlloc(0);
	RegExp re1(target1), re2(target2);
	// Check the regular expression received is correct.
	if (!re1 || !re2) {
		beep();
		return false;	
	}
    tgt1 = target1;
    
	String title(tit);

	if (title == NULL_STRING)
		title = msg("TokenBrowser")+" ["+target1+"]";

	String show;
	Int currMatch 	= 0;
	Int	matches 	= 0;

	// Check if it is correct to find out the function name for each match
	if (findFunc) {
	    String ext = String(getFileName().field(0," ")).baseFullName().extName();
        findFunc = (ext != "C" && ext != "c") ? false : true;
	}

    // Searching for regexp
	String currFunc;
	for (aux.goTo(0); aux.order() < filto; aux++) {
		if ((rcol = re1.search(aux.value())) != ERR) {
			if (r1)	show = re1.expand(r1);
			else	show = aux.value();
		} else if (!target2.isNull() &&
				(rcol = re2.search(aux.value())) != ERR) {
			if (r2)	show = re2.expand(r2);
			else	show = aux.value();
		}
        
		if (rcol != ERR) {
			if (lines >= aux.order()) 
				currMatch = matches;

	        if (findFunc) {
		        getFunction(currFunc,false,true,aux.order());
		        show=(currFunc + String(20,' '))(0,20)+String(1, C_VERT)+show;
			}
	        
	        show = show(0, MAX_WIDTH-6);

			funcs.add(tr_new Function(show.rTrim(),aux.order(), rcol));
			matches++;
		}
	}

	// Display a menu with the matching lines
	redraw();
	refresh();
	Int n = funcs.dim();
	if (n > 0) {
		Int smHeight = n;
		if (smHeight > MAX_HEIGHT)
			smHeight = MAX_HEIGHT;

		ScrollMenu sm(title+" ("+toString(n)+")",
				fpReadLine, smHeight, MAX_WIDTH+2);
		sm.setCurrent(currMatch,true);
		Int el = ERR;	
		if ( (sm.execute() != REJECT_EXIT) && (el = sm.currOpt()) != ERR) {
			lines = funcs.elem(el)->line;
			col  = funcs.elem(el)->col;
			ret  = true;
		}
		Int i;
		for (i = 0; i < n; i++)
			tr_delete(funcs.elem(i));

	} else
		beep();
    
	return ret;
}

void DaliView::setMark(const Int mo,const Int mc)
{
	idCheck();
	mark.goTo(mo,mc);
}

void DaliView::setMark()
{   
	idCheck();
	mark = cursor;
}

void DaliView::goMark()
{
	idCheck();
	goCursor(mark.order(),mark.column(), true);
}

void DaliView::tokenCompletion()
{
	idCheck();
    
	Int from, to;
	Int smWidth = 10;
	Int shortestMatchLength = ERR;
	String shortestEntry;
	
   // Get partial token bounds
	cursor.moveCol(-1, true);
	findTokenBounds(from, to);
	cursor.moveCol(1, true);
	
   // Check if got something ...
	if (from == ERROR || to == ERROR) {
		beep();
		return;
	}
	
   // Build partial token
	String tmp = cursor.value();
	String partialToken = tmp(from, to - from);
	
   // Take only the right portion of C++-type tokens
	if ((from = partialToken.index("::")) != ERR)
		partialToken = partialToken.field(1,"::");

   // Verify that the partial token is not null
	if (partialToken.isNull()) {
		redrawCurrLine();
		beep();
		return;
	}

   // Prepare things up
	RegExp re(String("[^a-zA-Z0-9_]\\(") + partialToken + "[a-zA-Z0-9_]*\\)");	
	funcs.reAlloc(0);
    shortestMatchLength = INT_MAX;
    
   // Build list of possible completions
	for (DaliTextCursor aux(*text); aux; ++aux) {
		Int pos = 0;
		while ((pos = re.search(aux.value(), pos)) != ERR) {
			String word = re.expand("\\1");
			pos += word.length();
			if (word.length() > partialToken.length()) {
				bool tokenFound = false;

				// Check if we already have the entry ...
				for (Int i = 0; i < funcs.dim() && !tokenFound; i++) {
					tokenFound = (funcs.elem(i)->nm == word);     
				}
                
                // If it is a new entry, added it and check it's width ...
				if (!tokenFound) {
					funcs.add(tr_new Function(word, ERR));           
					smWidth = max(word.length() + 2, smWidth); // toDo: why '+ 2' ?
					
					if (shortestMatchLength > word.length() + 2) {
						shortestMatchLength = word.length() + 2;
						shortestEntry = word;
					} 
				}
			}
		}
	}

   // Show the list if there are more than one match
	Int n = funcs.dim();
	Int el = n == 1 ? 0 : ERR;	

	String completionStr;

   // If we have more than one possibility, do some extra work ...
	if (n > 1) {
		assert(!shortestEntry.isNull());
	   
	   // Find the maximum common root among the found matches ...
   	    while (completionStr.isNull() && !shortestEntry.isNull()) {

   	    	Int df;
   	    	bool suitableRoot = true;
			for (df = 0; df < n; df++) {
				String aux = funcs.elem(df)->nm;
				if (aux.index(shortestEntry) == ERR) {
					suitableRoot = false;
					break;
				}
			}
			
		   // If we couldn't find a suitable common root,
		   // shorten the possible root, and try again ...
			if (suitableRoot) {
				completionStr = shortestEntry;
			}
			else {
				shortestEntry = shortestEntry(0, shortestEntry.length() -1);
			}
   	    }

       // If the shortestEntry is equal to the initial partial token, we
       // should let the user choose from the menu ...
		if (shortestEntry == partialToken) {
		   // Invalidate the completionStr ...
			completionStr = NULL_STRING;
			
		   // OK ! Show it !
			Int smHeight = n;
			if (smHeight > MAX_HEIGHT)
				smHeight = MAX_HEIGHT;
	        
			if (smWidth > MAX_WIDTH)
				smWidth = MAX_WIDTH;

			ScrollMenu sm(String("[")+toString(n)+"]", fpReadLine, smHeight, smWidth);
			if ( (sm.execute() != REJECT_EXIT) )
				el = sm.currOpt();
		}
	}
    
   // If by this point we don't have a new word to base our completion,
   // take it from the array of possibilities ...
	if (completionStr.isNull() && el != ERR)
		completionStr = funcs.elem(el)->nm;
	
   // Check for last time if we can do the completion job, 
   // if so, go ahead ...
	if (!completionStr.isNull()) {
		// Get the missing portion of the token
		completionStr = completionStr(partialToken.length());

		// Place the cursor at the end of the 'partial' token
		cursor.goTo(cursor.order(), to);
        
        // Insert the missing portion of the token
		cursor.insString(completionStr);
		
		// Place the cursor at the end of the 'completed' token
		cursor.moveCol(completionStr.length());
	}            
	
	// Do some cleaning
	for (Int df = 0; df < n; df++)
		tr_delete(funcs.elem(df));

	redrawCurrLine();
}

/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: eview.cc,v 1.1.1.1 1996/04/16 18:52:46 gustavof Exp $
*
* DESCRIPTION: Implementation of EditableView class
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <sys/types.h>
#include <sys/stat.h>

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

#include <ifound.h>
#include <ifound/message.h>
#include <ifound/stdc.h>
#include <ifound/rexp.h>
#include <ifound/chset.h>

#include <local/strfile.h>

#include <winapp/eview.h>
#include <winapp/etext.h>
#include <winapp/window.h>
#include <winapp/msgbox.h>
#include <winapp/waitmsg.h>

#define cursor 	(*_cursor)
#define block 	(*_block)

// metodos publicos

EditableView::EditableView(String f, Text *t, TextCursor *tc)
: View(	f, 
		t 	== NULL ? constructText(f) 			: t, 
		tc	== NULL ? tr_new EditableTextCursor 	: tc)
{        
	idCheck();
	_block  = tr_new TextCursor;

	// comienzo con modo insert
	mode |= INSERT;
}

EditableView::EditableView(const String &cmd, bool abortable,
							Text *t, TextCursor *tc)
: View(	cmd, abortable,
		t	==	NULL ? constructText(cmd, abortable)	: t,
		tc	== 	NULL ? tr_new EditableTextCursor 			: tc)
{ 
	idCheck();

	_block  = tr_new TextCursor;

	// comienzo con modo insert
	mode |= INSERT;
}

EditableText *EditableView::edText() 
{ 
	idCheck();

	if(text) {
		assert(text->edText() != NULL); 
		return text->edText(); 
	}
	return NULL;
}

EditableTextCursor *EditableView::edCursor() 
{ 
	idCheck();

	if(_cursor) {
		assert(_cursor->edTextCur() != NULL); 
		return _cursor->edTextCur(); 
	}
	return NULL;
}

Text *EditableView::constructText(const String &f)   
	// Used if text has not already been constructed by derived classes
{
	View *pvw;
	if (pvw = existText(f))
		return pvw->getText();

	return tr_new EditableText(f);
}																																	

Text *EditableView::constructText(const String &cmd, bool abortable)
	// Used if text has not already been constructed by derived classes
{
	return tr_new EditableText(cmd, abortable);
}																																	

EditableView::~EditableView()
{   
	idCheck();
    
	// Delete textcursor before deleting text!
	tr_delete(_block);
}

// Escribe la linea de status
void EditableView::_writeStLine()
{         
	idCheck();
	
	View::_writeStLine();
	
	// Write insert/overwrite status
	goTo(nfils, C_MODE);
	if (mode & INSERT)	*this << D_INS;
	else *this << D_OVR;
} 

bool EditableView::isTextMod()
{ 
	idCheck();

	return text->modif();
}

bool EditableView::store()
{          
	idCheck();

	return edText()->store();
}

void EditableView::chgInsMode(bool redrw)
{
	idCheck();

	if (mode&INSERT)
		mode &= ~INSERT;
	else
		mode |= INSERT;

	if (redrw)
		writeStLine();
}

bool EditableView::exit(bool igntxt)
{
	idCheck();
    
   	if (easyExit_)
		return true;

	if (dontExit_)
		return false;

	// si el texto no esta modificado
	if (!isTextMod() || isTextType(ISTREAM))
		return true;                          

	if (existActText() && (igntxt || ntxt != 0))
		return true;                          

	return false;
}

bool EditableView::chgFileName(const String &fileName)
{   
	idCheck();

	// son el mismo archivo
	if (sameFile(fileName , getTextName()))
		return true;
    
	// el archivo esta editado por otra view, por ahora
	// no se puede!
	if (existText(fileName) != NULL)
		return false;
    
	// el archivo no esta editado por ninguna view
	text->setFile(fileName, false);
	label = text->getTextName();  // Is this OK ?
	chgLabel(getLabel(true));
	return true;
}

//
// Attribute management methods
//  

void EditableView::setAttribute(TextCursor &tc, Int attr)
{
	idCheck();

	// estan el la misma posicion 
	if (tc == cursor) return;

	// esta primero el cursor que el tc
	if (tc > cursor)
		text->setAttr(cursor, tc, attr);
	else {
		// estoy dentro del texto
		if (cursor) {
			// estoy parado en numero de columna <> 0
			if (cursor.column()) {
				Int len = cursor.value().length();
				Int col = cursor.column();

				if (len == 0) len = 1;

				if (col < len)
					cursor.moveCol(-1);
				else
					cursor.goToCol(len-1);

				text->setAttr(tc, cursor, attr);

				cursor.goToCol(col);
			}
			// estoy en la columna 0
			else {
				(cursor)--;

				Int len = cursor.value().length();
				if (len == 0) len = 1;

				cursor.goToCol(len-1);

				text->setAttr(tc, cursor, attr);

				cursor.goTo(cursor.order()+1, 0);
			}
		}
		// estoy fuera del texto
		else {
			Int fil = cursor.order();
			Int col = cursor.column();

			cursor.goToLast();

			Int len = cursor.value().length();
			if (len == 0) len = 1;

			cursor.goToCol(len-1);
			text->setAttr(tc, cursor, attr);

			cursor.goTo(fil, col);
		}
	}
}

void EditableView::delAttribute(TextCursor &tc, Int attr)
{
	idCheck();

	// estan el la misma posicion 
	if (tc == cursor) return;

	// esta primero el cursor que el tc
	if (tc > cursor)
		text->delAttr(cursor, tc, attr);
	else {
		// estoy dentro del texto
		if (cursor) {
			// estoy parado en numero de columna <> 0
			if (cursor.column()) {
				Int col = cursor.column();
				Int len = cursor.value().length();

				if (len == 0) len = 1;

				if (col < len)
					cursor.moveCol(-1);
				else
					cursor.goToCol(len-1);

				text->delAttr(tc, cursor, attr);

				cursor.goToCol(col);
			}
			// estoy en la columna 0
			else {
				(cursor)--;

				Int len = cursor.value().length();
				if (len == 0) len = 1;

				cursor.goToCol(len-1);

				text->delAttr(tc, cursor, attr);

				cursor.goTo(cursor.order()+1, 0);
			}
		}
		// estoy fuera del texto
		else {
			Int fil = cursor.order();
			Int col = cursor.column();

			cursor.goToLast();

			Int len = cursor.value().length();
			if (len == 0) len = 1;

			cursor.goToCol(len-1);

			text->delAttr(tc, cursor, attr);

			cursor.goTo(fil, col);
		}
	}
}

//
// Block management methods
//  

bool EditableView::beginBlock()
{
	idCheck();

	if (!(cursor)) {
		beep();
		return false;
	}

	block = cursor;

	writeStLine();

	return true;
}

void EditableView::setBlock(TextCursor &tc)
{
	idCheck();

	// si estaba activo -> borro el activo
	if (block) delBlock(false);

	block = tc;

	blockOn();
	writeStLine();
}

bool EditableView::hasBlock()
{ 
	idCheck();

	if (block) return true;
	return false;
}

TextCursor &EditableView::getBlock()
{
	idCheck();

	return block;
}

void EditableView::delBlock(bool rw)
{
	idCheck();

	if (block) {
		blockOff(rw);
		block.clear();
	}
}

void EditableView::blockOn(bool rw)
{
	idCheck();

	if (block == cursor) {
		if (rw) View::redraw();
		return;
	}

	setAttribute(block, A_STANDOUT);

	if (rw)	View::redraw();
}

void EditableView::blockOff(bool rw)
{
	idCheck();

	delAttribute(block, A_STANDOUT);

	if (rw) View::redraw();
}

Int EditableView:: insertFile(const String &fname)
{
	idCheck();
	EditableText txt(fname);
	txt.activate();

	Int auxfil = cursor.order();

	// si estoy parado afuera
	if (!(cursor)) cursor.ins(NULL_STRING);

	cursor.ins(txt);

	cursor.goTo(auxfil);

	Int ret = txt.size();
	if (ret)
		View::redraw();

	return ret;
}


// end of Editable View																																																																											

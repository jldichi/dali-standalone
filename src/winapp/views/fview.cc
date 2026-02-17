/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: fview.cc,v 1.1.1.1 1996/04/16 18:52:46 gustavof Exp $
*
* DESCRIPTION: Implementation of file-locking & file-preserving 
* 			   methods for class View   
*
*********************************************************************/

#include <ifound.h>
#include <ifound/message.h>
#include <winapp/mldbox.h>
#include <local/strfile.h>
#include <ifound/stdc.h>
#include <ifound/rexp.h>
#include <winapp/window.h>
#include <ifound/chset.h>
#include <winapp/msgbox.h>
#include <winapp/waitmsg.h>

#include <winapp/fview.h>
#include <winapp/ftext.h>

#include <sys/types.h>
#include <sys/stat.h>

#define cursor (*_cursor)
#define block  (*_block)

FileView::FileView(String f, bool isPres, Text *t) 
: EditableView(	f, 
				t 	== NULL ? constructText(f,isPres)	: t ) 
{       
	idCheck();
}

FileView::~FileView()
{   
	idCheck();
}

Text *FileView::constructText(const String &f, bool isPres)   
	// Used if text has not already been constructed by derived classes
{      
	Text *ftext;
	
	if (isPres) {
		ftext = tr_new FileText(f, true);
	}
	else {
		View *pvw;
		if (pvw = existText(f))
			ftext = pvw->getText();
		else		
			ftext = tr_new FileText(f, false);
	}

	return ftext;
}																																	

FileText *FileView::fileText() 
{ 
	idCheck();

	if(text) {
		assert(text->fileText() != NULL); 
		return text->fileText(); 
	}
	return NULL;
}

bool FileView::load(String f)
{
	idCheck();

	if (hidden()) return false;

	View *pvw;
	if (pvw = existText(f))	{	// existe ya el text

		// si es el mismo archivo -> listo
		if (sameFile(f, getTextName()))
			return true;

		// queda alguna con este texto
		if (text->nroViews()!=1) {
			text->decViews();
			subText();				// redespliego el label de views >
			if (!existActText())	// no hay views act. desactivo text
				text->deActivate();
		} else
			tr_delete(text);

		text = pvw->getText();
		ntxt = text->nroViews();
		text->incViews();
		text->activate();

		// si es la segunda view debo redesplegar
		// el label de la primera
		if (ntxt==1) addText();

	// el texto nuevo no existe
	} else {
		if (text->nroViews()!=1) {
			text->decViews();
			subText();				// redespliego el label de views >
			if (!existActText())	// no hay views act. desactivo text
				text->deActivate();
			text = tr_new FileText(f, false);	
			ntxt = text->nroViews();
			text->incViews();
			text->activate();
		} else text->setFile(f);
	}
    
    chgLabel(getLabel(true));

	f_orig = c_orig = 0;

	cursor = *text;

	redraw();

	return true;
}

void FileView::storePreserve()
{         
	idCheck();

	fileText()->storePreserve();
}

void FileView::setPreserveDir(const String &s)
{ 
	FileText::setPreserveDir(s);
}

const String &FileView::getPreserveDir()
{   
	return FileText::getPreserveDir(); 
}

bool FileView::isRdOnly()
{ 
	idCheck();

	return !(fileText()->getFilePerm()&PERM_WRITE);
}

bool FileView::isLocked()
{ 
	idCheck();

	return (fileText()->getFilePerm()&PERM_LOCKED);
}

void FileView::addPath()
{
	idCheck();

	fileText()->addPath();
	label = text->getTextName();
	chgLabel(getLabel());
}

// end of view																																																																											

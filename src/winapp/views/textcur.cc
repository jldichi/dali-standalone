/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: textcur.cc,v 1.1.1.1 1996/04/16 18:52:46 gustavof Exp $
*
* DESCRIPTION: Implementation of TextCursor class
*
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <winapp/text.h>
#include <winapp/window.h>
#include <ifound/stdc.h>

TextCursor::TextCursor() : ListCursor(String)()
{ 
	idStart();

	ncol = 0; 
}

void TextCursor::reset()
{
	idCheck();
	BaseListCursor::reset();
	ncol = 0;	
}   

TextCursor::TextCursor(Text &txt) : ListCursor(String)(txt)
{ 
	idStart();

	ncol = 0; 
}

TextCursor::TextCursor(TextCursor &tc) : ListCursor(String)() 
{ 
	idStart();

	*this = tc; 
}

TextCursor::~TextCursor()
{ 
	idCheck();

	ncol = 0; 

	idStop();
}

bool TextCursor::insChar(char c)
{
	idCheck();

	bool res = ListCursor(String)::value().insChar(c, ncol);
	
	// si no modifique entonces me voy
	if (!res) return false;

	((Text*)list)->setModif();

	// debo modificar los ncol de los cursores 
	// que se encuentran en la misma vnfil y
	// el ncol es >= que el corriente
	for (TextCursor *aux = (TextCursor*)(((Text*)list)->headcur); aux; 
					aux = (TextCursor*)(aux->nextcur)) {
		if (aux != this && aux->order() == order() &&
			ncol <= aux->ncol && value().length()-1 > aux->ncol)
			(aux->ncol)++;
	}

	return true;
}

bool TextCursor::delChar()
{
	idCheck();

	bool res = ListCursor(String)::value().delChar(ncol);

	// si no modifique entonces me voy
	if (!res) return false;

	((Text*)list)->setModif();

	// debo modificar los ncol de los cursores 
	// que se encuentran en la misma vnfil y
	// el ncol es >= que el corriente
	for (TextCursor *aux = (TextCursor*)(((Text*)list)->headcur); aux; 
					aux = (TextCursor*)(aux->nextcur)) {
		if (aux != this && aux->order() == order() && 
			ncol < aux->ncol && value().length()+1 > aux->ncol)
			(aux->ncol)--;	
	}

	return true;
}

void TextCursor::goToFirst()
{
	idCheck();

	ListCursor(String)::goToFirst();					 
	if (*this) ncol = 0;
}

void TextCursor::goToLast()
{
	idCheck();

	ListCursor(String)::goToLast();
	if (*this) ncol = 0;
}

void TextCursor::goFirstNonSp()
{
	idCheck();

	Int len = value().length();
	
	Int i;
	for (i = 0; i < len && isSpace(value()[i]); i++)
		;

	ncol = i;
}

TextCursor &TextCursor::operator=(TextCursor &tc)
{
	idCheck();

	if (this != &tc) {
		ListCursor(String)::operator=(tc);
		this->ncol = tc.ncol;
    }
	return *this;
}

void TextCursor::operator=(Text &t)
{
	idCheck();

	ListCursor(String)::operator=(t);
	this->ncol = 0;
}

void TextCursor::moveCol(Int i, bool cursMove)
{
	idCheck();

	ncol += i;

	while (ncol < 0) {

		// si estoy en la primer linea lo coloco en cero
		if (order() == 0) {
			ncol = 0;
			return;
		}
			
		prev();

		ncol += value().length();
		if (cursMove)
			ncol ++;
	}
}

void TextCursor::goTo(Int f, Int c)
{
	idCheck();

	ListCursor(String)::goTo(f);
	ncol = c;
}

Int TextCursor::cmp(const BaseListCursor &tc) const
{
	idCheck();

	Int c = BaseListCursor::cmp(tc);
	if (c != 0) return c;
	return ncol - ((TextCursor*)&tc)->ncol;
}

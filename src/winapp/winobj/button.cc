/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: button.cc,v 1.3 1998/05/04 15:24:51 mauro Exp $
*
* DESCRIPTION: Implementation of button classes
*
*********************************************************************/
#ifdef __GNUC__
#pragma implementation
#endif

#include <winapp/button.h>
#include <winapp/window.h>
#include <stdio.h>

TextButton::TextButton(Int fo, Int co, const String &txt, Int l,
		BaseAction *a)
:	WinObj(fo, co),
	text_(txt)
{          
	idCheck();

	action = a;
	border = NO_BORDER;
	nfil = 1;

	hotKey_ = text_.index(HKSEQ);
	if (hotKey_ == ERR)
		hotKey_ = 0;
	else
		text_.delChar(hotKey_);
	len_ = (l == ERR) ? text_.length() : l;
}

TextButton::~TextButton()
{
	idCheck();
}

UnChar TextButton::hotKey() const
{
	idCheck();

	return hotKey_ == ERR ? ERR : text_[hotKey_];
}

void TextButton::drawText(Int attr)
{
	idCheck();

	wi->setAttr(attr);
	if (hotKey_ == ERR)
		*wi << text_;
    else {
        *wi << text_(0, hotKey_);
        if(active) wi->setAttr(attr|A_BOLD);
        *wi << text_[hotKey_];
        if(active) wi->setAttr(attr);
        *wi << text_(hotKey_ + 1);
    }
}

#define	PUSHBUTTON_LEFT		"["
#define PUSHBUTTON_RIGHT	"]"

PushButton::PushButton(Int fo, Int co, const String &txt, Int l,
		BaseAction *a)
:	TextButton(fo, co, txt, l, a)
{
	idCheck();

	ncol = len_;
}

Int PushButton::execute(UnChar c)
{
	idCheck();

	bool undisplay = wi->hidden();

	display();
	draw(true);

	wi->goTo(0, 0);

	Int ret = REJECT;

	forever {
		if (toupper(c) == toupper(hotKey())) {
			lastkey = K_ENTER;
			c = 0;
		} else {
			*wi >> c;
			lastkey = c;
		}
		if (lastkey == ' ' || lastkey == K_ENTER) { 
			bool end = false;
			draw(true,true);
			if (action != NULL 
				&& (ret = action->execute()) == ACCEPT_EXIT 
				|| ret == REJECT_EXIT) 
			{
			 	end = true;             
			}
			 	                                  
			draw(true,false);
			
			if (end) break;
		} 
		else if (	lastkey == K_CURS_LEFT 	|| lastkey == K_CURS_RIGHT 	||
					lastkey == K_CURS_UP 	|| lastkey == K_CURS_DOWN 	||
					lastkey == K_END 		|| lastkey == K_TAB 		|| 
					lastkey == K_CTRLX) 
			 {
			 	break;
			 }
	}
	draw(false);

	if (undisplay) 
		unDisplay();

	return lastkey == K_END ? REJECT_EXIT : ret;
}

bool PushButton::display(Int fo, Int co)
{
	idCheck();

	if (!(wi->hidden())) {
		wi->front();
		return true;
	}

	TextButton::display(fo, co);

	wi->wrap(false);

	draw();

	return true;
}

void PushButton::draw(bool hilite,bool pushed)
{
	idCheck();

	if (wi->hidden()) return;

	if (hilite) wi->setAttr(A_REVERSE);
	
	if (pushed)
		(*wi)(0, 0)	<< " " << PUSHBUTTON_LEFT << String(len_ - 4, ' ')
					<< PUSHBUTTON_RIGHT << " ";
	else
		(*wi)(0, 0)	<< PUSHBUTTON_LEFT << String(len_ - 2, ' ')
					<< PUSHBUTTON_RIGHT;
 
 	(*wi)(0, (len_ - text_.length())/2);
	drawText(hilite ? A_REVERSE : A_NORMAL);
	if (hilite) wi->setAttr(A_NORMAL);
	Window::refresh();
}

DefaultButton::DefaultButton(Int fo, Int co, const String &txt, Int l,
		BaseAction *a)
:	PushButton(fo, co, txt, l, a)
{
	idCheck();
}

void DefaultButton::draw(bool hilite,bool pushed)
{
	idCheck();

	if (wi->hidden()) return;

	if (hilite) wi->setAttr(A_REVERSE);

	if (pushed)
		(*wi)(0, 0)	<< " " << PUSHBUTTON_LEFT << PUSHBUTTON_LEFT 
					<< String(len_ - 6, ' ') 
					<< PUSHBUTTON_RIGHT << PUSHBUTTON_RIGHT << " ";
	else
		(*wi)(0, 0)	<< PUSHBUTTON_LEFT << PUSHBUTTON_LEFT
					<< String(len_ - 4, ' ')
					<< PUSHBUTTON_RIGHT << PUSHBUTTON_RIGHT;

	(*wi)(0, (len_ - text_.length())/2);
	drawText(hilite ? A_REVERSE : A_NORMAL);

	if (hilite) 
		wi->setAttr(A_NORMAL);

	Window::refresh();
}

bool DefaultButton::isDefault() const
{
	idCheck();

	return true;
}

#define CHECKBOX_LEN	3

CheckBox::CheckBox(Int fo, Int co, const String &txt, bool def, Int l)
:	TextButton(fo, co, txt, l),
	value_(def)
{
	idCheck();

	ncol = CHECKBOX_LEN + 1 + len_;
	selPttr ="[X]";
	delPttr ="[ ]";
	naPttr = String("[") + String(1,char(C_SHADOW)) + "]";
}

CheckBox::~CheckBox()
{
	idCheck();
}

bool CheckBox::display(Int fo, Int co)
{
	idCheck();

	if (!(wi->hidden())) {
		wi->front();
		return true;
	}

	TextButton::display(fo, co); 
	wi->wrap(false);

	draw();

	return true;
}

void CheckBox::draw(bool hilite)
{
	idCheck();

	if (wi->hidden()) return;
    
	if (hilite) wi->setAttr(A_REVERSE);

	wi->goTo(0, 0);
	if (!active) {
		*wi << naPttr;
	}                                             
	else {
		switch (value_) {
		case true:
			*wi << selPttr;
			break;
		case false:
			*wi << delPttr;
			break;
		case NULL_BOOL:
			*wi << String(CHECKBOX_LEN, ' ');
			break;
		}
	}
	
	if (hilite) wi->setAttr(A_NORMAL);

	if (text_) {
		*wi << ' ';
		drawText();
	}

	(*wi)(0, 0).flush();

	Window::refresh();
}

Int CheckBox::execute(UnChar c)
{
	idCheck();

	bool undisplay = wi->hidden();

	display();

	draw(true);  // Can this be done directly in display() ?

	wi->goTo(0, 0);

	forever {
		if (toupper(c) == toupper(hotKey())) {
			lastkey = ' ';
			c = 0;
		} else 
			*wi >> lastkey;

		if (lastkey != ' ')
			break;    
			
		value_ = ((bool)Boolean(value_) == NULL_BOOL || value_ == false) ? true : false;
		
		draw(true);
	}
    
	Int ret = (action != NULL) 
				? action->execute()
				: (lastkey == K_END) ? REJECT_EXIT : ACCEPT;
	    
	draw(false);
	
	if (undisplay) 
		unDisplay();

	return  ret;
}

#define RADIOBUTTON_LEN	3

RadioButton::RadioButton(Int fo, Int co, const String &txt, Int l, char sch)
:	TextButton(fo, co, txt, l)
{
	idCheck();

	ncol = RADIOBUTTON_LEN + 1 + len_;
	selected = nopts = ERR;
	selChar = sch;
}

RadioButton::~RadioButton()
{
	idCheck();
}

bool RadioButton::display(Int fo, Int co)
{
	idCheck();

	if (!(wi->hidden())) {
		wi->front();
		return true;
	}

	TextButton::display(fo, co); 
	wi->wrap(false);

	draw();

	return true;
}

void RadioButton::draw(bool hilite)
{
	idCheck();

	if (wi->hidden()) return;

	wi->goTo(0, 0);

	if (hilite) 
		drawText(A_REVERSE);
	else                    
		drawText(A_NORMAL);

	for (Int i = 0 ;i <= nopts; ++i)
		options[i]->display();
	
	(*wi)(0, 0).flush();

	Window::refresh();
}

Int RadioButton::execute(UnChar c)
{
	idCheck();

	assert(selected != ERR);
	
	bool undisplay = wi->hidden();

	display();

	draw(true);  // Can this be done directly in display() ?

	wi->goTo(0, 0);

	bool end = false;                      
	lastkey = c;
	bool first = true;
	forever {
		if (toupper(lastkey) == toupper(hotKey()) && !first) {
			select(selected + 1);                 
			*wi >> lastkey;
			continue;
		}           
		else
			*wi >> lastkey;

		first = false;		// I'am sorry
		
		switch (lastkey) {
			case K_CURS_UP:		select(selected - 1);
								break;

			case K_CURS_DOWN:	select(selected + 1);
								break; 

			case K_CURS_LEFT:
			case K_CURS_RIGHT:
			case K_END:
			case K_ENTER:
			case K_TAB:  
			case K_CTRLX: end = true; break;
			
			default: {
				for (Int k=0; k <= nopts; ++k) 
					if (toupper(lastkey) == toupper(options[k]->hotKey())) {
						select(k);                            
						break;
					}
				break;
			}
		};   
		
		if (end) break;
		draw(true);
	}
    
	Int ret = (action != NULL) 
				? action->execute()
				: (lastkey == K_END) ? REJECT_EXIT : ACCEPT;
	    
	draw(false);
	
	if (undisplay) 
		unDisplay();

	return  ret;
}

void RadioButton::select(Int a)
{                    
	idCheck();
	
	// UnSelect previous item
	options[selected]->setValue(false);
	options[selected]->draw();

	// Adjust tr_new value           
	selected = a;
	if(selected < 0)
		selected = nopts;
	else if (selected > nopts)
				selected = 0;		

	// Select tr_new item
	options[selected]->setValue(true);
	options[selected]->draw();
}   

void RadioButton::addItem(const String &opt, bool def)
{             
	idCheck();

	// Discard exceeding items
	if (nopts > MAX_OPTIONS)
		return;
		
	// Create tr_new radio button item
	++nopts;
	options[nopts] = tr_new CheckBox(forig+nopts+2, corig + 2, opt, def);

	if (def) 
		selected = nopts;
			
	// Set look and feel of a radio button
	options[nopts]->setSelectPttr(String("(") + String(1,selChar) + ")");
	options[nopts]->setUnSelectPttr(String("( )"));
	options[nopts]->setInactivePttr(String("(") + String(1,char(C_SHADOW)) + ")");
}                                         


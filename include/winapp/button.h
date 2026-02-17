/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: button.h,v 1.1.1.1 1996/04/16 18:51:42 gustavof Exp $
*
* DESCRIPTION: Class definitions for buttons type.
*
*********************************************************************/

#ifndef BUTTON_H
#define BUTTON_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/winobj.h>

class TextButton : public WinObj
{         

//private:
	TextButton(const TextButton &);
	
protected:

	String text_;
	Int len_;

public:

	TextButton(Int fo, Int co, const String &txt = NULL_STRING,
			Int l = ERR, BaseAction *a = NULL);
	virtual ~TextButton();
	UnChar hotKey() const;
	void drawText(Int attr = A_NORMAL);
};

class PushButton : public TextButton
{
//private:
	PushButton(const PushButton &);
	
public:

	PushButton(Int fo, Int co, const String &txt, 
				Int l = ERR, BaseAction *a = NULL);

	virtual void draw(bool hilite = false,bool pushed = false);

	// virtual methods
	bool display(Int fo = ERR, Int co = ERR);
	Int execute(UnChar c = 0);
};

class DefaultButton : public PushButton
{
//private:
	DefaultButton(const DefaultButton&);
	
public:

	DefaultButton(Int fo, Int co, const String &txt, Int l = ERR,
			BaseAction *a = NULL);

	void draw(bool hilite = false, bool pushed = false);
	bool isDefault() const;
};

class CheckBox : public TextButton
	// On/off toggle button
{

// private:

	bool value_;                        
	
	String selPttr;
	String delPttr;
	String naPttr;
	
	CheckBox(const CheckBox &);
	
public:	

	CheckBox(Int fo = ERR, Int co = ERR, const String &txt = NULL_STRING, 
				bool def = NULL_BOOL, Int l = ERR);
	~CheckBox();

	// some methods
	bool value() const				{ idCheck(); return value_; }
	void setValue(bool v)			{ idCheck(); value_ = v; }
	void draw(bool hilite = false);

	// Pattern management
	void setSelectPttr(const String &s) { idCheck(); selPttr = s;}
	void setUnSelectPttr(const String &s) { idCheck(); delPttr = s;};
	void setInactivePttr(const String &s) { idCheck(); naPttr = s;};

	// virtual methods
	bool display(Int fo = ERR, Int co = ERR);
	Int execute(UnChar c = 0);
};

#define MAX_OPTIONS 10
class RadioButton : public TextButton
{

// private:

	RadioButton(const RadioButton &);

	Int nopts;							// Number of options
	Int selected;						// Selected option
	char selChar;
	
	CheckBox *options[MAX_OPTIONS];
	void select(Int a);
		
public:	

	RadioButton(Int fo = ERR, Int co = ERR, 
				const String &txt = NULL_STRING,
				Int l = ERR, char sch = '*');
				
	~RadioButton();

	// 
	void addItem(const String &opt, bool def = false);

	// some methods
	Int  value() const;
	void setValue(Int v);
	void draw(bool hilite = false);
	
	// virtual methods
	bool display(Int fo = ERR, Int co = ERR);
	Int execute(UnChar c = 0);
};

inline Int RadioButton::value() const
{
	idCheck(); 
	return selected;
}

inline void RadioButton::setValue(Int v)
{ 
	idCheck(); 
	assert(v <= nopts);
	assert(selected != ERR);
	options[selected]->setValue(false); 
	options[selected = v]->setValue(true); 
}

#endif

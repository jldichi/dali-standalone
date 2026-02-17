/*
 * $Id: fmsimpl.h,v 1.1.1.1 1996/04/16 18:51:41 gustavof Exp $
 */

#ifndef FMSIMPL_H
#define FMSIMPL_H

#pragma interface

#include <ideafix/priv/fmval.h>

//////////////////////////////////////////////////////////////////////
//                 text fields: Fm*Text
//////////////////////////////////////////////////////////////////////

class wxItem;
class wxCharField;
class wxNumField;
class wxFloatField;
class wxBoolField;
class wxDateField;
class wxTimeField;

class FmCharText : public FmCharField {
	typedef FmCharField inherited;
	
	DECLARE_FmField(wxCharField);

public:
	FmCharText();
	
	FM_DECLARE_CLASS();
};

class FmNumText : public FmNumField {
	typedef FmNumField inherited;
	
	DECLARE_FmField(wxNumField);

public:
	FmNumText();

	FM_DECLARE_CLASS();
};

class FmFloatText : public FmFloatField {
	typedef FmFloatField inherited;
	
	DECLARE_FmField(wxFloatField);

public:
	FmFloatText();

	FM_DECLARE_CLASS();
};

class FmBoolText : public FmBoolField {
	typedef FmBoolField inherited;
	
	DECLARE_FmField(wxBoolField);

public:
	FmBoolText();

	FM_DECLARE_CLASS();
};

class FmDateText : public FmDateField {
	typedef FmDateField inherited;
	
	DECLARE_FmField(wxDateField);

public:
	FmDateText();

	FM_DECLARE_CLASS();
};

class FmTimeText : public FmTimeField {
	typedef FmTimeField inherited;
	
	DECLARE_FmField(wxTimeField);

public:
	FmTimeText();

	FM_DECLARE_CLASS();
};

#include <ideafix/priv/fmsimpl.icc>

#endif

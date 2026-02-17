#ifndef FMITEM_H
#define FMITEM_H               

#pragma interface

#include <ideafix/priv/fmsimpl.h>

class wxItem;
class wxEvent;

struct ListItem {
	String	value;
	String	name;
	ListItem(const String& theValue, const String& theName = NULL_STRING):
		value(theValue), name(theName!=NULL_STRING?theName:theValue) {};
};	

enum ListItemType {
 	NotDefine,
 	Named,
	InTable,
 	NotNamed
};

class gtype(PtrArray, ListItem);
class wxBitmap;
typedef wxBitmap FmBitmap;

class FmButton : public FmBaseItem
{      
    typedef FmBaseItem inherited;
    
public:
	enum LabelType {
			LABEL_NONE	= 0,
			LABEL_STRING,
			LABEL_BITMAP
	};

protected:
	Fm::Event	actionId_;       
	LabelType 	labelType_; 
	String 		label_;
	String		bitmapName_;
	FmBitmap	*bitmap_;
	FmValueObj *fld_;
	
protected:
  	static void actionFunc(wxItem& item, wxEvent& event);
	virtual wxItem *new_wxItem(wxPanel *panel,
					int x, int y, int w, int h, long style, char *name);

public:
  	FmButton();
  	~FmButton();             

	FM_DECLARE_CLASS();

	void initValues();
	    
 	void label(const String& label);
 	void bitmapName(const String& bitmapName);  	 
 	void bitmap(FmBitmap *bitmap);
	void actionId(Int actionId);
	inline Fm::Event actionId() const;
 	void field(FmValueObj *fld);
 	inline FmValueObj *field() const;
};
/* ------------------------------------------------------------ */


class FmStaticText : public FmBaseItem
{      
    typedef FmBaseItem inherited;
    
	String 		label_;
	
protected:
	virtual wxItem *new_wxItem(wxPanel *panel,
					int x, int y, int w, int h, long style, char *name);

public:
  	FmStaticText();
  	~FmStaticText();             

	FM_DECLARE_CLASS();

 	void label(const String& label);
};

/* ------------------------------------------------------------ */

class wxCheckBoxField;

class FmCheckBox : public FmBoolField
{      
    typedef FmBoolField inherited;
    DECLARE_FmField(wxCheckBoxField);

public:
  	FmCheckBox();
  	~FmCheckBox();             

	FM_DECLARE_CLASS();
};

/* ------------------------------------------------------------ */

class wxListBoxField;
class FmListBox	: public FmNumField
{
	typedef FmNumField inherited;

	DECLARE_FmField(wxListBoxField);
  		 
public:
	FmListBox();
	~FmListBox();
	
	FM_DECLARE_CLASS();
};

/* ------------------------------------------------------------ */

class wxRadioBoxField;
class FmRadioBox : public FmNumField
{      
    typedef FmNumField inherited;
    
    DECLARE_FmField(wxRadioBoxField);

    Int 			orientation_;
	 
public:
  	FmRadioBox();
  	~FmRadioBox();
  	
  	FM_DECLARE_CLASS();

    void orientation(Int orientation);
    inline Int orientation() const;
};

/* ------------------------------------------------------------ */

class wxSliderField;
class FmSlider : public FmNumField
{      
    typedef FmNumField inherited;

    DECLARE_FmField(wxSliderField);
    
	Num min_value_;
	Num max_value_;
    Int orientation_;
	 
	Int numToField(Num value) const;
	
public:
  	FmSlider();
  	~FmSlider();             

	FM_DECLARE_CLASS();
 	
	void min_value(Num min_value);
	void max_value(Num max_value);
    void orientation(Int orientation);
    inline Int orientation() const;
};

#include <ideafix/priv/fmitem.icc>

#endif /* FMITEM_H */

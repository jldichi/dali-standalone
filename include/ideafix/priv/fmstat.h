#ifndef FMSTATIC_H
#define FMSTATIC_H

#pragma interface

#include <ideafix/priv/fmbase.h>

class wxBitmap;
typedef wxBitmap FmBitmap;

class FmStaticBox : public FmBaseStatic {
    typedef FmBaseStatic inherited;

	Int shadow_;
    ColorVal *box_;
    ColorVal *border_;
    Int mask_;
    Int sstyle_;

protected:    
    virtual wxStaticItem *new_wxStatic(wxPanel *panel,
				int x, int y, int w, int h, long style, char *name);
				
public:
  	FmStaticBox();
  	~FmStaticBox();

	FM_DECLARE_CLASS();

	void initValues();
	    
	void shadow(Int shadow);
    void box(const ColorVal &box);
    void border(const ColorVal &border);
    void mask(Int mask);
    void sstyle(Int sstyle);
};

class FmStaticSeparator	: public FmBaseStatic {
    typedef FmBaseStatic inherited;

	Int layout_;
	Int shadow_;
    ColorVal *border_;
    Int sstyle_;
    
protected:    
    virtual wxStaticItem *new_wxStatic(wxPanel *panel,
				int x, int y, int w, int h, long style, char *name);
				
public:
  	FmStaticSeparator();
  	~FmStaticSeparator();

	FM_DECLARE_CLASS();

	void initValues();
	    
	void layout(Int layout);
	void shadow(Int shadow);
    void border(const ColorVal &border);
    void sstyle(Int sstyle);
};

class FmStaticBitmap : public FmBaseStatic {
    typedef FmBaseStatic inherited;

	String bitmapName_;

protected:    
    virtual wxStaticItem *new_wxStatic(wxPanel *panel,
				int x, int y, int w, int h, long style, char *name);
				
public:
  	FmStaticBitmap();
  	~FmStaticBitmap();

	FM_DECLARE_CLASS();

	void initValues();
	    
	void bitmapName(const String& bitmapName);
};
 
#endif /* FMSTATIC_H */

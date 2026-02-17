#ifndef SLIDE_H
#define SLIDE_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <winapp/winobj.h>
#include <winapp/button.h>
#include <ifound/action.h>

#define	INITSTR	"mín."
#define	ENDSTR	"máx."
#define	PERCENT	"%p"
#define LITCALIF "1,2,3,4,5"

class Slide : public TextButton
{         

//private:
	Slide(const Slide &);
	Int currval;
	String startstr, endstr;
	String fmt;
	Int granularity;

protected:
	virtual void drawLimits(void);
	virtual String expand(void);
	
public:

	Slide(Int fo, Int co, const String &txt = NULL_STRING, 
			Int l = ERR, 
			const String &format = PERCENT,
			const String &s_str = INITSTR, 
			const String &e_str = ENDSTR,
			BaseAction *a = NULL);

	Int getPercent(void);
	Int	getValue(void);
	void setGranularity(const int ng);

	virtual bool display(Int fo = ERR, Int co = ERR);
	Int execute(UnChar c = 0);

	// virtual methods
	virtual void draw(bool act = true);
};

class CompactSlide : public Slide 
{

public:
	CompactSlide(Int fo, Int co, const String &txt = NULL_STRING, 
			Int l = ERR, const String &format = PERCENT, 
			const String &lits = LITCALIF) 
			: Slide(fo,co,txt,l,format,lits) 
		{ idCheck(); ncol = l + text_.length() + expand().length() + 8; }
			
	// virtual methods
	virtual bool display(Int fo = ERR, Int co = ERR);
};

#endif
        









            
            
           

/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: prefcomm.h,v 1.1.1.1 1996/04/16 18:52:54 gustavof Exp $
*
* DESCRIPTION: Headers of preference command
*********************************************************************/

#include <str.h>
#include <panel.h>
#include <command.h>

class StrCell;
class FStrCell;
class Label;

BeginAnyCommand(Preferences)
	Int tw;		// tab width
	Int mv;		// max views
	Int bk;		// backups
	String ca, fma, rpa, sca, mma;		// F5 actions
public:
EndAnyCommand(Preferences);

// define el string de seleccion de los flags
#define STR_SEL	"[X]"
#define STR_DEL "[ ]"

// panel para la captura de datos para el find
class PrefBox : public Panel
{

private:

	Int tw;		// tab width
	Int mv;		// max views
	Int bk;		// backups
	String ca, fma, rpa, sca, mma;		// F5 actions

    void operator=(const PrefBox&) ;
           PrefBox(const PrefBox&) ;
    
public:

	// winobjs del panel
	FStrCell  	 *tabwidth;
	FStrCell  	 *maxviews;
	StrCell		 *backups;
	StrCell		 *c_action;
	StrCell		 *fm_action;
	StrCell		 *rp_action;
	StrCell		 *sc_action;
	StrCell		 *mm_action;
	StrCell		 *ok;
	StrCell		 *cancel;

	// labels del panel
	Label    *l_tabwidth;
	Label    *l_maxwidth;
	Label    *l_backups;
	Label    *l_action;
	Label	 	*l_c;
	Label    *l_fm;
	Label    *l_rp;
	Label    *l_sc;
	Label    *l_mm;

	PrefBox(String label, Int nf=ERROR, Int nc=ERROR, Int fo=ERROR,
			  Int co=ERROR);
	virtual ~PrefBox();

	virtual Int execute(UnChar c = 0);

	// resultados del panel
	Int gettw()		{ return tw; }
	Int getmv()		{ return mv; }
	Int getbk()		{ return bk; }
	String getca()			{ return ca; }
	String getfma()		{ return fma; }
	String getrpa()		{ return rpa; }
	String getsca()		{ return sca; }
	String getmma()		{ return mma; }

};

/*
Bool SelFOk(PFindText &p);
Bool SelFCancel(PFindText &p);
Bool SelFForward(PFindText &p);
Bool SelFPatron(PFindText &p);
*/


/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: schmcomm.h,v 1.1.1.1 1996/04/16 18:52:54 gustavof Exp $
*
* DESCRIPTION: Headers of search command's menus
*********************************************************************/

#include <winapp/panel.h>
#include <ifound/str.h>

#ifdef __GNUC__
#pragma interface
#endif

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

class StrCell;
class FStrCell;
class Label;
class PushButton;
class DefaultButton;
class CheckBox;

// valor de los flags
#define	BACKWARD	0x01
#define FORWARD		0x02
#define	FIND		0x04
#define REPLACE		0x08
#define ALL			0x10
#define CONFIRM		0x20

class PFindText : public Panel
	// panel para la captura de datos para el find
{
// private:
    
	String 	re;
	Int	 	flags;
	Int  	desde;
	Int  	hasta;

    PFindText(const PFindText&) ;
    void operator=(const PFindText&) ;
       
public:

	// winobjs del panel
	StrCell			*patron;
	CheckBox		*forw;
	FStrCell		*rfrom;
	FStrCell		*rto;
	DefaultButton	*ok_;
	PushButton		*cancel_;

	// labels del panel
	Label    *ttf;
	Label	 *tfrom;
	Label    *tto;

	PFindText(String label, Int nf=ERROR, Int nc=ERROR, Int fo=ERROR,
			  Int co=ERROR);
	virtual ~PFindText();

	// resultados del panel
	const String &getPatron()	{ idCheck(); return re;    }
	Int getFlags()				{ idCheck(); return flags; }
	Int getFrom()				{ idCheck(); return desde; }
	Int getTo()					{ idCheck(); return hasta; }

	Int   ok();
 	Int   cancel(); 
};

// debo crear un panel para hacer el find
class PReplaceText : public Panel
{
// private:

	String 	re;
	String 	newt;
	Int	 	flags;
	Int  	desde;
	Int  	hasta;

    PReplaceText(const PReplaceText&) ;
    void operator=(const PReplaceText&) ;

public:

	// winobjs del panel
	StrCell			*patron;
	StrCell			*newtext;
	CheckBox		*all;
	CheckBox		*confirm;
	FStrCell		*rfrom;
	FStrCell		*rto;
	DefaultButton	*ok_;
	PushButton		*cancel_;

	// labels del panel
	Label    *ttf;
	Label    *tnt;
	Label	 *tfrom;
	Label    *tto;

	PReplaceText(String label, Int nf=ERROR, Int nc=ERROR, Int fo=ERROR,
			  Int co=ERROR);
	virtual ~PReplaceText();

	// resultados del panel
	const String &getPatron()	{ idCheck();  return re;    }
	const String &getNewText()	{ idCheck();  return newt;  }
	Int getFlags()				{ idCheck();  return flags; }
	Int getFrom()				{ idCheck();  return desde; }
	Int getTo()					{ idCheck();  return hasta; }

	Int ok();
 	Int cancel(); 
};

class GoToLineDialog : public Panel
	// clase para el Dialog de GoToLine, captura el numero de linea.
{
// private:
	Int			lineNo;
	
	GoToLineDialog(const GoToLineDialog&) ;
    void operator=(const GoToLineDialog&) ;
    
public:

	// objetos del panel
	FStrCell	*lineNoFld;

	// labels del panel
	Label		*staticText;

	GoToLineDialog();
	virtual ~GoToLineDialog();

	const Int &value()	{ idCheck();  return lineNo; }

	virtual Int ok();
};


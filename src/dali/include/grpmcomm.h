/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: grpmcomm.h,v 1.1.1.1 1996/04/16 18:52:54 gustavof Exp $
*
* DESCRIPTION: Headers of search command's menus
*********************************************************************/

#ifndef GRPMCOMM_H
#define GRPMCOMM_H

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/panel.h>
#include <ifound/str.h>

#ifdef ISDEBUG
#	include <ifound/iddebug.h>
#endif

class StrCell;
class Label;
class PushButton;
class DefaultButton;
class CheckBox;

class GrepPanel : public Panel
	// panel para la captura de datos para el find
{
private:
    
	String 	expression;
	String 	grepFiles;
	String  grepname;
	String  grepcmd;
	bool 	caseIgnored;
	bool 	excludeExpr;
	bool 	printLines;
	bool 	editedFiles;

    GrepPanel(const GrepPanel&) ;
    void operator=(const GrepPanel&) ;
       
public:

	// winobjs del panel
	StrCell			*expression_;
	StrCell			*grepFiles_;
	CheckBox		*caseIgnored_;
	CheckBox		*excludeExpr_;
	CheckBox		*printLines_;
	CheckBox		*editedFiles_;
	DefaultButton	*grep_;
	PushButton		*egrep_;
	PushButton		*cancel_;

	// labels del panel
	Label    *lblExp;
	Label    *lblFiles;


	// With this panel, I change the tradition of passing the number of 
	// rows and cols to it. I think the user should only be able to 
	// provide the starting position of the panel. Bruno.
	
	GrepPanel(String label, Int fo=ERR, Int co=ERR, 
				const bool &edFiles = false);
				
	virtual ~GrepPanel();

	// resultados del panel
	const	String &getFiles()	 { idCheck(); return grepFiles;		}
	const	String &getGrepCmd();
	bool	useEditedFiles()	 { idCheck(); return editedFiles; 	}

	Int   gfStatus();
	Int   setOptions();
	Int   grep();
	Int   egrep();
 	Int   cancel(); 
};

#endif

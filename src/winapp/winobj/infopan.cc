/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: infopan.cc,v 1.1.1.1 1996/04/16 18:52:45 gustavof Exp $         
*
* DESCRIPTION: Implementation of info panel
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <winapp/mldbox.h>
#include <winapp/label.h>
#include <winapp/winobj.h>
#include <winapp/infopan.h>
#include <ifound/str.h>

InfoPanel::InfoPanel(const String &l, Int nf, Int nc, Int fo, Int co)
: Panel(l, nf, nc, fo, co)
{                             
	idCheck();
	
	assert (nfil < MAX_INFOLINES);
	
   // Intialization of labels
    Int la;
	for (la = 0; la < nfil ; ++la) 
		msg[la] = tr_new Label(NULL_STRING, la, 2, ncol-2);               

    assert (la == nfil);
    
   // inicializacion del panel
	InitLabels(&msg[0], nfil);
}

InfoPanel::~InfoPanel()
{
	idCheck();

	for (Int a = 0; a < nfil; a++)
		tr_delete(msg[a]);

	idCheck();
}

void InfoPanel::show(const String &strinfo)
{        
	idCheck();

	Int nlab = 0;
	
	for (StrCursor sc(strinfo,"\n"); nlab < nfil; nlab++) {
		if (sc) {
			String infoline = sc.value();
			msg[nlab]->setValue(infoline(0, ncol-2));
			++sc;
		} 
		else 
			msg[nlab]->setValue(NULL_STRING);
			
		msg[nlab]->draw();
	}
}



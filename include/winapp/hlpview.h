/********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: hlpview.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Class definitions for HelpView type.
*********************************************************************/

#ifndef HLPVIEW_H
#define HLPVIEW_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#include <winapp/window.h>
#include <ifound/str.h>
#include <ifound/itypes.h>
#include <ifound/msgtable.h>

#include <winapp/winapp.h>
#include <winapp/text.h>

struct CrossReference {
	String key;
	Int srow;
	Int scol;
	Int erow;
	Int ecol;
	CrossReference(const String &k, Int sr, Int sc, Int er, Int ec) 
		: key(k), srow(sr), scol(sc), erow(er), ecol(ec) {}
}; 

class gtype(PtrArray, CrossReference);

class HelpView : public EditableView {
//private:

	gtype(PtrArray, CrossReference) *xrefs;

	void goToXRef(Int el);
	Int current;
	
public:
	HelpView(const String &label, const String &contents, 
				const String &key, WinApp *app);
	~HelpView();
 
 	HelpView *helpView() { idCheck(); return this; }

	void goToNext();
	void goToPrev();
	String getXRefKey();
};

#endif

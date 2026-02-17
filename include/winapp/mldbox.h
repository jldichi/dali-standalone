/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: mldbox.h,v 1.1.1.1 1996/04/16 18:51:43 gustavof Exp $
*
* DESCRIPTION: Class definitions for dialog box type.
*
*********************************************************************/

#ifndef MLDBOX_H
#define MLDBOX_H

#include <ifound.h>

#ifdef __GNUC__
#pragma interface
#endif

#include <winapp/panel.h>

class PullDownMenu;
class Label;
class TextButton;

// valores retornados por el execute
#define	DBOX_YES	0
#define DBOX_NO		1
#define	DBOX_CANCEL	2

#define MAX_LABELS  10

class MLDBox : public Panel
{

// private:

    MLDBox(const MLDBox &);
    void operator=(const MLDBox &);

    Int nLabels;	// Cantidad de labels que coloque en el panel
    Int option;

public:

	TextButton *yes_;
	TextButton *no_;
	TextButton *cancel_;

	// labels del panel
	Label    *msg[MAX_LABELS];

	MLDBox(String label, String text, Int def = DBOX_YES);
	virtual ~MLDBox();

	Int getOpt() const { idCheck(); return isAccepted() ? option : DBOX_CANCEL; }	
	Int yes();
	Int no();
	Int cancel();
	Int execute(UnChar c = 0);
};

#endif

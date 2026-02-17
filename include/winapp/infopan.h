/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: infopan.h,v 1.1.1.1 1996/04/16 18:51:44 gustavof Exp $
*
* DESCRIPTION: Class definitions for dialog box type.
*
*********************************************************************/

#ifndef INFOPAN_H
#define INFOPAN_H

#include <ifound.h>

#ifdef __GNUC__
#	pragma interface
#endif

#include <winapp/panel.h>

class Label;

#define MAX_INFOLINES 20

class InfoPanel : public Panel
{

// private:

    InfoPanel(const InfoPanel &);
    void operator=(const InfoPanel &);

    Int nLabels;	// Cantidad de labels que coloque en el panel

	// labels del panel
	Label    *msg[MAX_INFOLINES];

public:
	InfoPanel(const String &label, Int rows, Int cols, Int forig, Int corig);
	virtual ~InfoPanel();
	void show(const String &strinfo);
};

#endif

/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dirsel.cc,v 1.1.1.1 1996/04/16 18:52:45 gustavof Exp $
*
* DESCRIPTION: Implementation of directory selector class
*
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <winapp/dirsel.h>
#include <winapp/menu.h>
#include <winapp/cell.h>                        
#include <winapp/button.h>
#include <winapp/winobj.h>

declare(ObjectAction, DirSelector);
implement(ObjectAction, DirSelector);

DirSelector::DirSelector(String l, Int nf, Int nc, Int fo, Int co,
						 const String &cwd)
: Panel(l, nf, nc, fo, co)
{
	idCheck();

	name  = tr_new StrCell(LONG_CWD-1, 25, 0, 1, 2);
	dir   = tr_new DirMenu(8, 20, 4, 4, cwd);
	dir->setAction(tr_new ObjectAction(DirSelector)(this, &DirSelector::getDir));

 	// OK and CANCEL buttons
	ok_      = tr_new DefaultButton(14,  3,libMsg("OK"), 10,
		defaultAction = tr_new ObjectAction(DirSelector)(this, &DirSelector::ok));
	cancel_  = tr_new PushButton(14, 17,libMsg("CANCEL"), 10,
		tr_new ObjectAction(DirSelector)(this, &DirSelector::cancel));

	InitObjs((WinObj**)&name, 4);

	// Start off at the cwd.
	name->value() = dir->retCwd();
	dir->setCurrent(0, FALSE);
}

DirSelector::~DirSelector()
{
	idCheck();

	tr_delete(name);
	tr_delete(dir);
	tr_delete(ok_);
	tr_delete(cancel_);
}

Int DirSelector::getDir()
{     
	idCheck();
	
	name->value() = dir->retDir();
    name->writeStr();
    
	return ACCEPT;
}

Int DirSelector::ok()
{     
	idCheck();

	strres = name->value().expandEnvVars();

	// If nothing has changed return REJECT_EXIT as if an error had ocurred
	return (strres == dir->retCwd()) ? REJECT_EXIT : ACCEPT_EXIT;
}

Int DirSelector::cancel()
{     
	idCheck();

	return REJECT_EXIT;
}


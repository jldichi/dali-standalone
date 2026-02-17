/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: edhelp.h,v 1.1.1.1 1996/04/16 18:52:53 gustavof Exp $
*
* DESCRIPTION: Interface to editor help class
*********************************************************************/

#ifndef EDHELP_H
#define EDHELP_H

#ifdef __GNUC__
#pragma interface
#endif

#include <help.h>
#include <ifound/str.h>

class EditorHelp : public Help {
                
private:
   EditorHelp(const EditorHelp&) ;                
   void operator=(const EditorHelp&) ; 
   
public:
		EditorHelp() : Help() { }
		EditorHelp(const String &module) : Help(module) { }

		String getIndex(Int index) const;
};

#endif // EDHELP_H

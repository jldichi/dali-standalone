/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: edfsel.h,v 1.2 2002/03/18 19:41:48 hardaiz Exp $
*
* DESCRIPTION:	Implementation of Editor File Selector
*********************************************************************/

#include <winapp/filesel.h>
#include <editor.h>

#ifdef __GNUC__
#pragma interface
#endif

class StrHistory;

class EdFileSelector : public FileSelector
{
  
 private: 
 	StrHistory *strHis;           
 	bool 	   fsPanel;	
 	
 	EdFileSelector(const EdFileSelector&) ;
 	void operator=(const EdFileSelector&) ;

 public:
 
 	EdFileSelector(String label, Int nf=ERROR, Int nc=ERROR, Int fo=ERROR, 
	  	 Int co=ERROR, StrHistory *his=NULL, const String &cwd=NULL_STRING,
	  	 const String &schpath=NULL_STRING);
		
	virtual ~EdFileSelector();
	virtual Int execute(UnChar = 0);
	bool isPanelOn()   	{ return fsPanel; };
};



/********************************************************************
* Copyright (c) 1992 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*
* $Id: formmsg.h,v 1.3 1996/09/18 14:03:06 valeria Exp $
*
*
* DESCRIPTION:
*
*********************************************************************/

#ifndef _FORMMSG_H
#define _FORMMSG_H

class FormMsgTable {
// private:

	MsgTable *msgTable_d;
	String tabName_d;
	String tab_d;
	String sec_d;

	MsgTable *msgTable()
	{
		if (msgTable_d == NULL)
			msgTable_d = tr_new MsgTable(tab_d, sec_d);
		return msgTable_d;
	}

public:

	FormMsgTable(const String &tabName, const String &tab, const String &sec)
	:	msgTable_d(NULL),
		tabName_d(tabName),
		tab_d(tab),
		sec_d(sec)
	{ }
	~FormMsgTable()
	{ 
		tr_delete(msgTable_d);
	}

	String find(const String &name)
	{
		return msgTable()->find(name);
	}

	String find(const String &sec, const String &name)
	{
		return msgTable()->find(sec,name);
	}

	const String &tabName()		{ return tabName_d;	}
	const String &tab()			{ return tab_d;		}
	const String &sec()			{ return sec_d;		}
};

#endif

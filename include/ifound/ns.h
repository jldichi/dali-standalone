/*********************************************************************
* Copyright (c) 1993 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id 1994/03/23 13:14:19 santiago Exp santiago $
*
* DESCRIPTION
*********************************************************************/

#ifndef NS_H
#define NS_H

#include <essentia/dbconst.h>		// NAMESPACE
#include <usrsrv/sconnect.h>		// ShadowConnection

#ifdef ISDEBUG
#	include <essentia/svdebug.h>
#endif

#include <ifound/str.h>
class Client;
#define DeclareNameSpace								\
														\
public:													\
														\
	Int objectType() const { return NAMESPACE; }		\
	String objType() { return "NameSpace"; }			\
														\
private:												\
														\
	void objMethodExec(Client *);


#define DefineNameSpace(cname)							\
														\
typedef void (cname::*NSMethod)();						\
static NSMethod nSMethods[] = paste(cname, Methods);	\
														\
void cname::objMethodExec(Client *)						\
{								 						\
	Int operation;										\
					  									\
	scon->readMsg();									\
	*scon >> operation;				 					\
														\
	if (operation < 0 ||								\
			operation >= paste(cname, _lastmsg))		\
		error(appMsg("NOVALIDMSG"));					\
														\
	(this->*nSMethods[operation])();								\
}

#endif // NS_H

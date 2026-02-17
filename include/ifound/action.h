/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: action.h,v 1.1.1.1 1996/04/16 18:51:32 gustavof Exp $
*
* DESCRIPTION: Class definitions for Actions.
*
*********************************************************************/

#ifndef ACTION_H
#define ACTION_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

class BaseAction {
	useIdCheck(BaseAction_id);

public:

	BaseAction() { idStart(); }
	virtual Int execute() = 0;
	virtual ~BaseAction() { idCheck(); idStop(); }
};

//
//	Class ObjectAction - Provides a 'C++' interface to actions
//
#define ObjectAction(CONTENT)		name2(CONTENT, ObjectAction)

#define	ObjectActiondeclare(CONTENT) 							\
																\
typedef Int (CONTENT::*name2(CONTENT, ObjMFP))();				\
																\
class ObjectAction(CONTENT) : public BaseAction { 				\
																\
	CONTENT *obj_;												\
	name2(CONTENT, ObjMFP) act_;								\
																\
public:															\
																\
	ObjectAction(CONTENT)(CONTENT *o, name2(CONTENT, ObjMFP) a);\
	~ObjectAction(CONTENT)();									\
																\
	Int execute();												\
																\
};

#define ObjectActionimplement(CONTENT)							\
	ObjectAction(CONTENT)::ObjectAction(CONTENT)				\
		(CONTENT *o, name2(CONTENT, ObjMFP) a)					\
		: obj_(o), act_(a) { }									\
	ObjectAction(CONTENT)::~ObjectAction(CONTENT)()				\
		{ }														\
	Int ObjectAction(CONTENT)::execute()						\
		{ idCheck(); return (obj_->*act_)(); }

//
//	Class GlobalAction - Provides a 'C' interface to actions
//
typedef Int (*IFPV)();

class GlobalAction : public BaseAction {

	IFPV act_;	

public:       

	inline explicit GlobalAction(IFPV act);
	inline Int execute();

};

GlobalAction::GlobalAction(IFPV act)
: act_(act)
{ }

Int GlobalAction::execute()		
{
	idCheck();
	return (*act_)();
}

#endif


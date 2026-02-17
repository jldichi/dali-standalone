/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: action2.h,v 1.2 1998/04/21 15:10:44 guiller Exp $
*
* DESCRIPTION: Class definitions for Actions.
*
*********************************************************************/

#ifndef ACTION2_H
#define ACTION2_H

#ifdef __GNUC__
#pragma interface
#endif

#include <ifound.h>

#define BaseAction(ARG1, ARG2)	name3(ARG1, ARG2, BaseAction)

#define	BaseActiondeclare2(ARG1, ARG2)							\
                                                                \
class BaseAction(ARG1, ARG2) {                                  \
                                                                \
public:                                                         \
                                                                \
	BaseAction(ARG1, ARG2)() { }                                \
	virtual operator Int() = 0;                                 \
	virtual Int execute(ARG1, ARG2) = 0;                        \
	virtual ~BaseAction(ARG1, ARG2)() { }                       \
};

//
//	Class ObjectAction - Provides a 'C++' interface to actions
//
#define ObjectAction3(CONTENT, ARG1, ARG2) name4(CONTENT, ARG1, ARG2, ObjectAction)

#define	ObjectActiondeclare3(CONTENT, ARG1, ARG2)				\
                                                                \
typedef Int (CONTENT::*name2(CONTENT, ObjMFP))(ARG1, ARG2);		\
																\
class ObjectAction3(CONTENT, ARG1, ARG2) 						\
	: public BaseAction(ARG1, ARG2) {							\
																\
	CONTENT *obj_;												\
	name2(CONTENT, ObjMFP) act_;								\
																\
public:															\
																\
	explicit ObjectAction3(CONTENT,ARG1,ARG2)					\
		(CONTENT *o = 0, name2(CONTENT, ObjMFP) a = 0);			\
	~ObjectAction3(CONTENT,ARG1,ARG2) ();						\
																\
	virtual operator Int() 	{ return (obj_ && act_); } 			\
	Int execute(ARG1 arg1, ARG2 arg2)							\
	{ return (obj_->*act_)(arg1, arg2); }						\
																\
};

#define ObjectActionimplement3(CONTENT, ARG1, ARG2)				\
	ObjectAction3(CONTENT,ARG1,ARG2)::ObjectAction3(CONTENT,	\
		ARG1,ARG2)												\
		(CONTENT *o, name2(CONTENT, ObjMFP) a)					\
		: obj_(o), act_(a) { }							 		\
																\
	ObjectAction3(CONTENT,ARG1,ARG2)::~ObjectAction3(CONTENT, 	\
		ARG1,ARG2) () { }

//
//	Class GlobalAction - Provides a 'C' interface to actions
//
#define GlobalAction(ARG1, ARG2) name3(ARG1, ARG2, GlobalAction)

#define	GlobalActiondeclare2(ARG1, ARG2)						\
																\
typedef Int (*IFP__)(ARG1, ARG2);								\
                                                                \
class GlobalAction(ARG1, ARG2) 									\
	: public BaseAction(ARG1, ARG2) {                         	\
                                                                \
	IFP__ act_;	                                                \
                                                                \
public:                                                         \
	GlobalAction(ARG1, ARG2)(IFP__ act);                        \
	~GlobalAction(ARG1, ARG2)();								\
                                                                \
	virtual operator Int() 	{ return Int(act_); } 				\
	Int execute(ARG1 arg1, ARG2 arg2)	                        \
		{ return (*act_)(arg1, arg2); }    						\
                                                                \
};

#define GlobalActionimplement2(ARG1, ARG2)						\
	GlobalAction(ARG1, ARG2)::GlobalAction(ARG1, ARG2)			\
		(IFP__ act) : act_(act) { }								\
	GlobalAction(ARG1, ARG2)::~GlobalAction(ARG1, ARG2)() { }

#endif

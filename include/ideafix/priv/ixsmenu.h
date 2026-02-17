/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ixsmenu.h,v 1.3 2002/11/01 19:59:12 albertoi Exp $
*
* DESCRIPTION
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*********************************************************************/

enum {
	MI_WCMD,
	MI_MENU,
	MI_SHELL,
	MI_PIPE,
	MI_DOFORM,
};
      
class MItem;
class MnItem;
class gtype(PtrArray, MItem);

class IxSMenu {

	String name_d;					// menu name
	String descr_d;					// menu description
	Int weight_d;					// maximum description item length
	gtype(PtrArray, MItem) &items_d;	// array of items

public:

	IxSMenu(const String &n = NULL_STRING, const String &d = NULL_STRING);
	~IxSMenu();

	const String &name();
	const String &descr();
		
	Int add(MItem *i);

	Int nItems();
	MItem *item(Int n);
		
	Int execute(String label);

	void store(FILE *f);
	void load(FILE *f);
};

class MItem {

protected:

	Int type_d;				// item type
	String descr_d;			// option description
	String unit_d;			// execution unit
	String args_d;			// args used by the unit
	IxSMenu *father_d;		// item dad
	String host_d;			// host where this item will be executed
			
public:

	MItem(IxSMenu *dad, const String &d, const String &u, const String &a, 
		  Int ty, const String &h);
	virtual ~MItem();

	virtual Int execute() = 0;

	virtual void store(FILE *fp);
	virtual void load(FILE *fp);
	
	virtual MnItem *menuItem();

	const Int &type();
	const String &descr();
	const String &unit();
	const String &args();
	
	static MItem *build(IxSMenu *dad, Int type, 
						const String &d = NULL_STRING, 
						const String &u = NULL_STRING, 
						const String &a = NULL_STRING,
						const String &h = NULL_STRING);
	static MItem *loadItem(FILE *fp, IxSMenu *menu);

	static void destHost(const String &h);
};

class WCmdItem : public MItem {
	
public:

	WCmdItem(IxSMenu *dad, const String &d, const String &u, 
			 const String &a, const String &h);
	~WCmdItem();
	
	Int execute();
};

class DoFormItem : public MItem {
	
public:

	DoFormItem(IxSMenu *dad, const String &d, const String &u, 
			   const String &a, const String &h);
	~DoFormItem();
	
	Int execute();
};

class MnItem : public MItem {

	IxSMenu *child_d;		// pointer to the child when the unit is a MENU

public:

	MnItem(IxSMenu *dad, const String &d, const String &u, 
		   const String &a, const String &h);
	~MnItem();

	void child(IxSMenu *menu);
	IxSMenu *child();

	MnItem *menuItem();

	Int execute();

	void store(FILE *fp);
	void load(FILE *fp);
};

class ShellItem : public MItem {

public:

	ShellItem(IxSMenu *dad, const String &d, const String &u, 
			  const String &a, const String &h);
	~ShellItem();

	Int execute();
};


class PipeItem : public MItem {

public:

	PipeItem(IxSMenu *dad, const String &d, const String &u, 
			 const String &a, const String &h);
	~PipeItem();

	Int execute();
};

String groupDir(const String &aDir, const String &grName);
String unitsFile(const String &storeDir);
String groupsFile(const String &storeDir);
String fullCMenuName(const String &storeDir, const String &unitName);
// default permission behavior for the groups
enum {
	HAVENOPERM,
	HAVEPERM,	
};

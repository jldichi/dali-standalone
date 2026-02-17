/*
 * $Id: fmactn.h,v 1.1.1.1 1996/04/16 18:51:40 gustavof Exp $
 */

#ifndef FMACTN_H
#define FMACTN_H

#ifdef __GNUG__                                                                 
#pragma interface                                                              
#endif

#include <ifound.h>
#include <ideafix/fmtype.h>

class FormRep;
class FmOption;
class FmFieldObj;

class FmAction {

protected:

	// The form this action belongs to
	FormRep *fm_;

	// This action's id
	Fm::Event id_;

	// This action's internal name
	String name_;
	// Action's title (for menu etc.)
	String title_;
	// This action's short description
	String descr_;

	// Whether this action should be ignored in these zones
	bool ignoreInKey_;
	bool ignoreInData_;
	
	// Another thing that could be here is the action's icon
	String bitmapName_, pushBitmapName_;

	// Whether this action should be confirmed
	bool confirm_;

	// The result of the user's code for this action
	Fm::Action result_;

protected:

	// default copy constructor should be OK
	FmAction(FormRep *theForm, Fm::Event id, const String &name,
			const String &bitmap);
			
	void desired(FmFieldObj *obj) const;
	void desiredKey() const;
	void desiredData() const;
	
public:

	virtual ~FmAction();

	// return action's id
	inline Fm::Event id() const;
	// alias of 'id()'
	inline operator int() const;

	// action's name
	inline const String &name() const;

	// action's title
	inline const String &title() const;
	inline void title(const String &newTitle);
	
	// action's description
	inline const String &descr() const;
	inline void descr(const String &newDescr);
	
	// bitmap's name
	inline const String &bitmapName() const;
	inline void bitmapName(const String &theName);
	inline const String &pushBitmapName() const;
	inline void pushBitmapName(const String &theName);
	
	// try to execute this action
	virtual bool preProcess();

	// process the action's result
	virtual bool processResult() const;

	// call user's code associated with this action
	virtual void callUser();

	// get & set action's result
	inline Fm::Action result() const;
	inline void result(Fm::Action res);

	// whether the action should be ignored
	bool ignored() const;

	// whether the action is confirmed
	inline void confirm(bool v);
	bool confirmed() const;

	inline void ignoreInKeyZone(bool v);
	inline void ignoreInDataZone(bool v);

	virtual FmAction *clone() const = 0;
	virtual FmAction *cloneBasic() const = 0;

	virtual bool addOption(const FmOption &option);  

	//  load and store methods
	virtual bool load(FILE *in);
	virtual void store(FILE *out);
};


class FmActionNull : public FmAction {

public:

	FmActionNull(FormRep *f);

	FmAction *clone() const;
	virtual FmAction *cloneBasic() const;
	
	bool preProcess();
};


class FmActionRead : public FmAction {

protected:

	FmActionRead(FormRep *form, Fm::Event id, const String &name,
			const String &bitmap);

public:

	FmActionRead(FormRep *form);

	FmAction *clone() const;
	virtual FmAction *cloneBasic() const;
	
	bool preProcess();
	bool processResult() const;
	void callUser();
};

class FmActionReadNext : public FmActionRead {
public:
	FmActionReadNext(FormRep *form);
	
	virtual FmAction *cloneBasic() const;
};

class FmActionReadPrev : public FmActionRead {
public:
	FmActionReadPrev(FormRep *form);
	
	virtual FmAction *cloneBasic() const;
};

class FmActionReadFirst : public FmActionRead {
public:
	FmActionReadFirst(FormRep *form);
	
	virtual FmAction *cloneBasic() const;
};

class FmActionReadLast : public FmActionRead {
public:
	FmActionReadLast(FormRep *form);
	
	virtual FmAction *cloneBasic() const;
};


class FmActionReset : public FmAction {

public:

	FmActionReset(FormRep *f);

	FmAction *clone() const;
	virtual FmAction *cloneBasic() const;
	
	bool preProcess();
	bool processResult() const;
};


class FmActionUpdate : public FmAction {

public:

	FmActionUpdate(FormRep *f);

	FmAction *clone() const;
	virtual FmAction *cloneBasic() const;
	
	bool preProcess();
	bool processResult() const;
};


class FmActionAdd : public FmAction {

public:

	FmActionAdd(FormRep *f);

	FmAction *clone() const;
	virtual FmAction *cloneBasic() const;
	
	bool preProcess();
	bool processResult() const;
};


class FmActionDelete : public FmAction {

public:

	FmActionDelete(FormRep *f);

	FmAction *clone() const;
	virtual FmAction *cloneBasic() const;
	
	bool preProcess();
	bool processResult() const;
};


class FmActionClose : public FmAction {

public:

	FmActionClose(FormRep *f);
	
	FmAction *clone() const;
	virtual FmAction *cloneBasic() const;
};


class FmActionUndo : public FmAction {

public:

	FmActionUndo(FormRep *f);
	
	FmAction *clone() const;
	virtual FmAction *cloneBasic() const;

	bool preProcess();
	bool processResult() const;
};


class FmActionSubform : public FmAction {

public:

	FmActionSubform(FormRep *f);
	
	FmAction *clone() const;
	virtual FmAction *cloneBasic() const;

	bool preProcess();
	bool processResult() const;
};

class FmActionClearField : public FmAction {

public:

	FmActionClearField(FormRep *f);
	
	FmAction *clone() const;
	virtual FmAction *cloneBasic() const;

	bool preProcess();
	bool processResult() const;
};


class FmActionUserDef : public FmAction {
	typedef FmAction inherited;
	
	// Whether a reset should be implied after executing this action
	bool reset_;

	// Whether all objects should be validated before executing this action
	bool validate_;

public:

	FmActionUserDef(FormRep *form, Fm::Event id, 
			const String &name = NULL_STRING);
			
	FmAction *clone() const;
	virtual FmAction *cloneBasic() const;
	
	bool preProcess();
	bool processResult() const;
	inline bool validateBeforeExec() const;
	inline bool resetAfterExec() const;

	virtual bool addOption(const FmOption &option);  

	//  load and store methods
	virtual bool load(FILE *in);                                
	virtual void store(FILE *out);                              
};

#include <ideafix/priv/fmactn.icc>

#endif



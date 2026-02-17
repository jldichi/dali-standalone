/*
 * $Id: fmsubf.h,v 1.1.1.1 1996/04/16 18:51:41 gustavof Exp $
 */

#ifndef FMSUBF_H
#define FMSUBF_H

#pragma interface

#include <ideafix/priv/fmrep.h>

class SubformData;
class gtype(Array, SubformIndex);

class FmSubform : public FmFieldObj {
	typedef FmFieldObj inherited;

	gtype(Array, SubformIndex)	&sfids_;

	SubformData *subformBuffer(FmBufferRow br) const;

    FormRep *selectSubform(const FmBufferRow br, SubformIndex index);
	SubformIndex currentSubformIndex(FmBufferRow br) const;

	// find subform by descr
	SubformIndex findSubform(const String &name);
	void addSubform(const String &name, FormRep *subform);
	void addSubform(SubformDescr *descr);

public:
	FmSubform();
	~FmSubform();
	
	FM_DECLARE_CLASS();

	virtual FieldType fType() const;
	
	virtual Int bufferSize() const;

	bool _isNull(FmBufferRow br) const;
	void _clear(FmBufferRow br);

	bool isNew(Int row) const;

	// display && hide subform window
	FormRep *currentSubform(Int row) const;
	SubformIndex currentSubformIndex(Int row) const;

	FormRep *useSubform(SubformIndex index, Int row);
	void openSubform();

	// add & query subforms
	// values must be already in list 'addInListValue'
	void addSubform(const String &name);
	void addSubform(FormRep *subform);
	SubformIndex numSubforms() const;
	const String &subformName(SubformIndex index) const;
	FormRep *subform(SubformIndex index) const;
};

#endif

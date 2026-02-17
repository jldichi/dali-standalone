/*
 * $Id: fmgroup.h,v 1.1.1.1 1996/04/16 18:51:41 gustavof Exp $
 */

#ifndef FMGROUP_H
#define FMGROUP_H

#include <ideafix/priv/fmobj.h>

#pragma interface

class FmObjGroup : public FmObjCollection {
	typedef FmObjCollection inherited;
	
public:
	FmObjGroup();

	FM_DECLARE_CLASS();
	
	virtual FieldType fType() const;
	virtual bool validate(Int row);
	bool validateUnique(Int row);
	bool isNullAll(FmBufferRow br);
};

#endif

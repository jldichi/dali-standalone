//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: index.h,v 1.9 2002/03/20 15:03:59 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef SQLFWINDEX_H
#define SQLFWINDEX_H

#include <sqldb/metadata/indexmd.h>
#include <sqldb/fwork/ifield.h>
#include <ifound/str.h>

class IndexFieldList;
class IndexArray;

/** 
 * This class is used to manipule table indexes. It certainly encapsulates
 * index metadata. When the SQLFWIndex object is created, the related Index
 * metadata is associated to the object if the index descriptor was used. If
 * the SQLFWIndex was created dynamically all necesary data is contained in the 
 * array of IndexFields. There are no set members because the index metadata 
 * has been loaded during Table load (table attachment) or during dynamic
 * construction. Remember that tables are loaded on demand and this is the
 * moment in which the index metadata associated to the table is loaded too.
 * Among the operations allowed, we find:
 *	- modification (remove flds, add flds).
 *	- modification of constraints.
 * 
 * @version
 * @author Diego Balbi & Eduardo Chiocconi.
 * @see IndexImpl
 */
class SQLFWIndex : public Object {

public:	
	// Constructors & Destructor.
	SQLFWIndex(const IndexMetadata& imd);	// used in getTablesTable...
	SQLFWIndex(const IndexFieldList &flds, Int opt);
	~SQLFWIndex();

	// Get Information methods.
	inline const IndexFieldArray &getIndexFlds() const;
	inline Int nFields() const;
	inline Int indexLen() const;
	inline Int indexFlags() const;
	inline const IndexField &getField(const String &name) const;
	inline const IndexField &getField(Int n) const;
	inline String indexName() const;
	inline dbindex indexDescriptor() const;
	inline bool isUnique() const;
	inline IndexMetadata *indexMetadata() const;
	inline bool isTemporary() const;
	bool fieldExists(const String &fldName) const;
	inline bool isPrimaryKey() const;

	// Debug
	void dump() const;
	String convertToString() const;
	Object *clone() const;

private:
	bool unique_d;
	IndexMetadata *indexMD_d;

	SQLFWIndex(const SQLFWIndex &);
	SQLFWIndex &operator=(const SQLFWIndex &);
};

#include <sqldb/fwork/index.icc>

#endif // SQLFWINDEX_H

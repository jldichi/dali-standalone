//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: bufftab.h,v 1.12 2002/03/19 22:27:51 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef BUFFTAB_H
#define	BUFFTAB_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>

class TableMetadata;
class TableArray;
class SQLFWSchema;

/**
 * This class is used to encapsulate the table metadata and table buffer
 * among other things. This is the structure that SchemaMetadata can access.
 * It encapsulates tab_array CFIX structure.
 * The creation is rather strange. The array of tab_array is created at 
 * schema level and we have to relate each one of the tab_array with a
 * BufferedTable. That is why the constructor of the class recives the
 * address of the tab_array to encapsulate.
 * 
 * @version
 * @author Eduardo Chiocconi.
 * @see TableMetadata
 * @see SchemaMetadata
 */
class BufferedTable {

public:
	BufferedTable(struct tab_array *ta, const SQLFWSchema *sc);
	~BufferedTable();

	// Set methods.
	inline void setTableDescriptor(dbtable t);
	void setBufferedTableName(const String &tname);
	bool setFieldBuff(Int fld, const char* buff, Int row=0);
	bool setFieldBuff2(Int fld, const char* buff, Int row=0);
	bool changeTabArrayPositionReference(struct tab_array *ta);
	bool setAliasTable(dbtable tab);
	bool replaceTableMetadata(TableMetadata *tmd);

	// Get methods.
	bool hasTableMDBeenLoaded() const;
	inline struct tab_array *buffTableRep() const;
	inline TableMetadata *tableMetadata() const;
	inline char *buffer() const;
	char* getFieldBuff(Int fld, Int row = 0) const;
	inline dbtable tableDescriptor() const;
	inline const String &getTableName() const;

	bool allocateBuffer(UShort len);

	// Debug
	void dump() const;

private:
	TableMetadata* tableMD_d;
	struct tab_array *tabArrayRep_d;
	String tabName_d;

	void replaceTableMD(const TableMetadata &tmd);
	void initializeTabArray();
};

#include <sqldb/metadata/bufftab.icc>

#endif	// BUFFTAB_H

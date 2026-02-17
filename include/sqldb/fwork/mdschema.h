/********************************************************************
* Copyright (c) 1998 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such soucone code.
*
* $Id: mdschema.h,v 1.4 2002/03/20 17:16:45 hardaiz Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef	METADATA_SCHEMA_H
#define	METADATA_SCHEMA_H

#include <ifound.h>
#include <ifound/str.h>
#include <ifound/itypes.h>
#include <sqldb/fwork/connect.h>

class UrlConnectionRelation;

class gtype(PtrArray, UrlConnectionRelation);

class UrlConnectionRelation {
public:
	UrlConnectionRelation(const String &url, SQLFWConnection *conn);
	~UrlConnectionRelation();

	inline const String &getUrl() const;
	inline const SQLFWConnection *getConnection() const;
	inline Int nUses() const;

	inline void setConnection(SQLFWConnection *conn);
	inline void invalidateConnection();
	inline void incUses();
	inline void decUses();

	void dump() const;

private:
	String url_d;
	SQLFWConnection *conn_d;
	Int nUses_d;
};


class MetadataSchemaConnections {
public:
	MetadataSchemaConnections();
	~MetadataSchemaConnections();
	
	void addConnection(UrlConnectionRelation *usc);
	bool exists(const String &url);
	UrlConnectionRelation *getUrlConnection(Int i) const;
	UrlConnectionRelation *getUrlConnection(String &url) const;
	Int nConnections() const;
	void clear();
	void dump() const;

private:
	gtype(PtrArray, UrlConnectionRelation) &connections_d;
	
};

#include <sqldb/fwork/mdschema.icc>

#endif	// METADATA_SCHEMA_H

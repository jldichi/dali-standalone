//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: openedsch.h,v 1.12 2002/03/19 22:28:37 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef OPENEDSCH_H
#define OPENEDSCH_H

#include <ideafix.h>
#include <ideafix/priv/dbdefs.h>
#include <ideafix/priv/gndefs.h>

#define CURR_SCH_SLOT	0

class SchemaMetadata;
class SQLFWSchema;

/**
 * This class implements an array containing the opened schemas.
 * Note that the position 0 contains the current schema. Thus, the schema at
 * that position is duplicated at some other position.
 *
 *			+---------+
 *		  0 |         |    Default
 *			+---------+
 *		  1 |         |
 *			+---------+
 *			    ...
 *			+---------+
 *			|         |  MAX_OPEN_SCH - 1
 *			+---------+
 *
 * It also contains a pointer to the schemaMD of 'metadata' schema.
 *
 * @version
 * @author Eduardo Chiocconi.
 * @see	SchemaMetadata
 */
class OpenedSchemas {

public:
	OpenedSchemas();
	~OpenedSchemas();

	static Int findSchSlot(char *server, const char *name, Int version);
	static schema findSchema(const char *name);
	static schema findSchemaVersion(char *name, Int version);
	static schema newSch(char *server, const char *name, Int version);
	static Int newSchSlot(char *server, const char *name, Int version);
	static void freeSch(schema sc);
	static SchemaMetadata *getSchMDPtr(schema sch);
	static schema setSchMDPtr(Int slot, SchemaMetadata *schptr);
	static schema findNextSchema(schema sch);
	static schema currentSchema(void);
	static Int slotsOccupied(void);
	static void freeUnusedSlots();
	static void freeSchSlot(Int slot);
	static bool existsSchemaMetadata(schema sch);
	static const SchemaMetadata& getMetadataOfSchemaMetadata(const SQLFWSchema &sc);
	static schema setCurrentSchema(schema newcurr);
	static bool destroyAllSchemas();
	static bool destroyMetadataSchema();

	// Debug
	static void dump();

private:
//	static SchemaMetadata **opened_schemas;
	static SchemaMetadata *metadata_d;	
};

#endif  // OPENEDSCH_H

//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: metasch.h,v 1.24 2002/03/19 22:28:24 hardaiz Exp $
// -----------------------------------------------------------------------

#ifndef __METASCH_H
#define __METASCH_H

#include <ifound.h>
#include <ifound/typeid.h>
#include <cfix.h>
#include <ideafix/priv/dbdefs.h>

/**
 * Structure used to store information about the tables in the metadata
 * schema.
 */
struct TableDescription {
	Int nTab;
	char *tabName;
	char *tabDescr;
};
 
/**
 * Structure used to store information about the fields of the metadata
 * tables.
 */
struct FieldDescription {
	Int nField;
	char *name;
	char *descr;
	type fType;
	UShort length;
	UShort dim;
	UShort offset;
	sqltype sqlType;
    field_flags flags;
	UChar ndec;
};

/**
 * Structure used to store information about the index (PK) fields of the 
 * metadata tables.
 */
struct IndexFieldDescription {
	Short nField;
	type fType;
	k_field_flags flags;
	UChar length;
	UShort offset;
	dbfield fld;
};

/**
 * Schema structures containig the tables.
 */
extern "C" TableDescription MetadataTables[];

/**
 * Table structures containing the fields.
 */
extern "C" FieldDescription SchemasTable[];
extern "C" FieldDescription TablesTable[];
extern "C" FieldDescription TabMembersTable[];
extern "C" FieldDescription IndicesTable[];
extern "C" FieldDescription IndexAttrsTable[];
extern "C" FieldDescription ReferencesTable[];
extern "C" FieldDescription ReferenceAttrsTable[];
extern "C" FieldDescription EnumsTable[];
extern "C" FieldDescription EnumsAttrsTable[];
extern "C" FieldDescription SchemaPermsTable[];
extern "C" FieldDescription TablePermsTable[];

/**
 * Index structures (Primary Key), containing index fields.
 */
extern "C" IndexFieldDescription SchemasPK[];
extern "C" IndexFieldDescription TablesPK[];
extern "C" IndexFieldDescription TablesIndex1[];
extern "C" IndexFieldDescription TabMembersPK[];
extern "C" IndexFieldDescription IndicesPK[];
extern "C" IndexFieldDescription IndexAttrsPK[];
extern "C" IndexFieldDescription ReferencesPK[];
extern "C" IndexFieldDescription ReferenceAttrsPK[];
extern "C" IndexFieldDescription EnumsPK[];
extern "C" IndexFieldDescription EnumsAttrsPK[];
extern "C" IndexFieldDescription SchemaPermsPK[];
extern "C" IndexFieldDescription TablePermsPK[];

/**
 * Miscellaneous defines.
 */
#define DBADMIN_UID			0
#define	LAST_FIELD			-1
#define MINALLOWEDSCH		0x010000

/**
 * Me assume the Metadata Schema is always placed in the last slot of the
 * opened schemas. This is a tautology because we always have to open this
 * schema to be able to load any other schema. The last valid position (slot)
 * in the array of opened schemas is "1".
 */
#define	METADATASCHEMASLOT	MAX_OPEN_SCH
#define	METADATASCHEMAID	(ITOSCH(METADATASCHEMASLOT))

/**
 * Index identifiers. (Primary Keys)
 */
#define SCHEMAS_PKID		(METADATASCHEMAID | 0x000100)
#define TABLES_PKID			(METADATASCHEMAID | 0x000200)
#define TABLES_INDEX1ID		(METADATASCHEMAID | 0x000201)
#define TABMEMBERS_PKID		(METADATASCHEMAID | 0x000300)
#define INDICES_PKID		(METADATASCHEMAID | 0x000400)
#define INDATTRS_PKID		(METADATASCHEMAID | 0x000500)
#define REFERENCES_PKID		(METADATASCHEMAID | 0x000600)
#define REFATTRS_PKID		(METADATASCHEMAID | 0x000700)
#define ENUMS_PKID			(METADATASCHEMAID | 0x000800)
#define ENUMSATTR_PKID		(METADATASCHEMAID | 0x000900)
#define SCHPERMS_PKID		(METADATASCHEMAID | 0x000A00)
#define TABPERMS_PKID		(METADATASCHEMAID | 0x000B00)
// IMPORTANT: this define must be updated every time a new table is added!!!
#define METADATA_NTABS		((TABPERMS_PKID & 0x00FF00) >> 8)

/**
 * Primary Key index length. (Number of fields in key).
 */
#define SCHEMAS_PK_LEN		1
#define TABLES_PK_LEN		2
#define TABLES_INDEX1_LEN	2
#define TABMEMBERS_PK_LEN	3
#define INDICES_PK_LEN		3
#define INDATTRS_PK_LEN		4
#define REFERENCES_PK_LEN	3
#define REFATTRS_PK_LEN		5
#define ENUMS_PK_LEN		2
#define ENUMSATTR_PK_LEN	3
#define SCHPERMS_PK_LEN		4
#define TABPERMS_PK_LEN		5

/**
 * Table identifiers.
 */
#define SCHEMASTABLEID		SCHEMAS_PKID
#define TABLESTABLEID		TABLES_PKID
#define TABMEMBERSTABLEID 	TABMEMBERS_PKID
#define INDICESTABLEID		INDICES_PKID
#define INDATTRSTABLEID		INDATTRS_PKID
#define REFERENCESTABLEID	REFERENCES_PKID
#define REFATTRSTABLEID		REFATTRS_PKID
#define ENUMSTABLEID		ENUMS_PKID
#define ENUMSATTRTABLEID	ENUMSATTR_PKID
#define SCHPERMSTABLEID	 	SCHPERMS_PKID
#define TABPERMSTABLEID		TABPERMS_PKID

/**
 * Table length. (Number of fields).
 */
#define SCHEMAS_TAB_LEN		4
#define TABLES_TAB_LEN		8
#define TABMEMBERS_TAB_LEN	19
#define INDICES_TAB_LEN		7
#define INDATTRS_TAB_LEN	9
#define REFERENCES_TAB_LEN	8
#define REFATTRS_TAB_LEN	7
#define ENUMS_TAB_LEN		3
#define ENUMSATTR_TAB_LEN	4
#define SCHPERMS_TAB_LEN	4
#define TABPERMS_TAB_LEN	5

/**
 * Number of indexes in each table.
 */
#define SCHEMAS_TAB_NINDS		1
#define TABLES_TAB_NINDS		2
#define TABMEMBERS_TAB_NINDS	1
#define INDICES_TAB_NINDS		1
#define INDATTRS_TAB_NINDS		1
#define REFERENCES_TAB_NINDS	1
#define REFATTRS_TAB_NINDS		1
#define ENUMS_TAB_NINDS			1
#define ENUMSATTR_TAB_NINDS		1
#define SCHPERMS_TAB_NINDS		1
#define TABPERMS_TAB_NINDS		1

/**
 * Schema Table fields.
 */
#define SCHEMAS_EXTENT		(METADATASCHEMAID | SCHEMASTABLEID | 0x000001)
#define	SCHEMAS_DESCRIP		(METADATASCHEMAID | SCHEMASTABLEID | 0x000002)
#define	SCHEMAS_OWNER		(METADATASCHEMAID | SCHEMASTABLEID | 0x000003)
#define	SCHEMAS_NTABS		(METADATASCHEMAID | SCHEMASTABLEID | 0x000004)

/**
 * Schema Table Index fields.
 */
#define	SCHEMA_INDFLD1		1

/**
 * Tables Table fields.
 */
#define	TABLES_SCHEMA		(METADATASCHEMAID | TABLESTABLEID | 0x000001)
#define	TABLES_EXTENT		(METADATASCHEMAID | TABLESTABLEID | 0x000002)
#define	TABLES_ORDER		(METADATASCHEMAID | TABLESTABLEID | 0x000003)
#define	TABLES_DESCRIP		(METADATASCHEMAID | TABLESTABLEID | 0x000004)
#define TABLES_NFLDS		(METADATASCHEMAID | TABLESTABLEID | 0x000005)
#define TABLES_NINDS		(METADATASCHEMAID | TABLESTABLEID | 0x000006)
#define TABLES_SIZE			(METADATASCHEMAID | TABLESTABLEID | 0x000007)
#define TABLES_UNVECTABLE	(METADATASCHEMAID | TABLESTABLEID | 0x000008)

/**
 * Tables Table Index fields.
 */
#define	TABLES_INDFLD1		1
#define	TABLES_INDFLD2		2

#define	TABLES_INDEX1_FLD1	1
#define	TABLES_INDEX1_FLD2	3

/**
 * Table member Table fields.
 */
#define	TABMEMBERS_SCHEMA	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000001)
#define	TABMEMBERS_EXTENT	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000002)
#define	TABMEMBERS_ID		(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000003)
#define	TABMEMBERS_NAME		(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000004)
#define	TABMEMBERS_DESCRIP	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000005)
#define	TABMEMBERS_DIM		(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000006)
#define	TABMEMBERS_LEN		(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000007)
#define	TABMEMBERS_SCALE	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000008)
#define	TABMEMBERS_TY		(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000009)
#define	TABMEMBERS_SQLTY	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x00000A)
#define	TABMEMBERS_TYNAME	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x00000B)
#define	TABMEMBERS_NOTNULL	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x00000C)
/* Expressions are stored in ASCII */
#define	TABMEMBERS_DEFLT	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x00000D)	
/* Expressions are stored in ASCII */
#define	TABMEMBERS_MASKATTR	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x00000E)
/* Expressions are stored in ASCII */
#define	TABMEMBERS_CHK		(METADATASCHEMAID | TABMEMBERSTABLEID | 0x00000F)
#define	TABMEMBERS_CHKDIGIT	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000010)
#define	TABMEMBERS_INPK		(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000011)
#define	TABMEMBERS_STAMPFLD	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000012)
#define	TABMEMBERS_NULLIS	(METADATASCHEMAID | TABMEMBERSTABLEID | 0x000013)
                                              
/**
 * Table Member Table Index fields.
 */
#define	TABMEMBERS_INDFLD1	1
#define	TABMEMBERS_INDFLD2	2
#define	TABMEMBERS_INDFLD3	3

/**
 * Indices Table fields.
 */
#define INDICES_SCHEMA		(METADATASCHEMAID | INDICESTABLEID | 0x000001)
#define	INDICES_EXTENT		(METADATASCHEMAID | INDICESTABLEID | 0x000002)
#define	INDICES_ID			(METADATASCHEMAID | INDICESTABLEID | 0x000003)
#define	INDICES_NAME		(METADATASCHEMAID | INDICESTABLEID | 0x000004)
#define	INDICES_NFLDS		(METADATASCHEMAID | INDICESTABLEID | 0x000005)
#define	INDICES_UNIQUEATTR	(METADATASCHEMAID | INDICESTABLEID | 0x000006)
#define	INDICES_HASUQPHYSINDEX (METADATASCHEMAID | INDICESTABLEID | 0x000007)

/**
 * Indices Table Index fields.
 */
#define INDICES_INDFLD1		1
#define INDICES_INDFLD2		2
#define INDICES_INDFLD3		3

/**
 * Index attribute Table fields.
 */
#define INDEXATTR_SCHEMA	(METADATASCHEMAID | INDATTRSTABLEID | 0x000001)
#define	INDEXATTR_EXTENT	(METADATASCHEMAID | INDATTRSTABLEID | 0x000002)
#define INDEXATTR_INDEXID	(METADATASCHEMAID | INDATTRSTABLEID | 0x000003)
#define	INDEXATTR_ID		(METADATASCHEMAID | INDATTRSTABLEID | 0x000004)
#define	INDEXATTR_NAME		(METADATASCHEMAID | INDATTRSTABLEID | 0x000005)
#define	INDEXATTR_ATTRID	(METADATASCHEMAID | INDATTRSTABLEID | 0x000006)
#define	INDEXATTR_FLAGS		(METADATASCHEMAID | INDATTRSTABLEID | 0x000007)
#define	INDEXATTR_OFFSET	(METADATASCHEMAID | INDATTRSTABLEID | 0x000008)
#define	INDEXATTR_LEN		(METADATASCHEMAID | INDATTRSTABLEID | 0x000009)

/**
 * Index Attribute Index fields.
 */
#define INDEXATTR_INDFLD1	1
#define INDEXATTR_INDFLD2	2
#define INDEXATTR_INDFLD3	3
#define INDEXATTR_INDFLD4	4

/**
 * References table fields.
 */
#define	REFERENCES_SCHEMA	(METADATASCHEMAID | REFERENCESTABLEID | 0x000001)
#define	REFERENCES_EXTENT	(METADATASCHEMAID | REFERENCESTABLEID | 0x000002)
#define	REFERENCES_FLDID	(METADATASCHEMAID | REFERENCESTABLEID | 0x000003)
#define	REFERENCES_TYPEIN	(METADATASCHEMAID | REFERENCESTABLEID | 0x000004)
#define	REFERENCES_TABNAME	(METADATASCHEMAID | REFERENCESTABLEID | 0x000005)
#define	REFERENCES_INDNAME	(METADATASCHEMAID | REFERENCESTABLEID | 0x000006)
#define	REFERENCES_ISNEG	(METADATASCHEMAID | REFERENCESTABLEID | 0x000007)
#define	REFERENCES_INDID	(METADATASCHEMAID | REFERENCESTABLEID | 0x000008)

/**
 * Reference Index fields.
 */
#define	REFERENCES_INDFLD1	1
#define	REFERENCES_INDFLD2	2
#define	REFERENCES_INDFLD3	3

/**
 * Reference fields Table fields.
 */
#define	REFATTRS_SCHEMA		(METADATASCHEMAID | REFATTRSTABLEID | 0x000001)
#define	REFATTRS_EXTENT		(METADATASCHEMAID | REFATTRSTABLEID | 0x000002)
#define	REFATTRS_FLDID		(METADATASCHEMAID | REFATTRSTABLEID | 0x000003)
#define	REFATTRS_IDMEMBER	(METADATASCHEMAID | REFATTRSTABLEID | 0x000004)
#define	REFATTRS_ISKEY		(METADATASCHEMAID | REFATTRSTABLEID | 0x000005)
#define	REFATTRS_ISCONST	(METADATASCHEMAID | REFATTRSTABLEID | 0x000006)
#define	REFATTRS_VALUE		(METADATASCHEMAID | REFATTRSTABLEID | 0x000007)

/**
 * Reference fields Index fields.
 */
#define	REFATTRS_INDFLD1	1
#define	REFATTRS_INDFLD2	2
#define	REFATTRS_INDFLD3	3
#define	REFATTRS_INDFLD4	4
#define	REFATTRS_INDFLD5	5

/**
 * Enum table fields.
 */
#define	ENUMS_SCHEMA		(METADATASCHEMAID | ENUMSTABLEID | 0x000001)
#define	ENUMS_NAME			(METADATASCHEMAID | ENUMSTABLEID | 0x000002)
#define	ENUMS_DESCRIP		(METADATASCHEMAID | ENUMSTABLEID | 0x000003)

/**
 * Enum Index fields.
 */
#define	ENUMS_INDFLD1		1
#define	ENUMS_INDFLD2		2

/**
 * Enum Attributes table fields.
 */
#define ENUMATTRS_SCHEMA	(METADATASCHEMAID | ENUMSATTRTABLEID | 0x000001)
#define	ENUMATTRS_NAME		(METADATASCHEMAID | ENUMSATTRTABLEID | 0x000002)
#define	ENUMATTRS_ID		(METADATASCHEMAID | ENUMSATTRTABLEID | 0x000003)
#define	ENUMATTRS_VALUE		(METADATASCHEMAID | ENUMSATTRTABLEID | 0x000004)
#define	ENUMATTRS_DESCR		(METADATASCHEMAID | ENUMSATTRTABLEID | 0x000005)

/**
 * Enum Attributes Index fields.
 */
#define	ENUMATTRS_INDFLD1	1
#define	ENUMATTRS_INDFLD2	2
#define	ENUMATTRS_INDFLD3	3

/**
 * Schema Permission table fields.
 */
#define	PERM_SCHEMA			(METADATASCHEMAID | SCHPERMSTABLEID | 0x000001)
#define	PERM_UORG			(METADATASCHEMAID | SCHPERMSTABLEID | 0x000002)
#define PERM_USRGRP			(METADATASCHEMAID | SCHPERMSTABLEID | 0x000003)
#define	PERM_PERM			(METADATASCHEMAID | SCHPERMSTABLEID | 0x000004)

/**
 * Schema Permission Index fields.
 */
#define	PERM_INDFLD1		1
#define	PERM_INDFLD2		2
#define	PERM_INDFLD3		3
#define	PERM_INDFLD4		4

/**
 * Table Permission table fields.
 */
#define	TABPERM_SCHEMA		(METADATASCHEMAID | TABPERMSTABLEID | 0x000001)
#define	TABPERM_EXTENT		(METADATASCHEMAID | TABPERMSTABLEID | 0x000002)
#define	TABPERM_UORG		(METADATASCHEMAID | TABPERMSTABLEID | 0x000003)
#define	TABPERM_USRGRP		(METADATASCHEMAID | TABPERMSTABLEID | 0x000004)
#define	TABPERM_PERM		(METADATASCHEMAID | TABPERMSTABLEID | 0x000005)

/**
 * Schema Permission Index fields.
 */
#define	TABPERM_INDFLD1		1
#define	TABPERM_INDFLD2		2
#define	TABPERM_INDFLD3		3
#define	TABPERM_INDFLD4		4
#define	TABPERM_INDFLD5		5

#endif  // METASCH

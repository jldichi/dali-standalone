//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: metadata.sc,v 1.6 1999/06/02 13:40:28 eduardoc Exp $
// -----------------------------------------------------------------------

/**
 * This is the schema metadata associated with a server. It controls and
 * stores the metadata of all schemas, tables, fields, indexes, permissions
 * and server managing stuff in this database.
 */
schema    metadata;

/**
 * This is the table to store schema metadata.
 */
table	schemas (
	/* Name of the schema.        								*/
	extent			char(16),
	/* Description of the schema. 							    */
	descrip			char(64),
	/* Owner of the schema.										*/
	owner			char(32),
	/* Tables in the schema. (optimization).					*/
	ntabs			int,
)
primary key(extent);

/**
 * This table stores the different metadata tables for the schemas
 * in the database.
 */
table	tables (
		/* Schema. 												*/
		sch			char(16),
        /* Name of the table.									*/
        extent  	char(16),
        /* Description of the table.							*/
        descrip 	char(64),
		/* Number of fields in the table.						*/
		nflds		int,
		/* Number of indices in the table.						*/
		ninds		int,
)
primary key(sch, extent);

/**
 * This table stores the metadata information (dictionary) related
 * to fields of the tables in the schemas of the database.
 */
table	tabmembers (
		/* Schema.												*/
		sch			char(16),
		/* Name of the table.									*/
        extent  	char(16),
        /* Id of the field.										*/
        id      	int,
        /* Name of the field.									*/
        name    	char(32),
        /* Description of the field.							*/
        descrip 	char(64),
        /* Dimension of the field. It may be a vector field).	*/
        dim     	int,
        /* Length of the field.									*/
        len     	int,
        /* Amount of decimals if the type is num.				*/
        scale   	int,
        /* Type of the field. (s_field.d.f_type)				*/
        ty      	int,
        /* SQL Type of the field. (s_field.d.f_sqltype)			*/
        sqlty      	int,
        /* Name of the type of the field. This is usually used
		   with enum fields.  									*/
        typename    char(32),
		/* Whether the field has "not null" attribute.			*/
        notnull 	bool,
        /* Default expresion for the field.						*/
        deflt   	char(128),
        /* Mask of the field.									*/
        maskattr    char(128),
		/* Expression that determines the field checking.		*/
        chk     	char(128),
        /* Check digit of the field.							*/
        chkdigit    char(1),
        /* Whether the field belongs to the primary key.		*/
        inpkey  	bool,
        /* Whether the field is a timestamp field.				*/
        stampfld    int,
)
primary key(sch, extent, id);

/**
 * this table is used to store the metadata of the indexes of the
 * tables in the database.
 */
table	indices (
		/* Schema.												*/
		sch			char(16),
        /* Name of the table to which the index belong.			*/
        extent  	char(16),
        /* Id of the index										*/
        id      	int,
        /* Name of the index.									*/
        name    	char(32),
        /* Amount of fields in the index.						*/
        nflds   	int,
        /* Whether the index is unique.							*/
        uniqueattr	bool,
)
primary key(sch, extent, id);

/**
 * This is the table that stores the metadata of the index fields.
 */
table	indexattr (
		/* Schema.												*/
		sch			char(16),
        /* Name of the table to which the index belongs.		*/
        extent  	char(16),
        /* Id of the index to which the field belongs.			*/
        indexid 	int,
        /* Id index field.										*/
        id      	int,
        /* Name of the field.									*/
		name    	char(32), 
        /* Id of the field inside the table.					*/
        attrid  	int,
    	/* flags of index field.								*/
		flags		int,
        /* Offset in a char field where the field starts. This
		   is only used with char fields.						*/
        offset  	int,
        /* Length of the field used in the table. It is only 
		   used for char fields.								*/
        len     	int,
        /* Whether the field is ascending of descending in the
		   index.											    */
        descend 	bool,
        /* Whether the field has a "not null" attribute on.
		   If "not null" is on and the field has a null value for
		   this field, there will be no entry for this record.	*/
        notnull 	bool,
)
primary key(sch, extent, indexid, id);

/**
 * Table that stores information about "in" associated with fields.
 * If the attribute is of the type "in values" the fields ttablename
 * and tindexname are null.
 */
table	references (
		/* Schema 												*/
		sch			char(16),
        /* Name of the table where "in" is placed.				*/
        extent  	char(16),
        /* Field of the table associated with the "in".			*/
        fldid		int,
        /* Type of in. (in table or in values)					*/
        ty      	int,
		/* Name of the table that is referenced by "in" clause.	*/
        ttablename  char(16),
        /* Name of the index used for the "in".					*/
        tindexname  char(32),
		/* Whether the reference is negated or not.				*/
		isneg		bool,
		/* Index descriptor. USed for performance.				*/
		indid		int,
)
primary key(sch, extent, fldid);

/**
 * Table that stores the metadata (information) about the fields in
 * the "in" attribute.
 * If the members are part of an "in value", the keys represent the
 * values before ":" symbol and the descriptions the values after
 * ":" symbol.
 */
table	refattrs (
		/* Schema.												*/
		sch			char(16),
        /* Name of the table to which the "in" belong.			*/
        extent  	char(16),
        /* Id of the field associated with the "in".			*/
        fldid		int,
        /* Id of the member of the "in".						*/
        id      	int,
        /* Whether the member is part of the key or descr.		*/
        iskey 		bool,
        /* Whether the field is a constant or a field.			*/
        isconst 	bool,
		/* Content of the key/description.						*/
        value   	char(64),
)
primary key(sch, extent, fldid, id);

/**
 * Table that stores information about "enums"  defined in a
 * schema.
 */
table	enums (
		/* Schema												*/
		sch	char(16),
        /* Name of the enum.									*/
        name    	char(32),
        /* Description of the enum								*/
        descrip 	char(64),
)
primary key(sch, name);

/**
 * Table that stores the attributes of the enums defined in a
 * schema.
 */
table	enumattrs (
		/* Schema												*/
		sch	char(16),
        /* Name of the enum to which the enum belong.			*/
        enumname	char(32),
        /* Id of the attribute.									*/
        id      	int,
        /* Value of the attribute.								*/
        value 		int,
        /* Description if the attribute.						*/
        descrip 	char(32),
)
primary key(sch, enumname, id);

/**
 * This table stores the information about schema permissions.
 */
table	schperms (
		/* Schema.												*/
        sch			char(16),
		/* Whether the permission is a user permission.			*/
		uorg		bool,
		/* User or group permission.							*/
		perm		int,
)
primary key(sch, uorg, perm);

/**
 * This table stores the information about table permissions.
 */
table	tabperms (
		/* Schema.												*/
        sch			char(16),
		/* Table.												*/
        tab			char(16),
		/* Whether the permission is a user permission.			*/
		uorg		bool,
		/* User or group permission.							*/
		perm		int,
)
primary key(sch, tab, uorg, perm);


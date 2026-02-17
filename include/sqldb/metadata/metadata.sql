connect metadata/metadata1

CREATE SCHEMA AUTHORIZATION metadata;

CREATE TABLE metadata.schemas (
	extent VARCHAR2 (16),
	descrip VARCHAR2 (64),
	owner VARCHAR2 (32),
	ntabs INTEGER,
	CONSTRAINT schemas_pk PRIMARY KEY (extent)
);

CREATE TABLE metadata.tables (
	sch VARCHAR2 (16),
	extent VARCHAR2 (16),
	ord INTEGER, 
	descrip VARCHAR2 (128),
	nflds INTEGER,
	ninds INTEGER,
	maxrecords INTEGER,
	unvectable INTEGER DEFAULT 0,
	CONSTRAINT tables_pk PRIMARY KEY (sch, extent),
	CONSTRAINT tables_sch_fk FOREIGN KEY (sch) REFERENCES schemas(extent)
);

CREATE TABLE metadata.tabmembers (
	sch VARCHAR2 (16), 
	extent VARCHAR2 (16),
	id INTEGER,
	name VARCHAR2 (32),
	descrip VARCHAR2 (64),
	dim INTEGER,
	len INTEGER,
	scale INTEGER,
	ty INTEGER,
	sqlty INTEGER,
	typename VARCHAR2 (32),
	notnull INTEGER,
	deflt VARCHAR2 (128),
	maskattr VARCHAR2 (128),
	chk VARCHAR2 (128),
	chkdigit VARCHAR2 (1),
	inpkey INTEGER,
	stampfld INTEGER,
	nullis VARCHAR(32),
	CONSTRAINT tabmembers_pk PRIMARY KEY (sch, extent, id),
	CONSTRAINT tabmembers_fk FOREIGN KEY (sch, extent) REFERENCES tables (sch, extent)
);

CREATE TABLE metadata.indices (
	sch VARCHAR2 (16),
	extent VARCHAR2 (16),
	id INTEGER,
	name VARCHAR2 (32),
	nflds INTEGER,
	uniqueattr INTEGER,
	hasuqphysind INTEGER,
	CONSTRAINT indices_pk PRIMARY KEY (sch, extent, id),
	CONSTRAINT indices_fk FOREIGN KEY (sch, extent) REFERENCES tables (sch, extent)
);

CREATE TABLE metadata.indexattr (
	sch VARCHAR2 (16),
	extent VARCHAR2 (16),
	indexid INTEGER,
	id INTEGER,
	name VARCHAR2 (32),
	attrid INTEGER,
	flags INTEGER,
	offset INTEGER,
	len INTEGER,
	CONSTRAINT indexattr_pk PRIMARY KEY (sch, extent, indexid, id),
	CONSTRAINT indexattr_fk FOREIGN KEY (sch, extent, indexid) REFERENCES indices (sch, extent, id)
);

CREATE TABLE metadata.references (
	sch VARCHAR2 (16),
	extent VARCHAR2 (16),
	fldid INTEGER,
	ty INTEGER,
	ttablename VARCHAR2 (16),
	tindexname VARCHAR2 (32),
	isneg INTEGER,
	indid INTEGER,
	CONSTRAINT references_pk PRIMARY KEY (sch, extent, fldid),
	CONSTRAINT references_fk FOREIGN KEY (sch, extent) REFERENCES tables (sch, extent)
);

CREATE TABLE metadata.refattrs (
	sch VARCHAR2 (16),
	extent VARCHAR2 (16),
	fldid INTEGER,
	id INTEGER,
	iskey INTEGER,
	isconst INTEGER,
	value VARCHAR2 (128),
	CONSTRAINT refattrs_pk PRIMARY KEY (sch, extent, fldid, id, iskey),
	CONSTRAINT refattrs_fk FOREIGN KEY (sch, extent, fldid) REFERENCES references (sch, extent, fldid)
);

CREATE TABLE metadata.enums (
	sch VARCHAR2 (16),
	name VARCHAR2 (32),
	descrip VARCHAR2 (64),
	CONSTRAINT enums_pk PRIMARY KEY (sch, name),
	CONSTRAINT enums_fk FOREIGN KEY (sch) REFERENCES schemas (extent)
);

CREATE TABLE metadata.enumattrs (
	sch VARCHAR2 (16),
	enumname VARCHAR2 (32),
	id INTEGER,
	value INTEGER,
	descrip VARCHAR2 (32),
	CONSTRAINT enumattrs_pk PRIMARY KEY (sch, enumname, id),
	CONSTRAINT enumattrs_fk FOREIGN KEY (sch, enumname) REFERENCES enums (sch, name)
);

CREATE TABLE metadata.schperms (
	sch VARCHAR2 (16),
	uorg INTEGER,
	usrgrp INTEGER,
	perm INTEGER,
	CONSTRAINT schperms_pk PRIMARY KEY (sch, uorg, usrgrp, perm),
	CONSTRAINT schperms_fk FOREIGN KEY (sch) REFERENCES schemas (extent)
);

CREATE TABLE metadata.tabperms (
	sch VARCHAR2 (16),
	extent VARCHAR2 (16),
	uorg INTEGER,
	usrgrp INTEGER,
	perm INTEGER,
	CONSTRAINT tabperms_pk PRIMARY KEY (sch, extent, uorg, usrgrp, perm),
	CONSTRAINT tabperms_fk FOREIGN KEY (sch, extent) REFERENCES tables (sch, extent)
);

GRANT SELECT ON metadata.schemas TO PUBLIC;
GRANT SELECT ON metadata.tables TO PUBLIC;
GRANT SELECT ON metadata.tabmembers TO PUBLIC;
GRANT SELECT ON metadata.indices TO PUBLIC;
GRANT SELECT ON metadata.indexattr TO PUBLIC;
GRANT SELECT ON metadata.references TO PUBLIC;
GRANT SELECT ON metadata.refattrs TO PUBLIC;
GRANT SELECT ON metadata.enums TO PUBLIC;
GRANT SELECT ON metadata.enumattrs TO PUBLIC;
GRANT SELECT ON metadata.schperms TO PUBLIC;
GRANT SELECT ON metadata.tabperms TO PUBLIC;

disconnect

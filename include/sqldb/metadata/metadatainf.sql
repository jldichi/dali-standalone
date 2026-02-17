CREATE DATABASE metadata WITH LOG;

CREATE TABLE metadata:schemas (
	extent  VARCHAR (16), 
	descrip  VARCHAR (64), 
	owner VARCHAR (32),  
	ntabs  INTEGER , 
	PRIMARY KEY (extent)
);

CREATE TABLE metadata:tables (
	sch  VARCHAR (16), 
	extent  VARCHAR (16), 
	ord INTEGER, 
	descrip  VARCHAR (128), 
	nflds  INTEGER , 
	ninds  INTEGER , 
	maxrecords INTEGER, 
	PRIMARY KEY (sch, ord)
);

CREATE TABLE metadata:tabmembers (
	sch  VARCHAR (16), 
	extent  VARCHAR (16), 
	id  INTEGER , 
	name  VARCHAR (32), 
	descrip  VARCHAR (64), 
	dim  INTEGER , 
	len  INTEGER , 
	scale  INTEGER , 
	ty  INTEGER , 
	sqlty  INTEGER , 
	typename  VARCHAR (32), 
	notnull  INTEGER , 
	deflt  VARCHAR (128), 
	maskattr  VARCHAR (128), 
	chk  VARCHAR (128), 
	chkdigit  VARCHAR (1), 
	inpkey  INTEGER , 
	stampfld  INTEGER , 
	nullis VARCHAR (32), 
	PRIMARY KEY (sch, extent, id, name)
);

CREATE TABLE metadata:indices (
	sch  VARCHAR (16), 
	extent  VARCHAR (16), 
	id  INTEGER , 
	name  VARCHAR (32), 
	nflds  INTEGER , 
	uniqueattr  INTEGER , 
	hasuqphysind INTEGER,
	PRIMARY KEY (sch, extent, id)
);

CREATE TABLE metadata:indexattr (
	sch  VARCHAR (16), 
	extent  VARCHAR (16), 
	indexid  INTEGER , 
	id  INTEGER , 
	name  VARCHAR (32), 
	attrid  INTEGER , 
	flags  INTEGER , 
	offset  INTEGER , 
	len  INTEGER , 
	PRIMARY KEY (sch, extent, indexid, id)
);

CREATE TABLE metadata:references (
	sch  VARCHAR (16), 
	extent  VARCHAR (16), 
	fldid  INTEGER , 
	ty  INTEGER , 
	ttablename  VARCHAR (16), 
	tindexname  VARCHAR (32), 
	isneg  INTEGER, 
	indid  INTEGER, 
	PRIMARY KEY (sch, extent, fldid)
);

CREATE TABLE metadata:refattrs (
	sch  VARCHAR (16), 
	extent  VARCHAR (16), 
	fldid  INTEGER , 
	id  INTEGER , 
	iskey  INTEGER , 
	isconst  INTEGER , 
	value  VARCHAR (128), 
	PRIMARY KEY (sch, extent, fldid, id, isKey)
);

CREATE TABLE metadata:enums (
	sch  VARCHAR (16), 
	name  VARCHAR (32), 
	descrip  VARCHAR (64), 
	PRIMARY KEY (sch, name)
);

CREATE TABLE metadata:enumattrs (
	sch  VARCHAR (16), 
	enumname  VARCHAR (32), 
	id  INTEGER , 
	value  INTEGER , 
	descrip  VARCHAR (32), 
	PRIMARY KEY (sch, enumname, id)
);

CREATE TABLE metadata:schperms (
	sch  VARCHAR (16), 
	uorg  INTEGER , 
	usrgrp INTEGER, 
	perm  INTEGER , 
	PRIMARY KEY (sch, uorg, usrgrp, perm)
);

CREATE TABLE metadata:tabperms (
	sch  VARCHAR (16), 
	extent  VARCHAR (16), 
	uorg  INTEGER , 
	usrgrp INTEGER, 
	perm  INTEGER , 
	PRIMARY KEY (sch, extent, uorg, usrgrp, perm)
);

GRANT CONNECT TO PUBLIC;
GRANT SELECT ON metadata:schemas TO PUBLIC;
GRANT SELECT ON metadata:tables TO PUBLIC;
GRANT SELECT ON metadata:tabmembers TO PUBLIC;
GRANT SELECT ON metadata:indices TO PUBLIC;
GRANT SELECT ON metadata:indexattr TO PUBLIC;
GRANT SELECT ON metadata:references TO PUBLIC;
GRANT SELECT ON metadata:refattrs TO PUBLIC;
GRANT SELECT ON metadata:enums TO PUBLIC;
GRANT SELECT ON metadata:enumattrs TO PUBLIC;
GRANT SELECT ON metadata:schperms TO PUBLIC;
GRANT SELECT ON metadata:tabperms TO PUBLIC;

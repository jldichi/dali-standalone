CREATE DATABASE metadata WITH LOG;

GRANT DBA TO "informix";
GRANT CONNECT TO "public";

CREATE TABLE schemas 
  (
    extent VARCHAR(16),
    descrip VARCHAR(64),
    owner VARCHAR(32),
    ntabs INTEGER
  ) EXTENT SIZE 16 NEXT SIZE 16 LOCK MODE PAGE;
REVOKE ALL ON schemas FROM "public";

CREATE UNIQUE INDEX ix_schemas ON schemas(extent);
ALTER TABLE schemas ADD CONSTRAINT PRIMARY KEY (extent);

CREATE TABLE tables 
  (
    sch VARCHAR(16),
    extent VARCHAR(16),
    ord INTEGER,
    descrip VARCHAR(128),
    nflds INTEGER,
    ninds INTEGER,
    maxrecords INTEGER,
	unvectable INTEGER DEFAULT 0,
  ) EXTENT SIZE 16 NEXT SIZE 16 LOCK MODE PAGE;
REVOKE ALL ON tables FROM "public";

CREATE UNIQUE INDEX ix_tables ON tables(sch,ord);
ALTER TABLE tables ADD CONSTRAINT PRIMARY KEY (sch,ord);
    

CREATE TABLE tabmembers 
  (
    sch VARCHAR(16),
    extent VARCHAR(16),
    id INTEGER,
    name VARCHAR(32),
    descrip VARCHAR(64),
    dim INTEGER,
    len INTEGER,
    scale INTEGER,
    ty INTEGER,
    sqlty INTEGER,
    typename VARCHAR(32),
    notnull INTEGER,
    deflt VARCHAR(128),
    maskattr VARCHAR(128),
    chk VARCHAR(128),
    chkdigit VARCHAR(1),
    inpkey INTEGER,
    stampfld INTEGER,
    nullis VARCHAR(32)
  ) EXTENT SIZE 16 NEXT SIZE 16 LOCK MODE PAGE;
REVOKE ALL ON tabmembers FROM "public";

CREATE UNIQUE INDEX "informix".ix_tabmembers ON tabmembers 
    (sch,extent,id);
	ALTER TABLE tabmembers ADD CONSTRAINT PRIMARY KEY (sch, extent,id);

CREATE TABLE indices 
  (
    sch VARCHAR(16),
    extent VARCHAR(16),
    id INTEGER,
    name VARCHAR(32),
    nflds INTEGER,
    uniqueattr INTEGER,
    hasuqphysind INTEGER
  ) EXTENT SIZE 16 NEXT SIZE 16 LOCK MODE PAGE;
REVOKE ALL ON indices FROM "public";

CREATE UNIQUE INDEX ix_indices ON indices(sch,extent,id);
ALTER TABLE indices ADD CONSTRAINT PRIMARY KEY (sch,extent,id);

CREATE TABLE indexattr 
  (
    sch VARCHAR(16),
    extent VARCHAR(16),
    indexid INTEGER,
    id INTEGER,
    name VARCHAR(32),
    attrid INTEGER,
    flags INTEGER,
    offset INTEGER,
    len INTEGER
  ) EXTENT SIZE 16 NEXT SIZE 16 LOCK MODE PAGE;
REVOKE ALL ON indexattr FROM "public";

CREATE UNIQUE INDEX ix_indexattr ON indexattr(sch,extent,indexid,id);
ALTER TABLE indexattr ADD CONSTRAINT PRIMARY KEY (sch,extent,indexid,id);


CREATE TABLE references 
  (
    sch VARCHAR(16),
    extent VARCHAR(16),
    fldid INTEGER,
    ty INTEGER,
    ttablename VARCHAR(16),
    tindexname VARCHAR(32),
    isneg INTEGER,
    indid INTEGER
  ) EXTENT SIZE 16 NEXT SIZE 16 LOCK MODE PAGE;
REVOKE ALL ON references FROM "public";

CREATE UNIQUE INDEX ix_references ON references(sch,extent,fldid);
ALTER TABLE references ADD CONSTRAINT PRIMARY KEY (sch,extent,fldid);

CREATE TABLE refattrs 
  (
    sch VARCHAR(16),
    extent VARCHAR(16),
    fldid INTEGER,
    id INTEGER,
    iskey INTEGER,
    isconst INTEGER,
    value VARCHAR(128)
  ) EXTENT SIZE 16 NEXT SIZE 16 LOCK MODE PAGE;
REVOKE ALL ON refattrs FROM "public";

CREATE UNIQUE INDEX ix_refattrs ON refattrs(sch,extent,fldid,id,iskey);
ALTER TABLE refattrs ADD CONSTRAINT PRIMARY KEY (sch,extent,fldid,id,iskey);

CREATE TABLE enums 
  (
    sch VARCHAR(16),
    name VARCHAR(32),
    descrip VARCHAR(64)
  ) EXTENT SIZE 16 NEXT SIZE 16 LOCK MODE PAGE;
REVOKE ALL ON enums FROM "public";

CREATE UNIQUE INDEX ix_enums ON enums(sch,name);
ALTER TABLE enums ADD CONSTRAINT PRIMARY KEY (sch,name);

CREATE TABLE enumattrs 
  (
    sch VARCHAR(16),
    enumname VARCHAR(32),
    id INTEGER,
    value INTEGER,
    descrip VARCHAR(32)
  ) EXTENT SIZE 16 NEXT SIZE 16 LOCK MODE PAGE;
REVOKE ALL ON enumattrs FROM "public";

CREATE UNIQUE INDEX ix_enumattrs ON enumattrs(sch,enumname,id);
ALTER TABLE enumattrs ADD CONSTRAINT PRIMARY KEY (sch,enumname,id);

CREATE TABLE schperms 
  (
    sch VARCHAR(16),
    uorg INTEGER,
    usrgrp INTEGER,
    perm INTEGER
  ) EXTENT SIZE 16 NEXT SIZE 16 LOCK MODE PAGE;
REVOKE ALL ON schperms FROM "public";

CREATE UNIQUE INDEX ix_schperms ON schperms(sch,uorg,usrgrp,perm);
ALTER TABLE schperms ADD CONSTRAINT PRIMARY KEY (sch,uorg,usrgrp,perm);

CREATE TABLE tabperms 
  (
    sch VARCHAR(16),
    extent VARCHAR(16),
    uorg INTEGER,
    usrgrp INTEGER,
    perm INTEGER
  ) EXTENT SIZE 16 NEXT SIZE 16 LOCK MODE PAGE;
REVOKE ALL ON tabperms FROM "public";

CREATE UNIQUE INDEX ix_tabperms ON tabperms(sch,extent,uorg,usrgrp,perm);
ALTER TABLE tabperms ADD CONSTRAINT PRIMARY KEY (sch,extent,uorg,usrgrp,perm);



GRANT SELECT ON schemas TO "public" ;
GRANT UPDATE ON schemas TO "public" ;
GRANT INSERT ON schemas TO "public" ;
GRANT DELETE ON schemas TO "public" ;
GRANT INDEX ON schemas TO "public" ;
GRANT SELECT ON tables TO "public" ;
GRANT UPDATE ON tables TO "public" ;
GRANT INSERT ON tables TO "public" ;
GRANT DELETE ON tables TO "public" ;
GRANT INDEX ON tables TO "public" ;
GRANT SELECT ON tabmembers TO "public" ;
GRANT UPDATE ON tabmembers TO "public" ;
GRANT INSERT ON tabmembers TO "public" ;
GRANT DELETE ON tabmembers TO "public" ;
GRANT INDEX ON tabmembers TO "public" ;
GRANT SELECT ON indices TO "public" ;
GRANT UPDATE ON indices TO "public" ;
GRANT INSERT ON indices TO "public" ;
GRANT DELETE ON indices TO "public" ;
GRANT INDEX ON indices TO "public" ;
GRANT SELECT ON indexattr TO "public" ;
GRANT UPDATE ON indexattr TO "public" ;
GRANT INSERT ON indexattr TO "public" ;
GRANT DELETE ON indexattr TO "public" ;
GRANT INDEX ON indexattr TO "public" ;
GRANT SELECT ON references TO "public" ;
GRANT UPDATE ON references TO "public" ;
GRANT INSERT ON references TO "public" ;
GRANT DELETE ON references TO "public" ;
GRANT INDEX ON references TO "public" ;
GRANT SELECT ON refattrs TO "public" ;
GRANT UPDATE ON refattrs TO "public" ;
GRANT INSERT ON refattrs TO "public" ;
GRANT DELETE ON refattrs TO "public" ;
GRANT INDEX ON refattrs TO "public" ;
GRANT SELECT ON enums TO "public" ;
GRANT UPDATE ON enums TO "public" ;
GRANT INSERT ON enums TO "public" ;
GRANT DELETE ON enums TO "public" ;
GRANT INDEX ON enums TO "public" ;
GRANT SELECT ON enumattrs TO "public" ;
GRANT UPDATE ON enumattrs TO "public" ;
GRANT INSERT ON enumattrs TO "public" ;
GRANT DELETE ON enumattrs TO "public" ;
GRANT INDEX ON enumattrs TO "public" ;
GRANT SELECT ON schperms TO "public" ;
GRANT UPDATE ON schperms TO "public" ;
GRANT INSERT ON schperms TO "public" ;
GRANT DELETE ON schperms TO "public" ;
GRANT INDEX ON schperms TO "public" ;
GRANT SELECT ON tabperms TO "public" ;
GRANT UPDATE ON tabperms TO "public" ;
GRANT INSERT ON tabperms TO "public" ;
GRANT DELETE ON tabperms TO "public" ;
GRANT INDEX ON tabperms TO "public" ;


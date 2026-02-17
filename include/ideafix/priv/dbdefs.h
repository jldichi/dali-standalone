/*********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dbdefs.h,v 1.24 2008/05/20 21:34:30 hardaiz Exp $
*
* DESCRIPTION
*
* Definitions header for IO routines.
*
* The header is the same for the Ideafix Data Base
* and for the Essentia Data Base.
*
* Fields used only for one of the interfaces are marked as:
*
*      IXDBMS : For the Ideafix DBMS
*      ESDBMS : For the Essentia DBMS
*
*********************************************************************/

#ifndef DBDEFS_H
#define DBDEFS_H

#ifndef IDEAFIX_H
#include <ideafix.h>
#endif

#include <ideafix/priv/gndefs.h>

#if HAVE_PRAGMA_PACK
#	pragma pack(4)
#endif

/* +++ CONFIG +++ */

#ifdef HAVE_SHORT_FILE_NAMES
#	define NAMELEN	  ((SizeT)8)   /* Max Length of Schema and Table names*/
#	define OLDNAMELEN ((SizeT)8)   /* TRANSITORIAMENTE Hasta una reconv. de datos*/
#else
#	define NAMELEN	  ((SizeT)10)  /* Max Length of Schema and Table names*/
#	define OLDNAMELEN ((SizeT)14)  /* TRANSITORIAMENTE Hasta una reconv. de datos*/
#endif

#define MAX_FILENAME	1024 /* Maximun complete filename			*/
#if defined (ACADEMIC_VERSION)
#define MAX_OPEN_SCH	(2+1)   /* Max number of opened schemas	add 1*/
#else
//#define MAX_OPEN_SCH	(13+1)   /* Max number of opened schemas	add 1*/

// No se sabe por que en algun momento se bajo de 16 a 13
// Creemos que alguien se equivoco haciendo una prueba con otras cosas
// de este fuente (aprox anio 1999).  H Ardaiz - Mayo 2008.
// Antes de cerrar Ideafix 5.2.1 lo volvemos a 16.
#define MAX_OPEN_SCH	(16+1)   /* Max number of opened schemas	add 1*/
#endif

#define	ACADEMIC_MAX_RECS	1024
#define	ACADEMIC_MAX_TABLES	32
#define CLUSTER			4	 /* Min num of tabs allocated in each req */

#define EXTLEN			4	 /* Max Extension name					*/

#define VALTAB_SIZE		5000 /* Original value table size			*/

#define CURSOR_BLK_SIZE	1024 /* Cache buffer size for Cursor		*/


/*	this number can be tuned using the env. variable NFILES,
	SET NFILES=NN:MM, where NN is the old NFILES and MM is the 
	maximum number of vfiles */

#define MAX_VFILES		100	 /* Max No of virtual files opened (all) */
#define DEF_NFILES		12	 /* Default Max Number of used Unix files*/

#ifndef HAVE_UMASK
#define CREAT_MODE		(S_IWRITE | S_IREAD)
#else
#define	CREAT_MODE		0777 /* Default creation mode				 */
#endif

#define	FRAME		1024	/* Minimun size of a index node, etc */
#define TABHEADER_SIZE	512	/* Table header size		     */
#define	MAX_KEY_LENGTH	256	/* Maximun length of a key	     */
/* Define added for compatibility with version 432. */
#define MAX_ARG_LENGTH  128	/* Maximun length of an arg to _CheckRecord */


#if HAVE_LOCK_CHAIN
#define LOCK_NODE_SIZE  5
         /* Number of locks per node in lock list*/
#endif

/* --- CONFIG --- */

#define ISVIRTUALFLD(f)		((f)&(F_CREAT_STAMP|F_MODIF_STAMP|F_PCREAT_STAMP))
#define HASSTAMPFIELDS(f)	((f)&(TAB_CREAT_STAMP|TAB_MODIF_STAMP|TAB_PCREAT_STAMP))

#define TBLOCK(t)		\
	((t).tblock        == NULL  ? NULL       : \
	 (t).tblock->d.len != 0     ? (t).tblock : _AttachTable((t).tblock))
#define RTBLOCK(t)	((t).tblock)

/* Constants */

#define CHKSUM_MAGIC 	1505
#if defined(ACADEMIC_VERSION)
#define	SCO_MAGIC		0603
#else
#define SCO_MAGIC 		0612
#endif
#define TAB_EXT			"tab"
#define IND_EXT			"i%02d"
#define SCHEMA_SUFFIX	"SCHEMA_SUFFIX"

/* BY_RECNO solo para uso interno! */
#define BY_RECNO	 	(find_mode) 0

/* Contantes usadas para los fields especiales */

/* These two defines have been added for compatibility with Version 432. */
#define SELF_NAME               "self"
#define SELF_DESCR              "Self"
#define OID_NAME		"oid"
#define OID_DESCR		"Object Id"
#define CTIME 			"ctime"
#define MTIME 			"mtime"
#define CDATE 			"cdate"
#define MDATE 			"mdate"
#define CUID  			"cuid"
#define MUID  			"muid"
#define LENUID				5	/* longitud del create/modif. uid. field*/

/* defines para la llamada ReadRootNode */
#define ROOT_NODE_NOT_LOCK	0
#define ROOT_NODE_LOCK  	1	
#define ROOT_NODE_TMP_IND 	2
#define ROOT_NODE_CHECK		3
#define ROOT_NODE_TEST		4

STRUCT DbConnection;
STRUCT LocalConnection;
STRUCT AnyValue;
STRUCT DbExpr;
STRUCT Privilege;
STRUCT IxField;

#define IDEA_POPUP_SCROLL	"IDEA_POPUP_SCROLL"

/* Typedefs for object id and record prefix types */

typedef struct {
	ULong cid;
	ULong iid;
} object_id;

typedef object_id DBOBJECT;

typedef struct {
	object_id oid;
	time_stamp cstamp;
	time_stamp mstamp;	
} record_prefix;

/* This structure holds access privileges for users			*/
struct dbprivg {
	unsigned long	id:16,		/* A group or user id.		*/
					id_type:2,	/* Is group or user id		*/
					privg:14;	/* Bits with privileges		*/
};

/* key field data. stored in .sco file */
struct s_index_field {
	itype			f_itype;
	k_field_flags	flags;
	UChar			len;
	UShort 			offset;
	dbfield			fld;
};

#if HAVE_LOCK_CHAIN
/* Typedef for a node for lock list */

typedef struct LOCKTABLE {
  long lockpos[LOCK_NODE_SIZE];
  struct LOCKTABLE * next;
} LOCKTABLE;

#endif
/* Typedef for a virtual file descriptor */

struct s_schema;
typedef struct s_vfile {
	struct	s_schema *sp;
	char	name[NAMELEN];
	char	ext[EXTLEN];
	int		mode;
	int		fd;
	short	nlocks;
	struct  s_vfile	*next,
					*prev;
#if HAVE_LOCK_CHAIN
	short lockwhole;        /* Flag of whole table lock */
	LOCKTABLE * locktable;  /* Pointer to first table */
#endif
} *vfile;

/* header record for a .tab or .ndx file */
struct header {
	long 		last;		/* Last record						*/
	long		pool;		/* First record in the pool			*/
	long		nreg;		/* Number of used records			*/
};

/****** Field and keys data structures *******/

#ifdef HAVE_PRAGMA_ALIGN
#	pragma SC align 4
#endif

struct s_field { 		/* field description stored in .sco file */
	struct fld_dict {
		value_type  		name;
		value_type  		descr;
		UShort				dim;	
		UShort 				offset;
		type 				f_type;
		itype  				f_itype;
		sqltype				f_sqltype;
		field_flags			flags;
		UShort      		len;
		UChar       		ndec;
		value_type  		mask;	/* mask expression's store */
		value_type  		relcheck;
		value_type			incheck;
		value_type			deflt;
	} d;
	char *cmask;
	STRUCT DbExpr *e;
	STRUCT DbExpr *deflt;
	STRUCT InCheck *inchk;
	char *collset;
	STRUCT IxField *fld;
};

struct cache {
	char *node;
	UShort n;
	UShort changed;
}; 

struct s_table;

struct s_index {

	struct k_dict { 	/* key dictionary. stored in .sco file */
		value_type		name;
		key_flags		flags;
		UShort 			len;
		UShort			n_fields;
		UShort			nodelen;		/* IXDBMS */
	} d;

	struct s_table *tp;
	struct s_index_field *f;

 	long objectId;						/* ESDBMS */

	union s_index_dbms {
		STRUCT DbConnection *con;		/* ESDBMS */
		struct s_index_ix {						/* IXDBMS */
			struct header		h;
			vfile				fd;
			bool				lock;
			bool				node_changed;
			char  				*root, 
								*node;
			struct tab_array 	*last_tap;
			long				max, 
								mtime;
			UShort				root_slot,
								leaf_slot,
								leaf_length,
								curr_node;
			struct cache *cache;
			UShort cache_size;
		} *ix;		

		struct s_index_sql {
			void *index;
			void *aliases;
		} *sql;
	} u;
};

/**** Table data structures ****/

struct s_dbcur;

struct s_table {

	struct t_dict { 	/* Table dictionary. Stored in .sco file */
		char		name[OLDNAMELEN];
		value_type	descr;
		tab_flags	flags;
		UChar		n_fields;
		UChar		n_keys;
		UShort		len;
		long		size;
		value_type	privg;
		UShort		n_privg;
#if IDEA_VERSION > 431
		value_type  relcheck;
#endif		
	} d;

	struct s_schema *sp;
	struct s_field	*f;
	struct s_index	*k;
	struct s_dbcur	*c;
	short			mode;
	UChar			n_alloc_fields;		/* No of allocated fields	*/
	UChar			n_alloc_keys;		/* No of allocated keys	*/
	UChar			n_alloc_cursors;	/* No of allocated cursors	*/
	UShort			act_privg;			/* Active privileges for user */
	STRUCT DbExpr   *e;					/* Table constraint */
	UShort			rel_depth;			/* Depth and Error flag for
									 	 * Set Relation (one byte for each
									 	 * one, respectively)
									 	 */
	union s_table_dbms {

		struct s_table_es {						/* ESDBMS 							*/
			long  objectId;				/* identificador de la tabla 		*/
			STRUCT DbConnection *con;	/* conexion con el server			*/
			ULong classId;				/* class id							*/
			long   cbufflen;				/* Longitud del buffer de la con	*/
		} es;

		struct s_table_ix {                            /* IXDBMS */
			struct header	h;
			vfile			fd;
			bool			lock;		/* The table header is locked*/
			bool			locktable;	/* Flag of table locked */
		} ix;

		struct s_table_sql {                            /* SQLDBMS */
			void *table;
			void *aliases;
		} sql;

	} u;
};

/* Element of Active Tables Array */

struct tab_array {
	char			*recbuff;
	struct s_table	*tblock;
	dbtable			alias_ot;	/* If an alias point to the org table */
	char			*stack;		/* Record stack */
	long			arecs;		/* Room for this many records w/out realloc */
	long			nrecs;		/* Current number of recs */
	
	union tab_array_dbms {

		struct tab_array_ix {                	/* IXDBMS */
			struct t_cache_ix {
				char *recbuff;
				long recno;
			} *cache;
			UShort	cache_size,
					cache_blocked;
		} ix;
		
		struct tab_array_es {					/* ESDBMS */
			struct t_cache_es {
				long cache_size;
				char *cachebuff;
				char *currpos;
				bool full;
			} *cache;
		} es;
	} u;					
};

/**** Schema data structures ****/

struct s_schema {

	struct s_dict {
	
		short magic;			/* IXDBMS : sco magic number				*/
		short nopen;			/* IXDBMS : no of process using this schema */
		short cpu_magic;		/* IXDBMS : Magic to define the kind of CPU */
		char ident[28];			/* IXDBMS : SCCS identification string		*/
		char name[OLDNAMELEN];	/* schema name								*/
		value_type descr;		/* description string						*/
		UChar	   n_tables;	/* current number of tables 				*/
		long       valtab_len;	/* IXDBMS : value table lenght				*/
		UShort     valtab_min;	/* IXDBMS : min. value table lenght			*/
		value_type privg;		/* lista con users, groups y public			*/
		UShort  n_privg;		/* cantidad de estructuras					*/
	
	} d;                                                                    
	

	struct tab_array *t;        /* table array pointer						*/
	short  mode;              	/* openschema mode							*/
	UChar  n_alloc_tables;		/* No of allocated tables slots				*/
	UShort owner;				/* Uid of schema owner						*/
	UShort act_privg;			/* Active privileges for user				*/
	
	union s_schema_dbms {
	
		struct s_schema_es {						/* ESDBMS						*/
			char  *server;				/* server of this schema  		*/
			long  objectId;				/* schema identification		*/
			STRUCT DbConnection *con;	/* server connection			*/
			long  version;				/* schema version				*/
		} es;
		
		struct s_schema_ix {						/* IXDBMS						*/
			char   *dbase;				/* dbase of this schema			*/
			char   *value_table;        /* value table pointer      	*/
			Int valtab_size;         /* Size to allocate val.tab 	*/
			vfile  fd;                	/* .sco file descriptor     	*/
			IFPL   write_schema;		/* Function to write the schema	*/
			bool   lock;     			/* locking flag             	*/
			long    sem_id;				/* Semaphore id. of this schema */
		} ix;

		struct s_schema_sql {
			void *schema;
		} sql;
	} u;
};

/**** Cursor data structures ****/

struct s_dbcur {

	dbindex ind;		/* index											*/
	long  lmode;		/* lock mode										*/
	short fnrec;		/* there are fnrecs forwarded records in cache		*/
	short bnrec;		/* there are bnrecs backwarded records in cache		*/
	bool morefrecs;		/* are there more forwarded records ?				*/
	bool morebrecs;		/* are there more backwarded records ?				*/
	long flags;			/* range flags										*/
	long wkflags;		/* work flags 										*/
	char *cache;		/* cache buffer										*/
	char *cptr;			/* cache pointer									*/
	char *rangefrom;    /* from range 										*/
	char *rangeto;      /* to range											*/
	short recsallocs;	/* cache was allocaded for recsallocs*tabreclen		*/
	dbfield *fields;	/* fields array which this cursor were created		*/
	int nflds;			/* amount of fields that there are in fields		*/

	union s_dbcur_dbms {

		struct s_dbcur_ix {				/* IXDBMS							*/
			long lnrec;	 				/* number of locked records			*/
		} ix;

		struct s_dbcur_es {				/* ESDBMS							*/
			long objectId;				/* cursor object Id					*/
			STRUCT DbConnection *con;	/* connection with server			*/
		} es;

		struct s_dbcur_sql {			/* SQLDB 							*/
			void *cur;					/* Pointer to an SQL  Cursor Object.*/
		} sql;
	} u;
};

#ifdef HAVE_PRAGMA_ALIGN
#	pragma SC align __DEFALIGN
#endif

/* Macros */
#define SCHTOI(x)	(int)((x>>16)&0xff)
#define ITOSCH(n)	((schema)n << 16)

#define TABTOI(x)	(int)((x>>8)&0xff)
#define ITOTAB(n)	((dbtable)n << 8)

#define FLDTOI(x)	((int)(x&0xff)-1)
#define ITOFLD(tab, slot) (dbfield) ((tab) | (slot+1))

#define INDTOI(x)	(int)(x&0xff)
#define CURTOI(x)	(int)(x&0xff)
        
#define PKEY(tab)	((dbindex)((tab)&0xffffff00))

#define essExecute(obj, code)	\
		((obj)->u.es.con->begin((obj)->u.es.objectId, code), (*(obj)->u.es.con))

enum stat_elem { STAT_MTIME, STAT_SIZE };

/* Defines for the get_tab_equiv function */

#define EQ_IDENTICAL	0x01000000
#define EQ_FORCE		0xFE000000

/* Not fatal  differences */

#define EQ_NEEDS_CHANGE	0x00000000
#define EQ_LESS_DIM		0x02000000
#define EQ_DIFF_MASK	0x04000000
#define EQ_LESS_LEN		0x08000000
#define EQ_LESS_NDEC	0x10000000

/* Fatal  differences */

#define EQ_NO_FIELD		0x20000000
#define EQ_BAD_TYPE		0x40000000
#define EQ_NOT_NULL		0x80000000

/* Flag for _DbOpen to avoid physical open of the file */

#define	IO_NOT_OPEN		010000

/* Magic number for CPUs */
#if MSDOS
#	ifndef __SC__
#		define CPU_MAGIC		0x5f1a
#	else
#		define CPU_MAGIC 		0x5f1d
#	endif
#else
#	if CPU_I286
#		define CPU_MAGIC		0x5f1b
#	else
#		if CPU_I386
#			define CPU_MAGIC	0x5f1c
#		else
#			define CPU_MAGIC	0x5f1d
#		endif
#	endif
#endif

/* Log DEFINITIONS */

#define	CLOSE_SCHEMA		1
#define	OPEN_SCHEMA			2
#define	BEGIN_TRANSACTION	3
#define	END_TRANSACTION		4
#define	DEL_RECORD			5
#define	PUT_RECORD			6
#define	ADD_RECORD			7
#define	SWITCH_TO_SCHEMA	8

/* ++ Prototypes ++ */

#ifdef __cplusplus
extern "C" {
#endif

extern int _DbOpenSchema(struct s_schema *sp);
extern void _DbUnlink(struct s_schema *sp, char *name, char *ext);
extern long _DbStat(vfile f, enum stat_elem elem);
extern int _DbWrite(vfile f, char *buff, unsigned int length, long offset);
extern void _DbClose(vfile f);
extern void _DbRename(struct s_schema *sp, char *oldname, char *ext, char *newname, char *new_ext);
extern vfile _DbOpen(struct s_schema *sp, char *name, char *ext, int mode);
extern int _DbRead(vfile f, char *buff, unsigned length, long offset, int mode);
extern void _DbFree(vfile f, UShort length, long offset);
extern int _DbLockf(vfile f, int lock_mode);
/* Prototype added to support compatibility with Version 432. */
extern int _CheckRecord(dbtable tab, bool all, char *arg);
extern DLLDEPEND bool _IsNull(char *p, itype t);
extern bool _CheckKey(struct s_index*ip, char *rec_buff);
extern int _FindFldSlot(struct s_table *tp, const char *fname);
extern int _FindIndSlot(struct s_table *tp, const char *name);
/*extern int _FindSchSlot(char *dir, const char *name, long vers); */
extern int _FindTabSlot(struct s_schema *sp, const char *name);
/*extern int _NewSchSlot(char *dir, const char *name, long vers); */
extern int _NewTabSlot(struct s_schema *sp, const char *name);
extern long _FindKey(struct tab_array *tap, struct s_index *ip, char *rec_buff, find_mode fmode, UShort nparts, char **key_ptr, int flags);
extern void _BuildKey(register char *key_buff, struct s_index *ip, register char *rec_buff, long recno);
extern void _UnBuildKey(register char *key_buff, struct s_index *ip, register char *rec_buff);
extern int _KeyCmp(struct s_index *ip, char *buff1, char *buff2);
extern int XKeyCmp(register signed char *a, register signed char *b);
extern void _SetKeyCmpIp(struct s_index *ip);
extern void _SetKeyParts(UShort kp);
extern DLLDEPEND void _PushRecord(struct tab_array *tap, void *buff, long size);
extern DLLDEPEND void _PopRecord(struct tab_array *tap, void *buff, long size);
extern void _ReOpenSchema(schema schm);
extern int _TabSeqNumber(dbtable tbl);


extern dbcursor _CreateCursor(dbindex ind, int mode);
extern DLLDEPEND void _DeleteCursor(struct s_dbcur *cursor);
extern dbindex _GetIndCursor(dbcursor cursor);
extern void _SendRangeCursor(struct s_dbcur *cursor);
extern long _GetNextBlock(struct s_dbcur *cp);
extern long _GetPrevBlock(struct s_dbcur *cp);
extern struct s_field* _GetFldPtr(dbfield fld);
extern struct s_field *_XGetFldPtr(dbfield fld, struct s_table **tpp, char **rb);
extern struct s_index*_GetIndPtr(dbindex ind);
extern long _AddKey(struct s_index*ip, long recno, char *rec_buff, struct tab_array*tap, dbindex ind);
extern UShort _FindUserPrivg(struct s_table *tp);
extern UShort _FindSchPrivg(struct s_schema *sp);
extern void _FreeSchSlot(int slot);
extern void FreeUnusedSlots(void) ;
extern int SlotsOcuppied(void);
extern struct s_schema *_GetSchPtr(schema schm);
extern schema _SetSchPtr(int slot, struct s_schema *schptr);
extern void _FreeTbSlot(struct s_schema *sp, int slot);
extern struct s_table *_GetTabPtr(dbtable tbl);
extern char *_GetTabBuff(dbtable tbl);
extern dbtable _SetTabPtr(schema schm, int slot, struct s_table *tabptr);
extern void _ModifySchema(struct s_schema *sp);
extern char *_StrValPut(struct s_schema *schptr, char *s);
extern char *_MemValPut(struct s_schema *schptr, char *s, UShort len);
extern char *_StrValPutOrAlloc(struct s_schema *schptr, char *s, char *prev);
extern void *_MemValPutOrAlloc(struct s_schema *schptr, void *s, UShort len, void *prev);
extern bool _FlushSchPrivileges(struct s_schema *sp);
extern bool _FlushTabPrivileges(struct s_schema *sp);
extern int _WriteSchema(schema schm);
extern DLLDEPEND void _FlushTable(struct tab_array *tap, int ind);
extern void _LockTabHeader(struct s_table *tp, bool aborte);
extern void _InitDbField(struct s_field *fp, char *cp, UShort n, dbfield fld);
extern void _FreeTbHeader(struct s_table *tp);
extern long _ReadRecord(struct tab_array *tap, dbindex ind, find_mode fmode, int mode, long recno, char *key_buff, char *dest_buff);
extern int _ReadRootNode(register struct s_index *ip, int lock_mode);
extern void _GetNodePar(register struct s_index *ip, register UShort n, register UShort *length, register UShort *recno);
extern int _ReadLeafNode(register struct s_index *ip, UShort n, bool new_flg);
extern long _AddRecord(struct tab_array *tap, char *buffer);
extern void _FlushCache(struct tab_array *tap);
extern void _DelKey(struct s_index *ip, long recno, char *rec_buff);
extern int SetDataCache(dbtable tab, UShort n, int mode);
extern void SetIndexCache(dbindex ind, UShort n);
extern void FreeIndex(dbindex ind, bool clearCache);
extern void _DbLog(UShort op, ...);
extern const char *CheckRecord(dbtable tab, int ind, char *par);
extern const char *__CheckRecord(dbtable tab, bool all, char *arg);
extern bool _IsStampField(struct s_field *fp); 
extern int _CreateIndex(struct s_index *ip);
extern dbtable _ReCreateTable(schema schm, char *name, char *descr, tab_flags flags, ULong size, UChar n_flds);

extern STRUCT InCheck *CreateIntegrity(dbindex ind, int nk, int nd, bool neg);
extern STRUCT InCheck *CreateInList(int nv, bool neg);
extern dbindex IntegrityIndex(STRUCT InCheck *ic);
extern void IntegritySetKey(STRUCT InCheck *ic, dbindex ind, int row, dbtable tab, dbtable atab);
extern bool InCheckValid(STRUCT InCheck *ic, const char *val, dbfield fld, int row, dbtable atab);
extern int InCheckNDspFld(STRUCT InCheck *ic);
extern int InCheckNKeyVal(STRUCT InCheck *ic);
extern bool IntegrityKeyValConst(STRUCT InCheck *ic, int i);
extern void InCheckGetKeyVal(STRUCT InCheck *ic, int i, char *buff);
extern dbfield IntegrityGetKeyFld(STRUCT InCheck *ic, int i);
extern void InCheckGetDspVal(STRUCT InCheck *ic, int i, char *buff);
extern dbfield IntegrityGetDspFld(STRUCT InCheck *ic, int n);
extern void DeleteInCheck(STRUCT InCheck *ic);
extern void AddInCheckKeyVal(STRUCT InCheck *ic, char *s);
extern void AddInCheckKeyFld(STRUCT InCheck *ic, dbfield fld);
extern void AddInCheckDspVal(STRUCT InCheck *ic, char *s);
extern void AddInCheckDspFld(STRUCT InCheck *ic, dbfield fld);
extern long StoreInCheck(STRUCT InCheck *ic, char *buff);
extern STRUCT InCheck *LoadInCheck(char *buff);
extern void PrintInCheck(STRUCT InCheck *ic);
extern bool InCheckIsInList(STRUCT InCheck *ic);
extern bool InCheckIsNegated(STRUCT InCheck *ic);
extern char *InListDescr(struct s_field *fp, const char *val);
extern int InListMaxDescrLen(STRUCT InCheck *ic);
extern void InCheckToString(STRUCT InCheck *ic, char *buff, type t);
extern void IntegrityCorrectRefs(STRUCT InCheck *ic, dbtable tbl, dbtable newtbl);
extern bool InCheckReferenced(STRUCT InCheck *ic, dbtable tbl);

extern STRUCT DbExpr *CreateDbExpr(char *buff, dbfield f, UShort r);
extern void DeleteDbExpr(STRUCT DbExpr *e);
extern short LoadDbExpr(STRUCT DbExpr *e, void *buff);
extern STRUCT DbExpr *CreateBuffDbExpr(void *buff);
extern short StoreDbExpr(STRUCT DbExpr *e, void *buff);
extern short GetLenDbExpr(STRUCT DbExpr *e);
extern char *StringDbExpr(STRUCT DbExpr *e);
extern void SetDbExpr(STRUCT DbExpr *e, dbfield f, UShort r);
extern int EvalDbExpr(STRUCT DbExpr *e);
extern char *ExecDbExpr(STRUCT DbExpr *e);
extern int ResolveFieldsDbExpr(STRUCT DbExpr *e);
extern bool HaveFieldsDbExpr(STRUCT DbExpr *e);
extern int ChkSyntaxDbExpr(STRUCT DbExpr *e);
extern long AddressDbExpr(STRUCT DbExpr *e);
extern bool _AddSchemaPriv(struct s_schema *sp, UShort privilege, UShort id, int idtype, bool grant);
extern bool _AddGrantPrivilege(struct s_table *tp, UShort privilege, UShort id, int idtype);
extern bool _RevokePrivilege(struct s_table *tp, UShort privilege,	UShort id, int idtype);
extern void CopyPrivg(dbtable to, dbtable from);
extern void CopyPrivileges(dbtable t_to, dbtable t_from);
extern void PrintPrivileges(dbtable t);
extern void _PrintPrivileges(struct s_table* t);
extern int ChownSchema(schema schm, long uid);
extern int ConvertTable(dbtable tab, dbtable newtab, bool force, bool drop, IFCPVA err_func, FCPVA msg_func, unsigned int cache, unsigned int dcache, bool correctRefs);
extern void CorrectRefs(dbtable tbl, dbtable newtbl);
extern void _ChangeTabSlot(struct s_table *tp, dbtable from, dbtable to);
extern void CorruptTrans();
extern int  GetTransMode();
extern bool _BeginTrans(STRUCT DbConnection *con, bool condStart);
extern bool _EndTrans(STRUCT DbConnection *con);
extern struct s_table *_AttachTable(struct s_table *t);
extern const char *_detailMsg(int nerr);
extern char *_AddSchemaSuffix(char *name);

/*
* ESDBMS
*/

extern void ReplaceTable(dbtable newtab, dbtable tab);
extern long ReceiveRecord(dbtable tab);
extern void _ReceiveFields(struct s_table *tp, STRUCT DbConnection *con, dbfield *flds, int nflds, char *buff);
extern void _SendFields(struct s_table *tp, STRUCT DbConnection *con, dbfield *flds, int nflds, char *buff);
extern void _ReceiveRecord(struct s_table *tp, STRUCT DbConnection *con, char *buff);
extern long ReceiveKey(struct s_index *ip, register char *rec_buff);
extern void _ReceiveKey(struct s_index *ip, STRUCT DbConnection *con, register char *rec_buff);
extern void SendFld(STRUCT DbConnection *ucon, struct s_field *fp, char *pos);
extern void SendRecord(dbtable tab);
extern void SendKey(struct s_index *ip, char *rec_buff, find_mode fmode);
extern Int  ConvertFlags(find_mode fmode, int mode);
extern long _CursorBuildIndex(struct s_dbcur *cursor, struct s_index *index, bool clear);
extern void _FreeCursorCache(struct s_dbcur *cp);
extern struct s_dbcur *_GetCursorPtr(dbcursor cursor);
extern char *_ExtractServer(char *name, char *server);
extern void ReceiveRecordPrefix(dbtable tab);
extern void _ReceiveRecordPrefix(struct s_table *tp, STRUCT DbConnection *con, record_prefix *p);
extern void ResolveVirtualFields(dbtable tab);
extern void _ResolveRecordPrefix(struct s_table *tp, char *rec_buff, record_prefix *p);
extern long _GetSchemaVersion(char *name, long version);
extern void _lockFlush(struct s_table *tp);
extern void _releaseFlush(void);
extern STRUCT IxField *BuildIxField(struct s_field *fp);

/*
* IXDBMS
*/

extern char *_ExtractDir(char *name,char *dir);
extern long _FindRecord(dbindex ind, find_mode fmode, char *rec_buff, UShort nparts);
extern char *MakeTempName(char *s);
extern void _free_record(struct s_table *tp, long recno);

/*
* SQLFW
*/
STRUCT SQLFWConnection;
STRUCT FieldMetadata;
STRUCT Field;
STRUCT String;
STRUCT DBMSProperties;

extern void setTransactionError(const char *errorDescr);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
extern bool isInGetRecordTrans();
extern String getTransactionError();
extern bool _SQLFWBeginTrans(SQLFWConnection *con, bool condStart);
extern bool _SQLFWEndTrans(SQLFWConnection *con);
#endif
extern void execMoveCursorFirst(dbcursor cursor);
extern void execMoveCursorLast(dbcursor cursor);
extern void execSetCursor(dbcursor cursor);
extern void execSetDirty(dbfield fld);
extern void execTrueInit(dbcursor cur);
extern void SQLFWFreeTableSlot(struct s_schema *sp, int slot);
extern Int SQLFWSetTabPtr(Int slot, struct s_schema *sp, struct s_table *tabptr);
extern Int SQLFWNewTabSlot(struct s_schema *sp, char const *name);
extern char * SQLFWGetCursorBuffer(struct s_dbcur *cp);
extern void SQLFWDumpTable(dbtable tab);
extern void SQLFWDumpIndex(dbtable tab, dbindex ind);
extern void SQLFWDumpSchema(schema sc);
extern long _SQLFWFindKey(dbindex ind, find_mode fmode, UShort nparts);
extern void dumpSchemaToFile(schema sc);
extern void execSetDirty(dbfield fld);
#ifdef __cplusplus
extern Field *buildSQLField(FieldMetadata *fmd);
extern DBMSProperties *getDBMSProperties();
#endif

#ifdef __cplusplus
}
#endif

/* -- Prototypes -- */

#if HAVE_PRAGMA_PACK
#	pragma pack()
#endif

#endif

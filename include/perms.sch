/* SCHEMA: perms Permission Schema	*/


/* TABLE: USERS 	RECORD LENGTH: 46	*/

# define USERS           (dbtable) 0x0100
# define USERS_NAME (dbfield) 0x0101	/* String */
# define USERS_FULLNAME (dbfield) 0x0102	/* String */
# define USERS_GPID (dbfield) 0x0103	/* Long */
# define USERSbyGPID (dbindex) 0x0100
# define USERSbyNAME (dbindex) 0x0101


/* TABLE: GROUPS 	RECORD LENGTH: 47	*/

# define GROUPS          (dbtable) 0x0200
# define GROUPS_ID (dbfield) 0x0201	/* Long */
# define GROUPS_NAME (dbfield) 0x0202	/* String */
# define GROUPS_FULLNAME (dbfield) 0x0203	/* String */
# define GROUPS_TYPE (dbfield) 0x0204	/* Integer */
# define GROUPSbyID (dbindex) 0x0200
# define GROUPSbyNAME (dbindex) 0x0201


/* TABLE: UNITS 	RECORD LENGTH: 49	*/

# define UNITS           (dbtable) 0x0300
# define UNITS_ID (dbfield) 0x0301	/* Long */
# define UNITS_NAME (dbfield) 0x0302	/* String */
# define UNITS_TYPE (dbfield) 0x0303	/* Integer */
# define UNITS_HOST (dbfield) 0x0304	/* String */
# define UNITSbyID (dbindex) 0x0300
# define UNITSbyNAME (dbindex) 0x0301
# define UNITSbyTYPE (dbindex) 0x0302


/* TABLE: INSTS 	RECORD LENGTH: 216	*/

# define INSTS           (dbtable) 0x0400
# define INSTS_UNITID (dbfield) 0x0401	/* Long */
# define INSTS_INSTID (dbfield) 0x0402	/* Integer */
# define INSTS_DESCRIP (dbfield) 0x0403	/* String */
# define INSTS_FATHER (dbfield) 0x0404	/* Long */
# define INSTS_ARGS (dbfield) 0x0405	/* String */
# define INSTS_POSORDER (dbfield) 0x0406	/* Long */
# define INSTSbyUNITID (dbindex) 0x0400
# define INSTSbyFATHER (dbindex) 0x0401
# define INSTSbyORDER (dbindex) 0x0402


/* TABLE: UNITPERM 	RECORD LENGTH: 32	*/

# define UNITPERM        (dbtable) 0x0500
# define UNITPERM_UNITID (dbfield) 0x0501	/* Long */
# define UNITPERM_GPID (dbfield) 0x0502	/* Long */
# define UNITPERM_PERMS (dbfield) 0x0503	/* Long */
# define UNITPERMbyUNITID (dbindex) 0x0500
# define UNITPERMbyGPID (dbindex) 0x0501


/* TABLE: MODINST 	RECORD LENGTH: 96	*/

# define MODINST         (dbtable) 0x0600
# define MODINST_ID (dbfield) 0x0601	/* Integer */
# define MODINST_DESCRIP (dbfield) 0x0602	/* String */
# define MODINST_WSCH (dbfield) 0x0603	/* String */
# define MODINST_VERSION (dbfield) 0x0604	/* String */
# define MODINST_INST (dbfield) 0x0605	/* Integer */
# define MODINST_CDATE (dbfield) 0x0606	/* Date */
# define MODINST_CTIME (dbfield) 0x0607	/* Time */
# define MODINST_CUID (dbfield) 0x0608	/* Long */
# define MODINST_MDATE (dbfield) 0x0609	/* Date */
# define MODINST_MTIME (dbfield) 0x060a	/* Time */
# define MODINST_MUID (dbfield) 0x060b	/* Long */
# define MODINSTbyID (dbindex) 0x0600


/* TABLE: PERUSER 	RECORD LENGTH: 4134	*/

# define PERUSER         (dbtable) 0x0700
# define PERUSER_IDUSER (dbfield) 0x0701	/* Long */
# define PERUSER_FIRSTNAME (dbfield) 0x0702	/* String */
# define PERUSER_LASTNAME (dbfield) 0x0703	/* String */
# define PERUSER_LOGIN (dbfield) 0x0704	/* String */
# define PERUSER_PASSWORD (dbfield) 0x0705	/* String */
# define PERUSER_EMAIL (dbfield) 0x0706	/* String */
# define PERUSER_EMAIL2 (dbfield) 0x0707	/* String */
# define PERUSER_ADDRESS (dbfield) 0x0708	/* String */
# define PERUSER_PHONE (dbfield) 0x0709	/* String */
# define PERUSER_PHONE2 (dbfield) 0x070a	/* String */
# define PERUSER_FAX (dbfield) 0x070b	/* String */
# define PERUSER_MOBIL (dbfield) 0x070c	/* String */
# define PERUSER_CITY (dbfield) 0x070d	/* String */
# define PERUSER_STATE (dbfield) 0x070e	/* String */
# define PERUSER_COUNTRY (dbfield) 0x070f	/* String */
# define PERUSER_ACTIVE (dbfield) 0x0710	/* Integer */
# define PERUSER_MODIFIER (dbfield) 0x0711	/* Long */
# define PERUSER_CREATOR (dbfield) 0x0712	/* Long */
# define PERUSER_MDATE (dbfield) 0x0713	/* String */
# define PERUSER_CDATE (dbfield) 0x0714	/* String */
# define PERUSER_MTIME (dbfield) 0x0715	/* String */
# define PERUSER_CTIME (dbfield) 0x0716	/* String */
# define PERUSER_PASSFROM (dbfield) 0x0717	/* String */
# define PERUSER_PASSTO (dbfield) 0x0718	/* String */
# define PERUSER_WDATE (dbfield) 0x0719	/* String */
# define PERUSER_WDAYS (dbfield) 0x071a	/* Integer */
# define PERUSER_FORCECHANGE (dbfield) 0x071b	/* Integer */
# define PERUSER_ALLOWCHANGE (dbfield) 0x071c	/* String */
# define PERUSER_EXPDAYS (dbfield) 0x071d	/* Integer */
# define PERUSER_NEVEREXP (dbfield) 0x071e	/* Integer */
# define PERUSER_IDPROFILE (dbfield) 0x071f	/* Long */
# define PERUSER_CHANGEPROFILE (dbfield) 0x0720	/* Integer */
# define PERUSER_DESCRIP (dbfield) 0x0721	/* String */
# define PERUSER_STATUS (dbfield) 0x0722	/* String */
# define PERUSER_TYPE (dbfield) 0x0723	/* String */
# define PERUSER_SUBTYPE (dbfield) 0x0724	/* String */
# define PERUSER_STREETTY (dbfield) 0x0725	/* String */
# define PERUSER_STREETNU (dbfield) 0x0726	/* Integer */
# define PERUSER_FLOOR (dbfield) 0x0727	/* Integer */
# define PERUSER_DEPART (dbfield) 0x0728	/* String */
# define PERUSER_ZIPCODE (dbfield) 0x0729	/* String */
# define PERUSER_BSTREET1 (dbfield) 0x072a	/* String */
# define PERUSER_BSTREET2 (dbfield) 0x072b	/* String */
# define PERUSER_IDNEIGHB (dbfield) 0x072c	/* String */
# define PERUSER_IDLOCAL (dbfield) 0x072d	/* String */
# define PERUSER_PHONE3 (dbfield) 0x072e	/* String */
# define PERUSER_CEL2 (dbfield) 0x072f	/* String */
# define PERUSER_WEBPAGE (dbfield) 0x0730	/* String */
# define PERUSER_JABBER (dbfield) 0x0731	/* String */
# define PERUSER_SMS (dbfield) 0x0732	/* String */
# define PERUSER_PUBLICKEY (dbfield) 0x0733	/* String */
# define PERUSER_PRIVATEKEY (dbfield) 0x0734	/* String */
# define PERUSERbyIDUSER (dbindex) 0x0700
# define PERUSERbyLOGIN (dbindex) 0x0701
# define PERUSERbyNAME (dbindex) 0x0702


/* TABLE: PERUGREL 	RECORD LENGTH: 311	*/

# define PERUGREL        (dbtable) 0x0800
# define PERUGREL_IDGROUP (dbfield) 0x0801	/* Long */
# define PERUGREL_IDUSER (dbfield) 0x0802	/* Long */
# define PERUGREL_GROUPROLE (dbfield) 0x0803	/* String */
# define PERUGREL_DESCRIPT (dbfield) 0x0804	/* String */
# define PERUGRELbyIDGROUP (dbindex) 0x0800


/* TABLE: PERGROUP 	RECORD LENGTH: 304	*/

# define PERGROUP        (dbtable) 0x0900
# define PERGROUP_IDGROUP (dbfield) 0x0901	/* Long */
# define PERGROUP_NAME (dbfield) 0x0902	/* String */
# define PERGROUP_DESCRIPT (dbfield) 0x0903	/* String */
# define PERGROUP_TYPE (dbfield) 0x0904	/* Integer */
# define PERGROUPbyIDGROUP (dbindex) 0x0900
# define PERGROUPbyNAME (dbindex) 0x0901


/* TABLE: PERPPREL 	RECORD LENGTH: 281	*/

# define PERPPREL        (dbtable) 0x0a00
# define PERPPREL_TYPEPART (dbfield) 0x0a01	/* Integer */
# define PERPPREL_IDPART (dbfield) 0x0a02	/* Long */
# define PERPPREL_IDPROF (dbfield) 0x0a03	/* Long */
# define PERPPREL_DESCRIPT (dbfield) 0x0a04	/* String */
# define PERPPRELbyTYPEPART (dbindex) 0x0a00
# define PERPPRELbyKEYPPREL2 (dbindex) 0x0a01
# define PERPPRELbyKEYPPREL1 (dbindex) 0x0a02
# define PERPPRELbyKEYPPREL3 (dbindex) 0x0a03


/* TABLE: PERPRREL 	RECORD LENGTH: 281	*/

# define PERPRREL        (dbtable) 0x0b00
# define PERPRREL_TYPEPART (dbfield) 0x0b01	/* Integer */
# define PERPRREL_IDPART (dbfield) 0x0b02	/* Long */
# define PERPRREL_IDROLE (dbfield) 0x0b03	/* Long */
# define PERPRREL_DESCRIPT (dbfield) 0x0b04	/* String */
# define PERPRRELbyTYPEPART (dbindex) 0x0b00
# define PERPRRELbyIDROLE (dbindex) 0x0b01


/* TABLE: PERATRIB 	RECORD LENGTH: 475	*/

# define PERATRIB        (dbtable) 0x0c00
# define PERATRIB_TYPEPART (dbfield) 0x0c01	/* Integer */
# define PERATRIB_IDPART (dbfield) 0x0c02	/* Long */
# define PERATRIB_NAME (dbfield) 0x0c03	/* String */
# define PERATRIB_DESCRIPT (dbfield) 0x0c04	/* String */
# define PERATRIB_VALUE (dbfield) 0x0c05	/* String */
# define PERATRIBbyTYPEPART (dbindex) 0x0c00


/* TABLE: PERSESSION 	RECORD LENGTH: 116	*/

# define PERSESSION      (dbtable) 0x0d00
# define PERSESSION_IDUSER (dbfield) 0x0d01	/* Long */
# define PERSESSION_PROFILENAME (dbfield) 0x0d02	/* String */
# define PERSESSION_STARTDATE (dbfield) 0x0d03	/* String */
# define PERSESSION_STARTTIME (dbfield) 0x0d04	/* String */
# define PERSESSION_ENDDATE (dbfield) 0x0d05	/* String */
# define PERSESSION_ENDTIME (dbfield) 0x0d06	/* String */
# define PERSESSIONbyIDUSER (dbindex) 0x0d00


/* TABLE: PERSESSACT 	RECORD LENGTH: 633	*/

# define PERSESSACT      (dbtable) 0x0e00
# define PERSESSACT_IDSESSION (dbfield) 0x0e01	/* Long */
# define PERSESSACT_IDACTIVITY (dbfield) 0x0e02	/* Float */
# define PERSESSACT_IDSYSTEM (dbfield) 0x0e03	/* Long */
# define PERSESSACT_IDMODULE (dbfield) 0x0e04	/* Long */
# define PERSESSACT_IDUNIT (dbfield) 0x0e05	/* Long */
# define PERSESSACT_IDFUNCTION (dbfield) 0x0e06	/* Long */
# define PERSESSACT_ACTDATE (dbfield) 0x0e07	/* String */
# define PERSESSACT_ACTTIME (dbfield) 0x0e08	/* String */
# define PERSESSACT_STATUS (dbfield) 0x0e09	/* String */
# define PERSESSACT_DESCRIPT (dbfield) 0x0e0a	/* String */
# define PERSESSACT_TYPE (dbfield) 0x0e0b	/* Integer */
# define PERSESSACT_PNAME (dbfield) 0x0e0c	/* String */
# define PERSESSACT_PID (dbfield) 0x0e0d	/* Float */
# define PERSESSACT_REFERENCE (dbfield) 0x0e0e	/* Float */
# define PERSESSACTbyIDSESSION (dbindex) 0x0e00


/* TABLE: PERPROFILE 	RECORD LENGTH: 1244	*/

# define PERPROFILE      (dbtable) 0x0f00
# define PERPROFILE_IDPROFILE (dbfield) 0x0f01	/* Long */
# define PERPROFILE_IDMENU (dbfield) 0x0f02	/* Long */
# define PERPROFILE_DESCRIPT (dbfield) 0x0f03	/* String */
# define PERPROFILE_NAME (dbfield) 0x0f04	/* String */
# define PERPROFILE_ENVIROMENT (dbfield) 0x0f05	/* String */
# define PERPROFILE_CONFIGURATION (dbfield) 0x0f06	/* String */
# define PERPROFILE_CUSERID (dbfield) 0x0f07	/* Long */
# define PERPROFILE_CDATE (dbfield) 0x0f08	/* String */
# define PERPROFILE_CTIME (dbfield) 0x0f09	/* String */
# define PERPROFILE_MUSERID (dbfield) 0x0f0a	/* Long */
# define PERPROFILE_MDATE (dbfield) 0x0f0b	/* String */
# define PERPROFILE_MTIME (dbfield) 0x0f0c	/* String */
# define PERPROFILE_ACTIVE (dbfield) 0x0f0d	/* Integer */
# define PERPROFILEbyIDPROFILE (dbindex) 0x0f00


/* TABLE: PERROLE 	RECORD LENGTH: 323	*/

# define PERROLE         (dbtable) 0x1000
# define PERROLE_IDROLE (dbfield) 0x1001	/* Long */
# define PERROLE_NAME (dbfield) 0x1002	/* String */
# define PERROLE_DESCRIPT (dbfield) 0x1003	/* String */
# define PERROLEbyIDROLE (dbindex) 0x1000


/* TABLE: PERCONST 	RECORD LENGTH: 91	*/

# define PERCONST        (dbtable) 0x1100
# define PERCONST_IDROLE (dbfield) 0x1101	/* Long */
# define PERCONST_PERMS (dbfield) 0x1102	/* String */
# define PERCONST_IDUNIT (dbfield) 0x1103	/* Long */
# define PERCONST_IDSYSTEM (dbfield) 0x1104	/* Long */
# define PERCONST_IDMODULES (dbfield) 0x1105	/* Long */
# define PERCONSTbyIDROLE (dbindex) 0x1100


/* TABLE: GENSYSTEM 	RECORD LENGTH: 323	*/

# define GENSYSTEM       (dbtable) 0x1200
# define GENSYSTEM_IDSYSTEM (dbfield) 0x1201	/* Long */
# define GENSYSTEM_NAME (dbfield) 0x1202	/* String */
# define GENSYSTEM_DESCRIPT (dbfield) 0x1203	/* String */
# define GENSYSTEMbyIDSYSTEM (dbindex) 0x1200


/* TABLE: GENVERSION 	RECORD LENGTH: 373	*/

# define GENVERSION      (dbtable) 0x1300
# define GENVERSION_IDSYSTEM (dbfield) 0x1301	/* Long */
# define GENVERSION_IDVERSION (dbfield) 0x1302	/* Long */
# define GENVERSION_NAME (dbfield) 0x1303	/* String */
# define GENVERSION_DESCRIPT (dbfield) 0x1304	/* String */
# define GENVERSION_DATEIN (dbfield) 0x1305	/* String */
# define GENVERSION_DATEOUT (dbfield) 0x1306	/* String */
# define GENVERSIONbyIDSYSTEM (dbindex) 0x1300


/* TABLE: GENMODULE 	RECORD LENGTH: 354	*/

# define GENMODULE       (dbtable) 0x1400
# define GENMODULE_IDSYSTEM (dbfield) 0x1401	/* Long */
# define GENMODULE_IDMODULE (dbfield) 0x1402	/* Long */
# define GENMODULE_NAME (dbfield) 0x1403	/* String */
# define GENMODULE_DESCRIPT (dbfield) 0x1404	/* String */
# define GENMODULE_IDVERSION (dbfield) 0x1405	/* Long */
# define GENMODULE_REFERENCE (dbfield) 0x1406	/* Integer */
# define GENMODULE_WSCH (dbfield) 0x1407	/* String */
# define GENMODULEbyIDSYSTEM (dbindex) 0x1400


/* TABLE: GENUNIT 	RECORD LENGTH: 893	*/

# define GENUNIT         (dbtable) 0x1500
# define GENUNIT_IDUNIT (dbfield) 0x1501	/* Long */
# define GENUNIT_IDSYSTEM (dbfield) 0x1502	/* Long */
# define GENUNIT_IDMODULE (dbfield) 0x1503	/* Long */
# define GENUNIT_NAME (dbfield) 0x1504	/* String */
# define GENUNIT_DESCRIPT (dbfield) 0x1505	/* String */
# define GENUNIT_TYPE (dbfield) 0x1506	/* Integer */
# define GENUNIT_PROGRAM (dbfield) 0x1507	/* String */
# define GENUNIT_DEFARGS (dbfield) 0x1508	/* String */
# define GENUNIT_URL (dbfield) 0x1509	/* String */
# define GENUNITbyIDUNIT (dbindex) 0x1500
# define GENUNITbyKEYUNIT (dbindex) 0x1501


/* TABLE: GENFUNC 	RECORD LENGTH: 348	*/

# define GENFUNC         (dbtable) 0x1600
# define GENFUNC_IDSYSTEM (dbfield) 0x1601	/* Long */
# define GENFUNC_IDMODULE (dbfield) 0x1602	/* Long */
# define GENFUNC_IDUNIT (dbfield) 0x1603	/* Long */
# define GENFUNC_IDFUNCTION (dbfield) 0x1604	/* Long */
# define GENFUNC_NAME (dbfield) 0x1605	/* String */
# define GENFUNC_DESCRIPT (dbfield) 0x1606	/* String */
# define GENFUNC_TYPE (dbfield) 0x1607	/* String */
# define GENFUNCbyIDSYSTEM (dbindex) 0x1600


/* TABLE: PERMENU 	RECORD LENGTH: 1009	*/

# define PERMENU         (dbtable) 0x1700
# define PERMENU_IDMENU (dbfield) 0x1701	/* Long */
# define PERMENU_IDITEM (dbfield) 0x1702	/* Long */
# define PERMENU_IDINSTANCE (dbfield) 0x1703	/* Integer */
# define PERMENU_CUSERID (dbfield) 0x1704	/* Long */
# define PERMENU_CDATE (dbfield) 0x1705	/* String */
# define PERMENU_CTIME (dbfield) 0x1706	/* String */
# define PERMENU_MUSERID (dbfield) 0x1707	/* Long */
# define PERMENU_MDATE (dbfield) 0x1708	/* String */
# define PERMENU_MTIME (dbfield) 0x1709	/* String */
# define PERMENU_NAME (dbfield) 0x170a	/* String */
# define PERMENU_DESCRIPT (dbfield) 0x170b	/* String */
# define PERMENU_FILENAME (dbfield) 0x170c	/* String */
# define PERMENUbyIDMENU (dbindex) 0x1700


/* TABLE: PERMNITEM 	RECORD LENGTH: 590	*/

# define PERMNITEM       (dbtable) 0x1800
# define PERMNITEM_IDMNITEM (dbfield) 0x1801	/* Long */
# define PERMNITEM_IDMENU (dbfield) 0x1802	/* Long */
# define PERMNITEM_PERMISION (dbfield) 0x1803	/* Long */
# define PERMNITEM_IDUNIT (dbfield) 0x1804	/* Long */
# define PERMNITEM_IDPARENT (dbfield) 0x1805	/* Long */
# define PERMNITEM_SLABEL (dbfield) 0x1806	/* String */
# define PERMNITEM_LLABEL (dbfield) 0x1807	/* String */
# define PERMNITEM_DESCRIPT (dbfield) 0x1808	/* String */
# define PERMNITEM_ARGS (dbfield) 0x1809	/* String */
# define PERMNITEM_IDSYSTEM (dbfield) 0x180a	/* Long */
# define PERMNITEM_IDMODULE (dbfield) 0x180b	/* Long */
# define PERMNITEM_IDREL (dbfield) 0x180c	/* Long */
# define PERMNITEM_ISLEAF (dbfield) 0x180d	/* Integer */
# define PERMNITEM_MORDER (dbfield) 0x180e	/* Long */
# define PERMNITEMbyIDMNITEM (dbindex) 0x1800


/* TABLE: GENCOMPON 	RECORD LENGTH: 901	*/

# define GENCOMPON       (dbtable) 0x1900
# define GENCOMPON_IDCOMPONENT (dbfield) 0x1901	/* Long */
# define GENCOMPON_IDUNIT (dbfield) 0x1902	/* Long */
# define GENCOMPON_IDSYSTEM (dbfield) 0x1903	/* Long */
# define GENCOMPON_IDMODULE (dbfield) 0x1904	/* Long */
# define GENCOMPON_NAME (dbfield) 0x1905	/* String */
# define GENCOMPON_DESCRIPT (dbfield) 0x1906	/* String */
# define GENCOMPON_TYPE (dbfield) 0x1907	/* Integer */
# define GENCOMPON_PROGRAM (dbfield) 0x1908	/* String */
# define GENCOMPON_DEFARGS (dbfield) 0x1909	/* String */
# define GENCOMPON_URL (dbfield) 0x190a	/* String */
# define GENCOMPONbyIDCOMPONENT (dbindex) 0x1900
# define GENCOMPONbyKEYUNIT (dbindex) 0x1901


/* TABLE: PERRSREL 	RECORD LENGTH: 57	*/

# define PERRSREL        (dbtable) 0x1a00
# define PERRSREL_IDROLE (dbfield) 0x1a01	/* Long */
# define PERRSREL_IDSYSTEM (dbfield) 0x1a02	/* Long */
# define PERRSREL_IDMODULE (dbfield) 0x1a03	/* Long */
# define PERRSREL_IDUNIT (dbfield) 0x1a04	/* Long */
# define PERRSREL_IDFUNCTION (dbfield) 0x1a05	/* Long */
# define PERRSREL_PERMISSION (dbfield) 0x1a06	/* Long */
# define PERRSREL_TYPE (dbfield) 0x1a07	/* Integer */
# define PERRSRELbyIDROLE (dbindex) 0x1a00


/* TABLE: PERAUDIT 	RECORD LENGTH: 284	*/

# define PERAUDIT        (dbtable) 0x1b00
# define PERAUDIT_ID (dbfield) 0x1b01	/* Long */
# define PERAUDIT_IDUSER (dbfield) 0x1b02	/* Long */
# define PERAUDIT_ACCION (dbfield) 0x1b03	/* String */
# define PERAUDIT_CDATE (dbfield) 0x1b04	/* Date */
# define PERAUDIT_CTIME (dbfield) 0x1b05	/* Time */
# define PERAUDITbyID (dbindex) 0x1b00


/* TABLE: PERAUDDATA 	RECORD LENGTH: 792	*/

# define PERAUDDATA      (dbtable) 0x1c00
# define PERAUDDATA_ID (dbfield) 0x1c01	/* Long */
# define PERAUDDATA_IDAUDIT (dbfield) 0x1c02	/* Long */
# define PERAUDDATA_ORIGDATA (dbfield) 0x1c03	/* String */
# define PERAUDDATA_NEWDATA (dbfield) 0x1c04	/* String */
# define PERAUDDATA_CAMPO (dbfield) 0x1c05	/* String */
# define PERAUDDATAbyID (dbindex) 0x1c00


/* TABLE: SESSION 	RECORD LENGTH: 122	*/

# define SESSION         (dbtable) 0x1d00
# define SESSION_ID (dbfield) 0x1d01	/* Float */
# define SESSION_CDATE (dbfield) 0x1d02	/* Time */
# define SESSION_IDUSER (dbfield) 0x1d03	/* Long */
# define SESSION_LOGIN (dbfield) 0x1d04	/* String */
# define SESSION_PERFIL (dbfield) 0x1d05	/* String */
# define SESSION_PID (dbfield) 0x1d06	/* Long */
# define SESSION_TTY (dbfield) 0x1d07	/* String */
# define SESSIONbyID (dbindex) 0x1d00


/* TABLE: PERSEC 	RECORD LENGTH: 17	*/

# define PERSEC          (dbtable) 0x1e00
# define PERSEC_ID (dbfield) 0x1e01	/* Integer */
# define PERSEC_PASS_MAXLENGTH (dbfield) 0x1e02	/* Integer */
# define PERSEC_PASS_MINLENGTH (dbfield) 0x1e03	/* Integer */
# define PERSEC_PASS_ALLOWNREP (dbfield) 0x1e04	/* Integer */
# define PERSEC_PASS_ATTEMPTS (dbfield) 0x1e05	/* Integer */
# define PERSECbyID (dbindex) 0x1e00


/* TABLE: PERPASS 	RECORD LENGTH: 53	*/

# define PERPASS         (dbtable) 0x1f00
# define PERPASS_ID (dbfield) 0x1f01	/* Long */
# define PERPASS_PASSWD (dbfield) 0x1f02	/* String */
# define PERPASS_CDATE (dbfield) 0x1f03	/* Long */
# define PERPASS_CTIME (dbfield) 0x1f04	/* Long */
# define PERPASSbyID (dbindex) 0x1f00


/* TABLE: DBASES 	RECORD LENGTH: 82	*/

# define DBASES          (dbtable) 0x2000
# define DBASES_ID (dbfield) 0x2001	/* Integer */
# define DBASES_NOMBRE (dbfield) 0x2002	/* String */
# define DBASES_DESCRIP (dbfield) 0x2003	/* String */
# define DBASESbyID (dbindex) 0x2000


/* TABLE: DB_USERS 	RECORD LENGTH: 18	*/

# define DB_USERS        (dbtable) 0x2100
# define DB_USERS_DBASEID (dbfield) 0x2101	/* Integer */
# define DB_USERS_USERID (dbfield) 0x2102	/* Long */
# define DB_USERSbyDBASEID (dbindex) 0x2100


/* TABLE: LOGPROC 	RECORD LENGTH: 362	*/

# define LOGPROC         (dbtable) 0x2200
# define LOGPROC_PID (dbfield) 0x2201	/* Long */
# define LOGPROC_NOMUSER (dbfield) 0x2202	/* String */
# define LOGPROC_NOMPRG (dbfield) 0x2203	/* String */
# define LOGPROC_FECHAI (dbfield) 0x2204	/* Date */
# define LOGPROC_HORAI (dbfield) 0x2205	/* Time */
# define LOGPROC_UID (dbfield) 0x2206	/* Long */
# define LOGPROC_PERFIL (dbfield) 0x2207	/* String */
# define LOGPROC_MENU (dbfield) 0x2208	/* String */
# define LOGPROC_BASE (dbfield) 0x2209	/* String */
# define LOGPROC_PPID (dbfield) 0x220a	/* Long */
# define LOGPROC_DESCRIP (dbfield) 0x220b	/* String */
# define LOGPROCbyPID (dbindex) 0x2200


/* TABLE: DB_PERFIL 	RECORD LENGTH: 18	*/

# define DB_PERFIL       (dbtable) 0x2300
# define DB_PERFIL_DBASEID (dbfield) 0x2301	/* Integer */
# define DB_PERFIL_PERFID (dbfield) 0x2302	/* Long */
# define DB_PERFILbyDBASEID (dbindex) 0x2300

# define IO_PERMS_CHKSUM (long) 0xf55f

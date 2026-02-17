/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: ixsystem.h,v 1.7 2009/08/12 16:04:47 hardaiz Exp $
*
* DESCRIPTION
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*********************************************************************/

#define UNITSFILE	"units"
#define GROUPSFILE	"groups"
#define CMENUEXT	".mno"
#define AUTHDIR		"AUTHDIR"
#define OLDAUTHDIR	"OLDAUTHDIR"
#define PROFILES	"PROFILES"

#define TIME_STAMP_LEN 22
#define BOUND "$"
#define SEPAR " "

#define PASS_MIN_LENGTH	4

/**
 * En esta implementacion, los metodos loadMenu y compGroupInfo pasaron de
 * ser privados a ser publicos.
 * Tambien se implementaros los metodos:
 * static char *timeStamp();
 * static Time toTimeSt(const String &ts);
 * static void setTimeStamp(String const &);
 * static String groupName();
 * static Time tStamp(const String &mnofile, const String basedir = NULL_STRING);
 */

class IxSMenu;

class IxSystem {

// private:
	
	static bool ixScaEnabled;

	static char ts_stamp[TIME_STAMP_LEN + 1]; // time stamp.
	
	static void storeMenu(IxSMenu *menu, const String &fileName);

	static void storeUnitsPerms(const String &storeDir, Int gpId);
	static void storeGroups(const String &storeDir);

	static String authDir(bool abort = true);
	
	static void dbGroupInfo(const String &gName, Int &gpId, Int &gpType);
	static String dbGroupInfo(Int &gpId, Int &gpType);

	static Int menuId(const String &unitName);

	static bool canExecUnit(Int unitId, Int gpId, Int gpType);

	static IxSMenu *generateMenu(Int unitId, const String &unitName, 
							  Int gpid, Int gpType);
	static IxSMenu *generateMenus(Int unitId, const String &unitName, 
							   Int gpId, Int gpType);

public:
	// Para el nuevo sistema de permisos.
	static void setIxScaEnabled(bool isEnabled); 
	static bool IxScaEnabled(); 

	static void loadMenu(IxSMenu *menu, const String &fileName);
	static String compGroupInfo(Int &gpType, bool abort = true);

	static String msg(const char *message);
	
	static bool readPermission(const String &unit);

	// execute checking in the database
	static Int execute(const String &unitName);

	// execute without checking in the database
	static Int exec(const String &unitName);

	static bool generate(const String &unitName, 
						 const String &grName = NULL_STRING);

	static String mnoFullName(const String &unit);

	static void destHost(const String &h);

	static Int moduleIsInstalled(Int modId);
	static Int moduleIsInstalled(Int modId, String &wsch);
	static bool moduleExists(Int modId, String &wsch);
	
	static char *timeStamp();
	static Time toTimeSt(const String &ts);
	static void setTimeStamp(String const &);
	static String groupName();
	static Time tStamp(const String &mnofile, const String basedir = NULL_STRING);
};


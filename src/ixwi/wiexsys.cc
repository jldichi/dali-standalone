/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
* 
* $Id: wiexsys.cc,v 1.5 2005/03/23 18:30:57 gsposito Exp $
*
* DESCRIPTION:
*
* ROUTINE       |  PURPOSE
*---------------+---------------------------------------------------
*********************************************************************/
#include <stdlib.h>
#include <ideafix.h>
#include <cfix.h>

#include <cracker/prcmd.h>
#include <cracker/pserconn.h>

#include <ideafix/priv/ixsystem.h>
#ifdef __NT__
#include <ifound/unistd.h>
#else
#include <unistd.h>
#endif

static void sendMenu(PrServerConnection &conn, const String &name)
{
#ifdef HAVE_TEXT_FILES
	char *mode = "rb";
#else
	char *mode = "r";
#endif
	char buff[TRANSCHUNK];
	FILE *f = fopen(toCharPtr(name), mode);
	assert(f != NULL);

	Int nbytes;
	while ((nbytes = fread(buff, 1, TRANSCHUNK, f)) != 0)
		conn.push(buff, nbytes);

	conn.push(buff, nbytes);

	fclose(f);
}

extern "C" int ExecSystem(const char *name)
{
	String mnoFile;

	if (!crackerClient())
		return IxSystem::execute(name);

	// Modificaciones para IxSystem2. DA
	// Si la variable AUTHDIR esta seteada se esta usando el IxSystem1
	// De lo contrario se usa el IxSystem2.
	if (getenv("AUTHDIR") != NULL) { 
		mnoFile = IxSystem::mnoFullName(name);
		if (access(toCharPtr(mnoFile),R_OK|F_OK) != 0) {
			WiDialog(WD_OK,WD_OK,"Aviso","No se encuentra el archivo: ' %s ' o no posee permisos sobre el mismo. ",toCharPtr(mnoFile));
			return ERR;
		} 
	} 
	else {
		// Para que saque el .mno desde profiles.
		String profileEnv = getenv("PROFILES");

		if (strstr(name,".mno"))
			mnoFile = toString(name); 		
		else
			mnoFile = toString(name) + ".mno"; 		

		mnoFile = profileEnv + "/" + mnoFile;

		if (access(toCharPtr(mnoFile),R_OK|F_OK) != 0) {
			WiDialog(WD_OK,WD_OK,"Aviso","No se encuentra el archivo: ' %s ' o no posee permisos sobre el mismo. ",toCharPtr(mnoFile));
                        return ERR;
                }
	} 

	prServerConn(PrCmd::execSystem) << name << timeStamp(mnoFile);
	Int st;
	prServerConn >> st;		
	switch (st) {
	case NEEDFILE:
		sendMenu(prServerConn, mnoFile);
		prServerConn >> st;
		assert(st != NEEDFILE);
		if (st != ERR)
			break;
		// intentionally fall down
	case ERR:
		error("Can't execute system '%s'", name);
		break;
	default:
		break;
	}
	return int(st);
}


/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: c_start.cc,v 1.24 2010/09/28 22:25:21 hardaiz Exp $
*
* DESCRIPTION
* Program start up
*********************************************************************/

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <ideafix.h>
#include <cfix.h>
#include <cfix/priv/macros.h>
#include <ideafix/priv/gndefs.h>
#include <ideafix/priv/ixsystem.h>
#include <ifound/priv/gfuncs.h>
#include <ifound/priv/pconfig.h>
#include <cracker/pserconn.h>
#ifdef __VC__
	#include <ifound/unistd.h>
#else
	#include <unistd.h>
#endif	

#ifndef __NT__

#include <local/critsect.h>

#define IXSCAFILE ".ixscarc"
#define IXSCAHOST "IxSCA_host"
#define IXSCAPORT "IxSCA_port"


// used for Ideafix Runtime Module users license check

extern "C" void CheckUserLicenses(void);
extern "C" void SetModuleID(int);

#ifdef IXSCA
extern "C" bool getVarFromRcFile(char *fileName, char* varName, String &varValue);
extern "C" bool changeRcFile(char *where,char *newp=NULL,char *pvar=NULL);
extern "C" bool IxSCAinitSession();
extern "C" void IxSCAsetSession(int session);
extern "C" void IxSCAsetHost(char* host);
extern "C" void IxSCAsetPort(int port);
extern "C" void IxSCAsetLogin(char* login);
extern "C" void IxSCAsetProfile( const char* profile);
extern "C" void IxSCAsetPassword(char* password);
extern "C" void IxSCAsetIdUser(int idUser);
extern "C" bool IxSCAstartProgram();
extern "C" bool IxSCAendProgram();
extern "C" char* IxSCAgetMessage();
extern "C" char* IxSCAgetError();
extern "C" bool	IxSCAisSessionOpened();

extern "C" char* IxSCAdoEncrypt( const char* pass );
extern "C" char* IxSCAdoUnEncrypt( const char* pass );

extern "C" void IxSCAdebug();

#endif /* IXSCA */

extern "C" bool crackerClient();


#endif /* __NT__ */

static int sendPermsToCracker(void);


extern "C" char *timeConversion(int c, const char **, int size, Int *maxwidth,
		    va_list *ap, char *wbuff, char *, Int *)
{
	Int v = size == 'l' ? va_arg(*ap, Int) : va_arg(*ap, int);
	if (*maxwidth == INT_MAX)
		*maxwidth = 0;
	switch (c) {
	case 'T' :
		TToStr(TIME(int((Int(v)))), wbuff, *maxwidth);
		break;
	case 'D' :
		DToStr(DATE(int((Int(v)))), wbuff, *maxwidth);
		break;
	default :
		assert(0);
		break;
	}		
	char *cp = wbuff;
	*maxwidth = INT_MAX;
	return cp;
}


/**
 *
 * void Start(int *_argc  char **argv, int module)
 *
 * This function gets called by Program() and wcmd()
 * and performs initialization.
 *
 * This function  accepts a parameter
 * containing the ID of the program to let the user licenses system
 * know which program is running and work on the licenses based on
 * that information.
 * By default, PKG_RUNTIME is used as the module ID.
 * 
 *
 * module = APPL_AURUS || APPL_DENARIUS || APPL_IANUS etc.
 *
 *
 */

extern "C" void Start(int *_argc, char **argv, int module)
{
	startUp(BaseName(argv[0]), *_argc, argv);

	// If we are running under Windows, don't check user licences
	// because this restriction is checked in the cracker server
        // (irexecsv).
	// This check is for unix programs using cfix

#ifndef __NT__

	BEGIN_CRITICAL_SECTION(ULICENSE_SEMAPHORE_KEY)
	{
	// Based on the module type we set the License_pkgtype.

	SetModuleID(module);
		
	// if it is PKG_DEVELOP we don't check for users licenses.
	if( module != PKG_DEVELOP )
	{


	// If we are running ixset don't check any user
	// licenses
	// i prefer this to using a special ID like IDEAFIX_TOOLS_ID
	// to avoid user licenses checks, because this way is more
	// obscure. in terms of security, both methods are poor.

	if( strcmp( BaseName(argv[0]), "ixset") )
			CheckUserLicenses();


	}

	} END_CRITICAL_SECTION;

#endif /* __NT__ */

	if (!IxSystem::readPermission(appName()))
		_processConfig()->sig = ERR;
	addConvertFunction('T', timeConversion);
	addConvertFunction('D', timeConversion);
}

extern "C" void StartUp(const char *sys, const char *prog, const char *, 
						int *_argc, char **argv)
{
	String fName;
	if (sys == NULL || *sys == '\0')
		fName = prog;
	else
		fName = String(sys) + SYSNAME_SEPAR + prog;		
	startUp(fName, *_argc, argv);
	if (!IxSystem::readPermission(appName()))
		_processConfig()->sig = ERR;
	addConvertFunction('T', timeConversion);
	addConvertFunction('D', timeConversion);
}

extern "C" void SetUpdatePerm(bool v)
{
	procUpdatePerm(v);
	
	if (crackerClient())
		sendPermsToCracker();
}

extern "C" bool GetUpdatePerm(void)
{
	return procUpdatePerm();
}

extern "C" void SetAddPerm(bool v)
{
	procAddPerm(v);
	
	if (crackerClient())
		sendPermsToCracker();
}

extern "C" bool GetAddPerm(void)
{
	return procAddPerm();
}

extern "C" void SetDelPerm(bool v)
{
	procDelPerm(v);
	
	if (crackerClient())
		sendPermsToCracker();
}

extern "C" bool GetDelPerm(void)
{
	return procDelPerm();
}

static int sendPermsToCracker(void)
{
	if (!crackerClient())
		return 0;

	Int perms = 0;

	if (!procAddPerm())
		perms |= A_PERM;
	if (!procDelPerm())
		perms |= D_PERM;
	if (!procUpdatePerm())
		perms |= U_PERM;

	prServerConn(PrCmd::sendPermsToCracker) << perms;

 	return prServerConn.result();
}

#if HAVE_SAVEMEM
extern "C" bool ProcSaveMem(void)
{
	return procSaveMem();
}
#endif

extern "C" UShort ProcUserId(void)
{
	return procUserId();
}

extern "C" UShort ProcUserGid(void)
{
	return procUserGid();
}

extern "C" Int ProcPid(void)
{
	return procPid();
}

extern "C" const char *ProcTtyName(void)
{
	return toCharPtr(procTtyName());
}

extern "C" const char *ProcTty(void)
{
	return toCharPtr(procTty());
}

extern "C" Int ProcSig(void)
{
	return procSig();
}

extern "C" char **ProcArgs(void)
{
	return procArgs();
}

extern "C" Int ProcNArgs(void)
{
	return argCnt();
}

extern "C" TIME ProcInitTime(void)
{
	return LTIMETOS(toInt(procInitTime()));
}

extern "C" DATE ProcInitDate(void)
{
	return LDATETOS(toInt(procInitDate()));
}
      
extern "C" char COMMA()
{
	return thousandSep();
}

extern "C" char DECPOINT()
{
	return decPoint();
}


#ifdef IXSCA
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
// Modificaciones para login con IxSCA. DA

// regresa el path completo del archivo .ixscarc del usuario
// $HOME/.ixscarc
// DA
String getIxscaFile() {

	String homeDir = String("HOME").getEnv();
	if (homeDir != NULL_STRING) {
		String path;
		path += homeDir;
		path += "/";
		path += IXSCAFILE;
		return path;	
	}
	
	// CARTEL
	return NULL_STRING;
}


// Retorna el nombre de host de la maquina
// DA
String getHostName() {
		char* name;
		gethostname(name, 100);
		return name;
}

// Retorna el hostid de la maquina
// DA
long getHostId() {
        return gethostid();
}


// Saco del .rc el ip del servidor.
// Si no puede obtener retorna null
// Si todo bien retorna el numero de ip del servidor
// DA
extern "C" String getHostFromFile()
{
	String rcFile, host;
        rcFile = getIxscaFile();
	if (rcFile == NULL_STRING) {
		return NULL_STRING;
	}

	if (!getVarFromRcFile(toCharPtr(rcFile), IXSCAHOST, host)) {
		return NULL_STRING;
	}
	return host;
}

// Saco del rc el puerto del servidor.
// Si no puede obtener retorna null
// Si todo bien retorna el numero de puerto del servidor
// DA
extern "C" int getPortFromFile()
{
	String rcFile, port;
        rcFile = getIxscaFile();
	
	if (rcFile == NULL_STRING) {
		return -1;
	}		

	if (!getVarFromRcFile(toCharPtr(rcFile), IXSCAPORT, port))
		return -1;
	return toInt(port);
}

// Obtengo el ip del servidor de la variable de ambiente IXSCAHOST
// Si no esta definida trata de sacarla del archivo .rc
// Si todo bien retorna el numero de ip del servidor
// DA
extern "C" String getServerHost() 
{
	String serverHost = String(IXSCAHOST).getEnv();
        if (serverHost != NULL_STRING) {
                return serverHost;
        } else {
		return getHostFromFile();
	}
}

// Obtengo el port del servidor de la variable de ambiente IXSCAPORT
// Si no esta definida trata de sacarla del archivo .rc
// Si todo bien retorna el numero de port del servidor
// DA
extern "C" int getServerPort() 
{
	String serverPort = String(IXSCAPORT).getEnv();
        if (serverPort != NULL_STRING) {
                return toInt(serverPort);
        } else {
		return getPortFromFile();
	}
}

extern "C" void ixscaSet(SNODE sNode) {
/*
	IxSCAsetSession(sNode.idSession);
	IxSCAsetIdUser(sNode.idUser);
	IxSCAsetProfile(sNode.profile);
	IxSCAsetLogin(sNode.login);
	IxSCAsetPassword(sNode.password);
	IxSCAsetHost(toCharPtr(getServerHost()));
	IxSCAsetPort(getServerPort());
*/
}


// @DIEGOS 2006-04-27 (11:44)
// Esta funcion la comente porque no utilizo estas variables
extern "C" int getSession() {
#if 0
	if (String("IxSCA_enabled").getEnv() == String("true")) { 
		if ((getServerHost() != NULL_STRING) && (getServerPort() != 0)) {
#endif

			// hay .ixscarc
			SNODE sNode = getSessionFromWM(getHostId());

			if (sNode.idSession == 0 || sNode.idSession == -1 || sNode.idSession == -2) {
				// no hay sesion wm
				WiMsg( "No sesion" );
				return 0;
			} else {
				// hay sesion en el WM
				// session != 1 y session != 0
				//
				ixscaSet(sNode);	
				return sNode.idSession;
	   	}
#if 0
		} else {
			// No hay .ixscarc
			WiMsg("ERROR: No se encuentra el archivo \".ixscarc\" o esta mal configurado!");
	       	return -1;
		}
	 } else {	
	 	WiMsg("IxSCA_enabled no seteada");
 		return -1;
 	}
#endif
}

// Trata de logear el inicio del programa
// si no puede retorna false
bool startProgram() {
	return IxSCAstartProgram() ;
}

// Metodo llamado desde wcmd
// retorna true si hay una session abierta
// en este caso guarda en el .rc los datos de la nueva sesion.
// retorna false si no se pudo conseguir ninguna sesion.
// 
extern "C" bool GetSession() {

	// Vio si fue iniciada una session
	// fprintf( stderr, "Si hay sesion abierta | HOME(%s)\n", getenv("HOME") ) ;
	if ( IxSCAisSessionOpened() ) {
		// fprintf( stderr, "hay sesion abierta\n" ) ;
		IxSCAdebug() ;
		return true ;
	}
	else { // Si no hay una sesion abierta
		// fprintf( stderr, "Init session\n" ) ;
		if ( !IxSCAinitSession() ) {
			// fprintf( stderr, "NO se pudo iniciar\n" ) ;
			Stop(1) ;
		}

		IxSCAdebug() ;
		return startProgram();

	} // Final 'Si no hay una sesion abierta

}

extern "C" void EndProgram() {
	IxSCAendProgram();
}

#endif

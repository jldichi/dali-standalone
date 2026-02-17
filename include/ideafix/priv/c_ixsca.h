// c_ixsca.h -- API SESSION CLIENT -- DA 05/03/2003

#ifndef C_IXSCA_H
#define C_IXSCA_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct perSec
{
	int pass_maxLength;
	int pass_minLength;
	int pass_allowNRep;
} perSec;

typedef struct perPass
{
	char passwd[20];
	int cdate;
	int ctime;
} perPass;

typedef struct passInfo
{
	int id;
	int rowCount;
} passInfo;

typedef struct dbase_s
{
	char	descrip[55];
	int	id;
} dbase_s;

// ---------------------------------------------------------------
// funciones de manejo de eventos

	extern int IxSCAdoLogin(char* login, char* password);
	extern int IxSCAstartSession(char* login, char* password);
	extern int IxSCAstartSessionPrg(char* login, char* password, char* programName);
	extern bool IxSCAisSessionOpened();
	extern bool IxSCAstartProgramByName(char* programName);
	extern bool IxSCAstartProgram();
	extern bool IxSCAchangeProgramByName(char* programName);
	extern bool IxSCAchangeProgram();
	extern bool IxSCAendProgramByName(char* programName);
	extern bool IxSCAendProgram();
	extern bool IxSCAaddSessionActivity(char* type, char* description, char* status);
	extern bool IxSCAhasAccess(char* programName, char* args);
	extern bool IxSCAendSessionById(int sessionId);
	extern bool IxSCAendSession();
	extern bool IxSCAclearSession();
	extern bool IxSCAisEnabled();
	extern void IxSCAstartFunction(char *funcName, char *comment);
	extern char* IxSCAgetUserAttribute(int userId, char* attName);
	extern bool IxSCAhasRole(int userId, char* rName);
	extern bool IxSCAchangeProfile(int userId, char* pName);
	extern int IxSCAgetProfileList(int userId, char** list);

// ------------------------------------------------------------------
// funciones de obtencion

	extern DATE IxSCAGetStartDate(int idSession);
	extern DATE IxSCAGetExpirationDate(int idSession);
	extern TIME IxSCAGetStartTime(int idSession);
	extern TIME IxSCAGetExpirationTime(int idSession);
	extern char* IxSCAgetMessage();
	extern char* IxSCAgetError();
	extern int IxSCAgetSecurity();

// ------------------------------------------------------------------
// funciones de seteo

	extern void IxSCAsetHost(char* host);
	extern void IxSCAsetPort(int port);
	extern void IxSCAsetLogin(char* login);
	extern void IxSCAsetPassword(char* password);
	extern void IxSCAsetSession(int session);
	extern const char* IxSCAgetHome();
	extern char* IxSCAFullUserName( int idu ) ;
	extern const char* IxSCAUserEmail( int idu ) ;
	extern char* IxSCAUserLogin( int idu ) ;

// ------------------------------------------------------------------
// funciones para debug y status

	extern char* IxSCAgetRequest();
	extern char* IxSCAgetResponse();
	extern void IxSCAdebugStatus();


// ------------------------------------------------------------------
// Observers sobre las entidades del IxSCA

	extern int IxSCAgetIdSession();
	extern const char* IxSCAgetProfile();
	extern const char *IxSCAgetUserName();
	extern const char *IxSCAgetUserLogin();
	extern char *IxSCAgetPassword();
	extern int IxSCAgetIdUser();
	extern const char *IxSCAgetProfile();
	extern char *IxSCAgetServerMessage();

	extern void IxSCAsetProfile( const char* profileName );

// ---------------------------------------------------------------------
// Los siguientes metodos, son solo de consulta no deben modificar el
// usuario actual que esta almacenado en el buffer.

	extern char *IxSCAgetUserNameById(int idUser);
	extern char *IxSCAgetLoginById(int idUser);
	extern char *IxSCAgetEmailById(int idUser);
	extern char *IxSCAisUserActiveById(int idUser);
	extern int IxSCAgetIdUserByName(char *userName);
	extern char *IxSCAgetProfileById(int idUser);
	extern char *IxSCAgetUserAttributeById(int idUser, char *attName);

// -------------------------------------------------
// Funciones para el logeo de sesiones

	extern bool IxSCAinitSession(); 


// -------------------------------------------------
// Funciones para el debug de la session

	extern void IxSCAdebug(); 


// -------------------------------------------------
// Funciones para el manejo de password

	extern char* IxSCAdoEncrypt( const char* pass ); 
	extern char* IxSCAdoUnEncrypt( const char* pass ); 

// -------------------------------------------------
// Funciones para el manejo de lista de usuarios

    extern int* IxSCAallocUserList();
    extern long IxSCAcountUserList();
    extern void IxSCAfreeUserList(int* list);

#ifdef  __cplusplus
}
#endif


#endif

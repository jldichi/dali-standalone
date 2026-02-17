#ifndef IXSCA_H
#define IXSCA_H

#include <ideafix/priv/ixclient.h>
#include <ideafix/priv/debuger.h>
#include <ideafix/priv/ixlist.h>

struct ixscaData
{
	IxClient *ixClient;
	int idSession;
	int idUser;
	String login;
	String password;
	int idSystem;
	int idModule;
	int idUnit;
	int idFunction;
	int idProfile;
	String profileName;
	String startTime;
	String startDate;
	String expirationTime;
	String expirationDate;
	String error;
	String message;
	String request;
	String response;
	String attributeValue;
	IxList list;	
};

class IxSCA {

private:
	ixscaData data;
	ixscaData backUpData;
	Debuger debuger;
	
	bool doProgram(String programName, String type, String PName, int PId);
	bool hasAccess(String type, String values[]);

public:
	// geters...
	int getIdSession() { return data.idSession; };
	int getIdUser() { return data.idUser; };
	String getLogin() { return data.login; };
	String getPassword() { return data.password; };
	int getIdSystem() { return data.idSystem; };
	int getIdModule() { return data.idModule; };
	int getIdUnit() { return data.idUnit; };	
	int getIdFunction() { return data.idFunction; };
	int getIdProfile() { return data.idProfile; };
	const char* getProfileName() { return data.profileName ? toCharPtr(data.profileName) : ""; };
	String getStartTime() { return data.startTime; };
	String getExpirationTime() { return data.expirationTime; };
	String getStartDate() { return data.startDate; };
	String getExpirationDate() { return data.expirationDate; };
	String getError() { return data.error; };
	String getMessage() { return data.message; };
	String getAttributeValue() { return data.attributeValue; };
	IxList getList() { return data.list; };
	IxClient* getIxClient() { return data.ixClient; };
	String getHost() { return getIxClient()->getHost(); };
	int getPort() { return getIxClient()->getPort(); };       

	
	// seters...
	void setIdSession(int id) { data.idSession = id; };
	void setIdUser(int id) {
		data.idUser = id;
		String ixIdUser("IxSCA_idUser");
		ixIdUser = ixIdUser + "=" + toString(id);
                debuger.postError("Seting IxSCA_idUser...");
                debuger.postError(toCharPtr(ixIdUser));
		ixIdUser.putEnv();
	};
	void setLogin(String lin) {
		data.login = lin;
		String ixUserName("IxSCA_UserName");
                String log(lin);
                log =  ixUserName + "=" + log;
                debuger.postError("Seting IxSCA_UserName...");
                debuger.postError(toCharPtr(log));
                log.putEnv();
	 };
	void setPassword(String pass) { data.password = pass; };
	void setIdSystem(int id) { data.idSystem = id; };
	void setIdModule(int id) { data.idModule = id; };
	void setIdUnit(int id) { data.idUnit = id; };	
	void setIdFunction(int id) { data.idFunction = id; };
	void setIdProfile(int id) {  data.idProfile = id; };
	void setProfileName(String profile) {
		data.profileName = profile;
	        String ixProfileName("IxSCA_profileName");
                String prof(profile);
		prof =  ixProfileName + "=" + prof;
                debuger.postError("Seting IxSCA_profileName...");
                debuger.postError(toCharPtr(prof));
                prof.putEnv();
	};
	void setStartTime(String sTime) { data.startTime = sTime; };
	void setExpirationTime(String eTime) { data.expirationTime = eTime; };
	void setStartDate(String sDate) { data.startDate = sDate; };
	void setExpirationDate(String eDate) { data.expirationDate = eDate; };
	void setError(String er) { data.error = er; };
	void setMessage(String msg) { data.message = msg; };
	void setAttributeValue(String val) { data.attributeValue = val; };
	void setList(IxList l) { data.list = l; };

        void setHost(String h) {getIxClient()->setHost(h);};
        void setPort(int p) {getIxClient()->setPort(p);};
	
	// debug
	String getRequest() {
		data.request = getIxClient()->getRequest();
		debuger.postError("GetRequest");		
		debuger.postError(toCharPtr(data.request));
		debuger.postError("EndGetRequest %d", data.request.length());
		debuger.postError("EndGetRequest");
		return data.request;
	};
	String getResponse() {
		data.response = toCharPtr(getIxClient()->getResponse());
		debuger.postError("GetResponse");		
		debuger.postError(toCharPtr(data.response));
		debuger.postError("EndGetResponse");
		return data.response;
	};
	
	void debugStatus() {
		debuger.postError("\nIXSCA STATUS");
		debuger.postError("\tIdSession = %d", data.idSession);	
		debuger.postError("\tIdUser = %d", data.idUser);
		debuger.postError("\tLogin = %s", data.login ? toCharPtr(data.login) : "");
		debuger.postError("\tPassword = %s", data.password ? toCharPtr(data.password) : "");
		debuger.postError("\tIdSystem = %d", data.idSystem);	
		debuger.postError("\tIdModule = %d", data.idModule);		
		debuger.postError("\tIdUnit = %d", data.idUnit);		
		debuger.postError("\tIdFunction = %d", data.idFunction);		
		debuger.postError("\tIdProfile = %d", data.idProfile);	
		debuger.postError("\tNameProfile = %s", data.profileName ? toCharPtr(data.profileName) : "");	
		debuger.postError("\tExpirationTime = %s", data.expirationTime ? toCharPtr(data.expirationTime) : "");		
		debuger.postError("\tExpirationDate = %s", data.expirationDate ? toCharPtr(data.expirationDate) : "");		
		debuger.postError("\tError = %s", data.error ? toCharPtr(data.error) : "");		
		debuger.postError("\tMessage = %s", data.message ? toCharPtr(data.message) : "");		
		debuger.postError("END STATUS\n");		
	};

	IxSCA() {
		data.ixClient = new IxClient();
		data.idSession = 0;
		data.idUser = 0;
		data.login = "";
		data.password = "";
		data.idSystem = 0;
		data.idModule = 0;
		data.idUnit = 0;
		data.idFunction = 0;
		data.idProfile = 0;
		data.profileName = "";
		data.expirationTime = "";
		data.expirationDate = "";
		data.error = "";
	};
	IxSCA(String h){
		setHost(h);
		IxSCA();
	};

	IxSCA(String h, int p){
		setHost(h);
		setPort(p);
		IxSCA();
	};

	void saveData() {
		backUpData = data;
	};
	void restoreData() {
		data = backUpData;
		
		// se hace el seteo para que se actualizen las variables de ambiente.
		setIdUser(getIdUser());
		setLogin(getLogin());
		setProfileName(getProfileName());
	};

	int startSession(String login, String password, String tty, String ip);
	int startSession(String login, String password, String tty);
	int startSession(String login, String password);
	int startSession();
	bool endSession();
	bool endSession(int sessionId);
	bool startProgram(String programName, String PName, int PId); 
	bool changeProgram(String programName, String PName, int PId); 
	bool endProgram(String programName, String PName, int PId); 
	bool addSessionActivity(String type, String description, String status, String PName, int PId);
	bool hasAccess(String programName, String args);
	bool hasAccess(int idSystem, int idModule, int idUnit, String args);
	bool getuserAttributeById(String attName);
	bool getuserAttributeById(int userId, String attName);
	bool getUserById(int userId, String values[]);
	char* getUserNameById(int userId);
	char* getUserLoginById(int userId);
	char* getUserEmailById(int userId);
	char* isUserActiveById(int userId);
	bool hasRole(String roleName);
	bool hasRole(int userId, String roleName);
	bool changeProfile(String profileName);
	bool changeProfile(int userId, String profileName);
	int getProfileList(char** list);
	int getProfileList(int userId, char** list);
	bool doLogin(String login, String password);
};

#endif

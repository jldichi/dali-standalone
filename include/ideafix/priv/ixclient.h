#define IXCLIENT_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>

#define LINESIZE 4096
#define SERVER_PORT 8080

class IxClient
{

private:
	// variables para la conexion
	int sock;
	struct sockaddr_in server;
	struct hostent *host_info;
	bool connected;
	bool loged;
	String host;
	int port;
	
	// variables del cliente
	String* arrayValue;
	int integerValue;
	String stringValue;
	bool booleanValue;
	int idSystem;
	int idModule;
	int idUnit;
	int idFunction;

	// variables para debug
	Debuger debuger;
	String request;
	String response;

	// funciones para la conexion
	bool connection(const String);
	bool connection(const String, int);
	void disconnect(); 
	bool send(char*, String*);
	bool send(char*, String*, char*);
	bool send(char*, String*, char*, int);
	int Readline(int sockd, char *vptr, int maxlen);
	String ReadXML(int sock);
	bool processEvent(XMLHandler* handler, String type, String values[]);

public:
	IxClient::IxClient() {
        	connected = false;
		loged = false;
		host = "NULL";
		port = SERVER_PORT;
	}
	IxClient::IxClient(String h ) {
        	connected = false;
		loged = false;
		host = h;
		port = SERVER_PORT;
	}
	IxClient::IxClient(String h, int p) {
        	connected = false;
		loged = false;
		host = h;
		port = p;
	}
	
	IxClient::~IxClient() {
        	if (connected)
                close(sock);
	}

	void setHost(String h) {host = h;};
	void setPort(int p) {port = p;};
	
	bool doEvent(String type, String values[]);
	
	String* getArrayValue() {return arrayValue;};
	int getIntegerValue() {return integerValue;};
	String getStringValue() {return stringValue;};
	bool getBooleanValue() {
		debuger.postError("บบบบบบบบบบบบบบบalfa");
		return booleanValue;
	};
	int getIdSystem() {return idSystem;};
	int getIdModule() {return idModule;};
	int getIdUnnit() {return idUnit;};	
	int getIdFunction() {return idFunction;};
	bool isLoged() {return loged;};
	bool isConnected() {return connected;};
	String getHost() {return host;};
	int getPort() {return port;};

	
	void setArrayValue(String* array) {arrayValue = array;};
	void setIntegerValue(int i) {integerValue = i;};
	void setStringValue(String s) {stringValue = s;};
	void setBooleanValue(bool b) {booleanValue = b;};
	void setIdSystem(int id) {idSystem = id;};
	void setIdModule(int id) {idModule = id;};
	void setIdUnnit(int id) {idUnit = id;};	
	void setIdFunction(int id) {idFunction = id;};
	void setLoged(bool b) {loged = b;};
	void setConnected(bool b) {connected = b;};
	

	// debug
	void setRequest(String req) {request = req;};
	void setResponse(String res) {response = res;};
	String getRequest() {return request;};
	String getResponse() {return response;};

};

#endif

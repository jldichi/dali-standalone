#ifndef XMLHANLDER_H
#define XMLHANLDER_H

#include <ideafix/priv/ErrorReporter.h>
#include <stdio.h>
#include <ideafix.h>
#include <ideafix/priv/debuger.h>
#include <ideafix/priv/URLEncode.h>

#include <iostream>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>


#define LOADUSER        "LoadUser"
#define STARTSESSION    "StartSession"
#define ENDSESSION      "EndSession"
#define STARTPROGRAM    "StartProgram"
#define CHANGEPROGRAM   "ChangeProgram"
#define ENDPROGRAM      "EndProgram"
#define ADDSESSIONAC    "AddSessionAc"
#define HASACCESSBYNAME "HasAccessByName"
#define HASACCESSBYID   "HasAccessById"
#define GETATTRIBUTE	"GetUserAttribute"
#define HASROLE		"HasRole"
#define CHANGEPROFILE	"ChangeProfile"
#define GETPROFILELIST	"GetProfileList"
#define GETUSER		"GetUser"

// funciones auxiliares
#define	C2X(str) 	XMLString::transcode(str)
#define	X2C(str) 	XMLString::transcode(str)
#define	S2X(str) 	XMLString::transcode(toCharPtr(str))
#define	X2S(str) 	String(XMLString::transcode(str))
#define	S2C(str) 	toCharPtr(str) 
	
class XMLHandler
{

private:

public:
	Debuger debuger;
	XMLHandler::XMLHandler();
	XMLHandler::~XMLHandler();

	virtual DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]){}
	DOMDocument* createXML(char* name);
	DOMDocument* addUserObject(DOMDocument* doc, char* name, char* classPath);
	DOMDocument* infoToXML(DOMDocument* doc, char* type, int idUser, int idSystem, int idModule, int idUnit);
	DOMDocument* charToXML(String xml);
	char* XMLToChar(DOMDocument* doc);
	virtual bool XMLToValues(DOMDocument* doc,  String type, String values[]){}
	int countChildElements(DOMNode *n);
	DOMNode* getChild(DOMNode *n, char* name);
	String getStringValue(DOMElement* rootElem);
	String getChildValue(DOMElement* rootElem, String elem);
};

#endif

#ifndef STARTSESSIONEVENT_H
#define STARTSESSIONEVENT_H

#include "XMLHandler.h"

class StartSessionEvent: public XMLHandler
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc, String type, String values[]);
	String getIdSession(DOMElement* rootElem);
	void StartSessionEvent::getDateTimeValues(DOMElement* rootElem, String values[]);
};

#endif

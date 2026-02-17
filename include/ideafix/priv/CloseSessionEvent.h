#ifndef CLOSESESSIONEVENT_H
#define CLOSESESSIONEVENT_H

#include "XMLHandler.h"

class CloseSessionEvent: public XMLHandler
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc,  String type, String values[]);
};

#endif

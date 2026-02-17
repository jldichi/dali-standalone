#ifndef GETUSEREVENT_H
#define GETUSEREVENT_H

#include "XMLHandler.h"

class GetUserEvent: public XMLHandler
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc, String type, String values[]);
};

#endif

#ifndef HASROLEEVENT_H
#define HASROLEEVENT_H

#include "XMLHandler.h"

class HasRoleEvent: public XMLHandler
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc, String type, String values[]);
};

#endif

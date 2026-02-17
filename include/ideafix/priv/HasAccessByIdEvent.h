#ifndef HASACCESSBYIDEVENT_H
#define HASACCESSBYIDEVENT_H

#include "XMLHandler.h"

class HasAccessByIdEvent: public XMLHandler
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc, String type, String values[]);
};

#endif

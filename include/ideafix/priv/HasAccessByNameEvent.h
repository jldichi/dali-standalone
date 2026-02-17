#ifndef HASACCESSBYIDNAMEEVENT_H
#define HASACCESSBYIDNAMEEVENT_H

#include "XMLHandler.h"

class HasAccessByNameEvent: public XMLHandler
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc, String type, String values[]);
};

#endif

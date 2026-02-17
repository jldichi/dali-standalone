#ifndef GETUSERATTREVENT_H
#define GETUSERATTREVENT_H

#include "XMLHandler.h"

class GetUserAttrEvent: public XMLHandler
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc, String type, String values[]);
};

#endif

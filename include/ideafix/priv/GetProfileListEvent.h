#ifndef GETPROFILELISTEVENT_H
#define GETPROFILELISTEVENT_H

#include "XMLHandler.h"

class GetProfileListEvent: public XMLHandler
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc, String type, String values[]);
};

#endif

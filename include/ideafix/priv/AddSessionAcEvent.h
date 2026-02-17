#ifndef ADDSESSIONACEVENT_H
#define ADDSESSIONACEVENT_H

#include "XMLHandler.h"

class AddSessionAcEvent: public XMLHandler
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc, String type, String values[]);
};

#endif

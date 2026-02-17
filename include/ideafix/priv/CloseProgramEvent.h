#ifndef CLOSEPROGRAMEVENT_H
#define CLOSEPROGRAMEVENT_H

#include "XMLHandler.h"

class CloseProgramEvent: public XMLHandler
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc,  String type, String values[]);
};

#endif

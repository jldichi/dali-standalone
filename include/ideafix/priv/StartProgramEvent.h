#ifndef STARTPROGRAMEVENT_H
#define STARTPROGRAMEVENT_H

#include "XMLHandler.h"

class StartProgramEvent: public XMLHandler
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc, String type, String values[]);
};

#endif

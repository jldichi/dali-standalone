#ifndef CHANGEPROGRAMEVENT_H
#define CHANGEPROGRAMEVENT_H

#include "XMLHandler.h"

class ChangeProgramEvent: public XMLHandler 
{

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc,  String type, String values[]);
};

#endif

#ifndef LOADUSEREVENT_H
#define LOADUSEREVENT_H

#include "XMLHandler.h"

class LoadUserEvent: public XMLHandler {

public:
	DOMDocument* valueToXML(DOMDocument* doc, String type, String value[]);
	bool XMLToValues(DOMDocument* doc,  String type, String values[]);
};

#endif

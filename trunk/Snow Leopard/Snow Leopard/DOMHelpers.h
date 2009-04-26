#ifndef DOMHELPERS_H_
#define DOMHELPERS_H_

#include <string>
#include "xerces.h"

static double getAttributeDouble(std::string attributeName,xerces DOMNamedNodeMap* attributes)
{
	return atof(XercesString(attributes->getNamedItem(XercesString(attributeName).xmlCh())->getNodeValue()).stdStr().c_str());
}

static int getAttributeInt(std::string attributeName,xerces DOMNamedNodeMap* attributes)
{
	return atoi(XercesString(attributes->getNamedItem(XercesString(attributeName).xmlCh())->getNodeValue()).stdStr().c_str());
}
static unsigned int getAttributeUnsignedInt(std::string attributeName,xerces DOMNamedNodeMap* attributes)
{
	return atoi(XercesString(attributes->getNamedItem(XercesString(attributeName).xmlCh())->getNodeValue()).stdStr().c_str());
}
static std::string getAttributeStr(std::string attributeName,xerces DOMNamedNodeMap* attributes)
{
	return XercesString(attributes->getNamedItem(XercesString(attributeName).xmlCh())->getNodeValue()).stdStr();
}
static bool getAttributeBool(std::string attributeName,xerces DOMNamedNodeMap* attributes)
{
	if (XercesString(attributes->getNamedItem(XercesString(attributeName).xmlCh())->getNodeValue()).stdStr()=="true")
		return true;
	else if (XercesString(attributes->getNamedItem(XercesString(attributeName).xmlCh())->getNodeValue()).stdStr()=="false")
		return false;
	else throw new std::exception("boolean value not true or false");
}


#endif
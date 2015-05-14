#ifndef GRAPH_EXCHANGER_UTILS_H
#define GRAPH_EXCHANGER_UTILS_H

#include <xercesc\util\PlatformUtils.hpp>
#include <xercesc\dom\DOM.hpp>
#include <xercesc\framework\LocalFileFormatTarget.hpp>
#include <xercesc\parsers\XercesDOMParser.hpp>
#include <xercesc\sax\HandlerBase.hpp>
XERCES_CPP_NAMESPACE_USE

#include "Node.h"

namespace msonlab { namespace graph {
	virtual DOMElement* serializeNode(const NodePtr node, DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey);
}}

#endif
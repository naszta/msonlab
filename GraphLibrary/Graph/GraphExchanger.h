#ifndef GRAPH_EXCHANGER 
#define GRAPH_EXCHANGER

#include <xercesc\dom\DOM.hpp>
#include <xercesc\framework\LocalFileFormatTarget.hpp>
#include <xercesc\parsers\XercesDOMParser.hpp>
#include <xercesc\sax\HandlerBase.hpp>
#include <xercesc\util\PlatformUtils.hpp>
XERCES_CPP_NAMESPACE_USE

#include "../Global.h"
#include "Graph.h"
#include "GraphBuilder.h"
#include "PersistenceUtils.h"

namespace msonlab
{
	class GraphExchanger
	{
	private:
		bool outputXML(DOMDocument* myDOMDocument, std::string filePath) const;
		void createKeys(DOMDocument* inDocument) const;
		DOMElement* serializeNode(const NodePtr node, DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey) const;
		DOMElement* serializeEdge(const EdgePtr edge, DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey) const;

	public:
		static GraphPtr ImportGraph(std::string inputPath);
		bool ExportGraph(const Graph& graph, std::string outputPath) const;
	};
}
#endif
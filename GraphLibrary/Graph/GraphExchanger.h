#ifndef GRAPHLIB_GRAPH_EXCHANGER 
#define GRAPHLIB_GRAPH_EXCHANGER

#include <string>

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

namespace msonlab { namespace persistence {


	class GraphExchanger
	{
	public:
		static GraphPtr ImportGraph(std::string inputPath);
		bool ExportGraph(const Graph& graph, std::string outputPath) const;
	};
} }
#endif
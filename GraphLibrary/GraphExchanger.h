#ifndef GRAPH_EXCHANGER 
#define GRAPH_EXCHANGER

#include "Graph.h"
#include "Global.h"
#include "GraphBuilder.h"

#include <xercesc\util\PlatformUtils.hpp>
#include <xercesc\dom\DOM.hpp>
#include <xercesc\framework\LocalFileFormatTarget.hpp>
#include <xercesc\parsers\XercesDOMParser.hpp>
#include <xercesc\sax\HandlerBase.hpp>
XERCES_CPP_NAMESPACE_USE

namespace msonlab
{
	class GraphExchanger
	{
	private:
		//msonlab::GraphPtr graph;

		bool outputXML(DOMDocument* myDOMDocument, std::string filePath) const;
		void createKeys(DOMDocument* inDocument) const;
		DOMElement* serializeNode(const NodePtr node, DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey) const;
		DOMElement* serializeEdge(const EdgePtr edge, DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey) const;

	public:
		//GraphExchanger(msonlab::GraphPtr toExchange);

		enum supportedNodeType
		{
			ADD,
			CONSTANT,
			DIVIDE,
			MULTIPLY,
			SQUAREROOT
		};
		static std::string getSupportedNodeTypeName(supportedNodeType nodeType)
		{
			switch (nodeType)
			{
			case msonlab::GraphExchanger::ADD:
				return "ADD";
				break;
			case msonlab::GraphExchanger::CONSTANT:
				return "CONSTANT";
				break;
			case msonlab::GraphExchanger::DIVIDE:
				return "DIVIDE";
				break;
			case msonlab::GraphExchanger::MULTIPLY:
				return "MULTIPLY";
				break;
			case msonlab::GraphExchanger::SQUAREROOT:
				return "SQUAREROOT";
				break;
			default:
				throw new Exceptions::UnknownNodeTypeException("The given node type is unkown. Please double-check the spelling and note that the name is case-sensitve.");
			}
		}

		enum supportedEdgeType
		{
			SIMPLE,
			BLUE,
			MULTIPLIER
		};
		static std::string getSupportedEdgeTypeName(supportedEdgeType edgeType)
		{
			switch (edgeType)
			{
			case msonlab::GraphExchanger::SIMPLE:
				return "SIMPLE";
				break;
			case msonlab::GraphExchanger::BLUE:
				return "BLUE";
				break;
			case msonlab::GraphExchanger::MULTIPLIER:
				return "MULTIPLIER";
				break;
			default:
				throw new Exceptions::UnknownEdgeTypeException("The given edge type is unkown. Please double-check the spelling and note that the name is case-sensitve.");
				break;
			}
		}

		static GraphPtr ImportGraph(std::string inputPath);
		bool ExportGraph(const Graph& graph, std::string outputPath) const;
	};
}
#endif
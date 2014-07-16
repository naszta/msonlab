#pragma once

#include "Graph.h"
#include "Global.h"
#include "GraphBuilder.h"



namespace msonlab
{
	class GraphExchanger
	{
	private:
		msonlab::Graph::gPtr graph;

		bool outputXML(DOMDocument* myDOMDocument, std::string filePath) const;

		void createKeys(DOMDocument* inDocument) const;

	public:
		GraphExchanger(msonlab::Graph::gPtr toExchange);

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


		static Graph::gPtr ImportGraph(std::string inputPath);
		bool ExportGraph(std::string outputPath) const;


		
	};
}
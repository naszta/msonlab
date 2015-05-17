#ifndef GRAPHLIB_GRAPH_PERSISTENCEEUTILS_H
#define GRAPHLIB_GRAPH_PERSISTENCEUTILS_H

#include <string>

namespace msonlab { namespace persistence {

	enum SupportedNodeType
	{
		ADD,
		CONSTANT,
		DIVIDE,
		MULTIPLY,
		SQUAREROOT
	};

	enum SupportedEdgeType
	{
		SIMPLE,
		BLUE,
		MULTIPLIER
	};

	std::string edgeTypeToString(SupportedEdgeType type_);
	std::string edgeTypeToColor(SupportedEdgeType type_);
	std::string nodeTypeToString(SupportedNodeType type_);
	std::string nodeTypeToColor(SupportedNodeType type_);
}}

#endif
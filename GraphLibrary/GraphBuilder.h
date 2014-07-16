#pragma once

#include "Graph.h"
#include "Global.h"
#include "NodeAdd.h"
#include "NodeConstant.h"
#include "NodeDivide.h"
#include "NodeMultiply.h"
#include "NodeSquareRoot.h"
#include "Edge.h"
#include "BlueEdge.h"
#include "MultiplierEdge.h"

namespace msonlab
{
	class GraphBuilder
	{
		static std::map<unsigned int, Node::nPtr> nodes;
		static std::map<unsigned int, Edge::ePtr> edges;

	public:

		static void add_node(unsigned int id, std::string node_type, std::string label, std::string custom_data);
		static void add_edge(unsigned int id, unsigned int source_id, unsigned int target_id, std::string edge_type, std::string custom_data);

		static void reset();
		static Graph::gPtr build();

	};

}
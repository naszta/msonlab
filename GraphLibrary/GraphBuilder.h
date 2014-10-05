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
		static std::map<unsigned int, NodePtr> nodes;
		static std::map<unsigned int, EdgePtr> edges;

	public:

		static void add_node(unsigned int id, std::string node_type, types::LabelType label, std::string custom_data);
		static void add_edge(unsigned int id, unsigned int source_id, unsigned int target_id, std::string edge_type, std::string custom_data);

		static void reset();
		static GraphPtr build();

	};

}
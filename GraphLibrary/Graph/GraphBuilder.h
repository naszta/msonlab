#ifndef GRAPH_BUILDER 
#define GRAPH_BUILDER

#include <string>

#include "../Global.h"

#include <string>

#include "Edge.h"
#include "Function.h"
#include "Graph.h"
#include "Node.h"


namespace msonlab
{
	class GraphBuilder
	{
		static std::map<unsigned int, NodePtr> nodes;
		static std::map<unsigned int, EdgePtr> edges;

	public:

		NodeId addNode(types::LabelType label, std::string _functiontype, graph::function::functiondata _value = 0);
		EdgeId addEdge(NodeId from, NodeId to, types::LabelType label, std::string _edgetype);

		//Graph build();

		static void add_node(unsigned int id, std::string node_type, types::LabelType label, std::string custom_data);
		static void add_edge(unsigned int id, unsigned int source_id, unsigned int target_id, std::string edge_type, std::string custom_data);

		static void reset();
		static Graph build();

	};
}
#endif
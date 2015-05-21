#ifndef GRAPH_BUILDER 
#define GRAPH_BUILDER

#include <string>

#include "../Global.h"
#include "Edge.h"
#include "Graph.h"
#include "Node.h"


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
		static Graph build();

	};
}
#endif
#include "GraphBuilder.h"


namespace msonlab
{
	std::map<unsigned int, Node::nPtr> GraphBuilder::nodes;
	std::map<unsigned int, Edge::ePtr> GraphBuilder::edges;

	void GraphBuilder::add_node(unsigned int id, std::string node_type, std::string label, std::string custom_data)
	{
		if (node_type == std::string("ADD"))
		{
			GraphBuilder::nodes.insert(std::pair<unsigned int, Node::nPtr>
				(id,
				Node::nPtr(new NodeAdd(id, label, nullptr))
				));
		}
		if (node_type == std::string("CONSTANT"))
		{
			GraphBuilder::nodes.insert(std::pair<unsigned int, Node::nPtr>
				(id,
				Node::nPtr(new NodeConstant(id, label, Types::DataType(new double(atof(custom_data.c_str())))))
				));
		}
		if (node_type == std::string("DIVIDE"))
		{
			GraphBuilder::nodes.insert(std::pair<unsigned int, Node::nPtr>
				(id,
				Node::nPtr(new NodeDivide(id, label, nullptr))
				));
		}
		if (node_type == std::string("MULTIPLY"))
		{
			GraphBuilder::nodes.insert(std::pair<unsigned int, Node::nPtr>
				(id,
				Node::nPtr(new NodeMultiply(id, label, nullptr))
				));
		}
		if (node_type == std::string("SQUAREROOT"))
		{
			GraphBuilder::nodes.insert(std::pair<unsigned int, Node::nPtr>
				(id,
				Node::nPtr(new NodeMultiply(id, label, nullptr))
				));
		}
	}

	void GraphBuilder::add_edge(unsigned int id, unsigned int source_id, unsigned int target_id, std::string edge_type, std::string custom_data)
	{
		if (edge_type == std::string("SIMPLE"))
		{
			GraphBuilder::edges.insert(std::pair<unsigned int, Edge::ePtr>
				(id,
				Edge::ePtr(new Edge(id, std::string(""), 0, nodes.at(source_id), nodes.at(target_id)))
				));
		}
		if (edge_type == std::string("BLUE"))
		{
			GraphBuilder::edges.insert(std::pair<unsigned int, Edge::ePtr>
				(id,
				Edge::ePtr(new BlueEdge(id, std::string(""), 0, nodes.at(source_id), nodes.at(target_id)))
				));
		}
		if (edge_type == std::string("MULTIPLIER"))
		{
			GraphBuilder::edges.insert(std::pair<unsigned int, Edge::ePtr>
				(id,
				Edge::ePtr(new MultiplierEdge(id, std::string(""), 0, nodes.at(source_id), nodes.at(target_id), atof(custom_data.c_str())))
				));
		}
	}


	void GraphBuilder::reset()
	{
		GraphBuilder::nodes.clear();
		GraphBuilder::edges.clear();
	}

	Graph::gPtr GraphBuilder::build()
	{
		Graph::gPtr ret(new Graph());
		for (auto e : GraphBuilder::edges)
		{
			ret->addEdge(e.second);
		}

		return ret;
	}



}
#include "GraphBuilder.h"


namespace msonlab
{
	std::map<unsigned int, NodePtr> GraphBuilder::nodes;
	std::map<unsigned int, EdgePtr> GraphBuilder::edges;

	using namespace types;

	void GraphBuilder::add_node(unsigned int id, std::string node_type, types::LabelType label, std::string custom_data)
	{
		if (node_type == std::string("ADD"))
		{
			GraphBuilder::nodes.insert(std::pair<unsigned int, NodePtr>
				(id,
				NodePtr(new NodeAdd(id, label, nullptr))
				));
		}
		if (node_type == std::string("CONSTANT"))
		{
			DataPtr data = std::make_shared<DataType>(atof(custom_data.c_str()));
			GraphBuilder::nodes.insert(std::pair<unsigned int, NodePtr>
				(id, NodePtr(new NodeConstant(id, label, data))));
		}
		if (node_type == std::string("DIVIDE"))
		{
			GraphBuilder::nodes.insert(std::pair<unsigned int, NodePtr>
				(id,
				NodePtr(new NodeDivide(id, label, nullptr))
				));
		}
		if (node_type == std::string("MULTIPLY"))
		{
			GraphBuilder::nodes.insert(std::pair<unsigned int, NodePtr>
				(id,
				NodePtr(new NodeMultiply(id, label, nullptr))
				));
		}
		if (node_type == std::string("SQUAREROOT"))
		{
			GraphBuilder::nodes.insert(std::pair<unsigned int, NodePtr>
				(id,
				NodePtr(new NodeMultiply(id, label, nullptr))
				));
		}
	}

	void GraphBuilder::add_edge(unsigned int id, unsigned int source_id, unsigned int target_id, std::string edge_type, std::string custom_data)
	{
		if (edge_type == std::string("SIMPLE"))
		{
			GraphBuilder::edges.insert(std::pair<unsigned int, EdgePtr>
				(id,
				EdgePtr(new Edge(id, L"", 0, nodes.at(source_id), nodes.at(target_id)))
				));
		}
		if (edge_type == std::string("BLUE"))
		{
			GraphBuilder::edges.insert(std::pair<unsigned int, EdgePtr>
				(id,
				EdgePtr(new BlueEdge(id, L"", 0, nodes.at(source_id), nodes.at(target_id)))
				));
		}
		if (edge_type == std::string("MULTIPLIER"))
		{
			GraphBuilder::edges.insert(std::pair<unsigned int, EdgePtr>
				(id,
				EdgePtr(new MultiplierEdge(id, L"", 0, nodes.at(source_id), nodes.at(target_id), atof(custom_data.c_str())))
				));
		}
	}


	void GraphBuilder::reset()
	{
		GraphBuilder::nodes.clear();
		GraphBuilder::edges.clear();
	}

	GraphPtr GraphBuilder::build()
	{
		GraphPtr ret = std::make_unique<Graph>();
		for (auto e : GraphBuilder::edges)
		{
			ret->addEdge(e.second);
		}

		return ret;
	}



}
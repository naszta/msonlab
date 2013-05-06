#pragma once
#include "Graph.h"

namespace msonlab
{
	Graph::Graph()
	{

	}

	Graph::Graph(const Graph& other)
	{

	}

	bool Graph::addNode(Node::nPtr toAdd)
	{
		return false;
	}

	bool Graph::addEdge(Edge::ePtr toAdd)
	{
		return false;
	}

	size_t Graph::numberOfNodes() const
	{
		return 0;
	}

	size_t Graph::numberOfEdges() const
	{
		return 0;
	}

	size_t Graph::numberOfOutputs() const
	{
		return 0;
	}

	bool Graph::importGraph(std::istream &in) const
	{
		return false;
	}

	bool Graph::exportGraph(std::ostream &out) const
	{
		return false;
	}


}
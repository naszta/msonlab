#pragma once
#include "Graph.h"

namespace msonlab
{
	Graph::Graph()
	{
		throw Exceptions::NotImplementedException("Graph default constructor");
	}

	Graph::Graph(const Graph& other)
	{
		throw Exceptions::NotImplementedException("Graph copy constructor");
	}

	bool Graph::addNode(Node::nPtr toAdd)
	{
		throw Exceptions::NotImplementedException("Graph::addNode function");
	}

	bool Graph::addEdge(Edge::ePtr toAdd)
	{
		throw Exceptions::NotImplementedException("Graph::addEdge function");
	}

	size_t Graph::numberOfNodes() const
	{
		throw Exceptions::NotImplementedException("Graph::numberOfNodes function");
	}

	size_t Graph::numberOfEdges() const
	{
		throw Exceptions::NotImplementedException("Graph::numberOfEdges function");
	}

	size_t Graph::numberOfOutputs() const
	{
		throw Exceptions::NotImplementedException("Graph::numberOfOutputs function");
	}

	bool Graph::importGraph(std::istream &in) const
	{
		throw Exceptions::NotImplementedException("Graph::importGraph function");
	}

	bool Graph::exportGraph(std::ostream &out) const
	{
		throw Exceptions::NotImplementedException("Graph::exportGraph function");
	}


}
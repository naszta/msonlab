#pragma once
#include "Graph.h"


namespace msonlab
{
	Graph::Graph()
	{
		nodes.clear();
		edges.clear();
	}

	Graph::Graph(const Graph& other)
	{
		throw Exceptions::NotImplementedException("Graph copy constructor");
	}

	bool Graph::addNode(Node::nPtr toAdd)
	{
		msonlab::IProcessable::pVect::iterator it;
		it = std::find(nodes.begin(),nodes.end(),toAdd);

		if (it != nodes.end())
			return false;

		it = nodes.insert(nodes.end(),toAdd);

		if (*it == toAdd)
			return true;
		else
		{
			throw Exceptions::FailedToAddNodeException("Failed to add the node to the graph!");
			return false;
		}
	}

	bool Graph::addEdge(Edge::ePtr toAdd)
	{
		msonlab::IProcessable::pVect::iterator it;
		it = std::find(edges.begin(),edges.end(),toAdd);

		if (it != edges.end())
			return false;

		msonlab::Node::nPtr fromNode = (*toAdd).getFrom();
		addNode(fromNode);
		msonlab::Node::nPtr toNode = (*toAdd).getTo();
		addNode(toNode);

		it = edges.insert(edges.end(),toAdd);

		if (*it == toAdd)
			return true;
		else
		{
			throw Exceptions::FailedToAddEdgeException("Failed to add the edge to the graph!");
			return false;
		}
	}

	size_t Graph::numberOfNodes() const
	{
		return nodes.size();
	}

	size_t Graph::numberOfEdges() const
	{
		return edges.size();
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
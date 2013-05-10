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
		{
			(*fromNode).registerSuccessor(toAdd);
			(*toNode).registerPredecessor(toAdd);
			return true;
		}
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

	bool Graph::importGraph(std::istream &in) const
	{
		throw Exceptions::NotImplementedException("Graph::importGraph function");
	}

	bool Graph::exportGraph(std::ostream &out) const
	{
		throw Exceptions::NotImplementedException("Graph::exportGraph function");
	}

	IProcessable::pVect Graph::getInputNodes() const
	{
		IProcessable::pVect retVal;

		IProcessable::pVect::const_iterator it = nodes.begin();
		while (it != nodes.end())
		{
			if ((*(*it)).getPlace() == IProcessable::Input)
				retVal.insert(retVal.end(),(*it));

			++it;
		}
		return retVal;
	}

	IProcessable::pVect Graph::getOutputNodes() const
	{
		IProcessable::pVect retVal;

		IProcessable::pVect::const_iterator it = nodes.begin();
		while (it != nodes.end())
		{
			if ((*(*it)).getPlace() == IProcessable::Output)
				retVal.insert(retVal.end(),(*it));

			++it;
		}
		return retVal;
	}



}
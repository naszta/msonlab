#pragma once
#include "Graph.h"
#include "BFSIterator.h"
#include "DFSIterator.h"
#include "NodeTest.h"

namespace msonlab
{

	using std::swap;

	Graph::Graph()
	{
		nodes.clear();
		edges.clear();
		this->iteratorEnd = std::make_shared<NodeTest>(0, L"0", nullptr, 0);
	}

	Graph::Graph(Graph&& other)
	{
		swap(nodes, other.nodes);
		swap(edges, other.edges);
		swap(iteratorEnd, other.iteratorEnd);
	}

	bool Graph::addNode(NodePtr toAdd)
	{
		msonlab::NodeVect::iterator it;
		it = std::find(nodes.begin(), nodes.end(), toAdd);

		if (it != nodes.end())
			return false;

		it = nodes.insert(nodes.end(), toAdd);

		if (*it == toAdd)
			return true;
		else
		{
			throw Exceptions::FailedToAddNodeException("Failed to add the node to the graph!");
			return false;
		}
	}

	bool Graph::addEdge(EdgePtr toAdd)
	{
		msonlab::EdgeVect::iterator it;
		it = std::find(edges.begin(), edges.end(), toAdd);

		if (it != edges.end())
			return false;

		msonlab::NodePtr fromNode = (*toAdd).getFrom();
		addNode(fromNode);
		msonlab::NodePtr toNode = (*toAdd).getTo();
		addNode(toNode);

		it = edges.insert(edges.end(), toAdd);

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

	size_t Graph::size() const
	{
		return nodes.size();
	}

	size_t Graph::numberOfNodes() const
	{
		return nodes.size();
	}

	size_t Graph::numberOfEdges() const
	{
		return edges.size();
	}

	const NodeVect& Graph::getNodes() const
	{
		return nodes;
	}

	const EdgeVect& Graph::getEdges() const
	{
		return edges;
	}

	NodeVect Graph::getInputNodes() const
	{
		NodeVect retVal;

		NodeVect::const_iterator it = nodes.begin();
		while (it != nodes.end())
		{
			if ((*(*it)).getPlace() == IProcessable::Input)
				retVal.insert(retVal.end(), (*it));

			++it;
		}
		return retVal;
	}

	NodeVect Graph::getOutputNodes() const
	{
		NodeVect retVal;

		NodeVect::const_iterator it = nodes.begin();
		while (it != nodes.end())
		{
			if ((*(*it)).getPlace() == IProcessable::Output)
				retVal.insert(retVal.end(), (*it));

			++it;
		}
		return retVal;
	}

	GraphPtr Graph::getPartialGraphByEdgeType(Edge::EdgeTypeEnum edgeType) const
	{
		GraphPtr ptrGraph = std::make_unique<Graph>();

		EdgeVect::const_iterator it = edges.begin();
		while (it != edges.end())
		{
			if ((*(*it)).getEdgeType() == edgeType)
			{
				(*ptrGraph).addEdge((*it));
			}

			++it;
		}

		return ptrGraph;
	}

	BFSIterator Graph::bfsIteratorBegin()
	{
		BFSIterator bfsIT(*this);
		return bfsIT;
	}

	BFSIterator Graph::bfsIteratorEnd()
	{
		BFSIterator bfsIt(this->iteratorEnd);
		return bfsIt;
	}

	DFSIterator Graph::dfsIteratorBegin()
	{
		DFSIterator dfsIT(*this);
		return dfsIT;
	}

	DFSIterator Graph::dfsIteratorEnd()
	{
		DFSIterator dfsIt(this->iteratorEnd);
		return dfsIt;
	}
}
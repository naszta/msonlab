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
		_nodes.clear();
		_edges.clear();
		this->iteratorEnd = std::make_shared<NodeTest>(0, L"0", nullptr, 0);
	}

	Graph::Graph(Graph&& other)
	{
		swap(_nodes, other._nodes);
		swap(_edges, other._edges);
		swap(iteratorEnd, other.iteratorEnd);
	}

	bool Graph::addNode(NodePtr toAdd)
	{
		msonlab::NodeVect::iterator it;
		it = std::find(_nodes.begin(), _nodes.end(), toAdd);

		if (it != _nodes.end())
			return false;

		it = _nodes.insert(_nodes.end(), toAdd);

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
		it = std::find(_edges.begin(), _edges.end(), toAdd);

		if (it != _edges.end())
			return false;

		msonlab::NodePtr fromNode = (*toAdd).getFrom();
		addNode(fromNode);
		msonlab::NodePtr toNode = (*toAdd).getTo();
		addNode(toNode);

		it = _edges.insert(_edges.end(), toAdd);

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
		return _nodes.size();
	}

	size_t Graph::numberOfNodes() const
	{
		return _nodes.size();
	}

	size_t Graph::numberOfEdges() const
	{
		return _edges.size();
	}

	const NodeVect& Graph::getNodes() const
	{
		return _nodes;
	}

	const EdgeVect& Graph::getEdges() const
	{
		return _edges;
	}

	NodeVect Graph::getInputNodes() const
	{
		NodeVect retVal;

		NodeVect::const_iterator it = _nodes.begin();
		while (it != _nodes.end())
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

		NodeVect::const_iterator it = _nodes.begin();
		while (it != _nodes.end())
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

		EdgeVect::const_iterator it = _edges.begin();
		while (it != _edges.end())
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
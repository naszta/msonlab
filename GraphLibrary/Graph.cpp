#pragma once
#include "Graph.h"
#include "BFSIterator.h"
#include "DFSIterator.h"

namespace msonlab
{
	Graph::Graph()
	{
		nodes.clear();
		edges.clear();
		this->iteratorEnd = std::make_shared<Node>(0, L"0", nullptr);
	}

	Graph::Graph(const Graph& other)
	{
		*this = other;
	}

	// TODO: rewrite as deep copy
	Graph& Graph::operator=(const Graph& other)
	{
		if (this != &other)
		{
			this->nodes = other.nodes;
			this->edges = other.edges;
		}

		return *this;
	}

	bool Graph::addNode(IProcessable::nPtr toAdd)
	{
		msonlab::IProcessable::nVect::iterator it;
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

	bool Graph::addEdge(IProcessable::ePtr toAdd)
	{
		msonlab::IProcessable::eVect::iterator it;
		it = std::find(edges.begin(), edges.end(), toAdd);

		if (it != edges.end())
			return false;

		msonlab::IProcessable::nPtr fromNode = (*toAdd).getFrom();
		addNode(fromNode);
		msonlab::IProcessable::nPtr toNode = (*toAdd).getTo();
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

	size_t Graph::numberOfNodes() const
	{
		return nodes.size();
	}

	size_t Graph::numberOfEdges() const
	{
		return edges.size();
	}

	const IProcessable::nVect& Graph::getNodes() const
	{
		return nodes;
	}

	const IProcessable::eVect& Graph::getEdges() const
	{
		return edges;
		}


	IProcessable::nVect Graph::getInputNodes() const
	{
		IProcessable::nVect retVal;

		IProcessable::nVect::const_iterator it = nodes.begin();
		while (it != nodes.end())
		{
			if ((*(*it)).getPlace() == IProcessable::Input)
				retVal.insert(retVal.end(), (*it));

			++it;
		}
		return retVal;
	}

	IProcessable::nVect Graph::getOutputNodes() const
	{
		IProcessable::nVect retVal;

		IProcessable::nVect::const_iterator it = nodes.begin();
		while (it != nodes.end())
		{
			if ((*(*it)).getPlace() == IProcessable::Output)
				retVal.insert(retVal.end(), (*it));

			++it;
		}
		return retVal;
	}

	Graph::gPtr Graph::getPartialGraphByEdgeType(Edge::EdgeTypeEnum edgeType) const
	{
		Graph::gPtr ptrGraph = std::make_unique<Graph>();

		IProcessable::eVect::const_iterator it = edges.begin();
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
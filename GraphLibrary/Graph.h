#pragma once
#include "Global.h"
#include <boost/enable_shared_from_this.hpp>
#include "IProcessable.h"
#include "Node.h"
#include "Edge.h"

namespace msonlab
{

	/// Models a Graph. The graph is consits of nodes connected by edges.
	/// std::enable_shared_from_this allows an object that is currently managed by a
	/// std::shared_ptr named p to safely generate std::shared_ptr instances pt1, pt2 ...
	/// that all share ownership of t with pt.
	class Graph : public std::enable_shared_from_this<Graph>
	{
	private:
		IProcessable::nVect nodes;
		IProcessable::eVect edges;

		IProcessable::nPtr iteratorEnd; // this is a sign for every iterator

		friend class GeneticAlgorithm;
		friend class GraphAlgorithms;
		friend class GraphIterator;
		friend class BFSIterator;
		friend class DFSIterator;

	public:
		typedef std::shared_ptr<Graph> gPtr;

		Graph();
		Graph(const Graph& other);
		Graph& operator=(const Graph& other);

		bool addNode(IProcessable::nPtr toAdd);
		bool addEdge(IProcessable::ePtr toAdd);

		size_t numberOfNodes() const;
		size_t numberOfEdges() const;

		bool importGraph(std::istream &in) const;
		bool exportGraph(std::ostream &out) const;

		IProcessable::nVect getInputNodes() const;
		IProcessable::nVect getOutputNodes() const;

		BFSIterator bfsIteratorBegin();
		BFSIterator bfsIteratorEnd();
		DFSIterator dfsIteratorBegin();
		DFSIterator dfsIteratorEnd();

		gPtr getPartialGraphByEdgeType(Edge::EdgeTypeEnum _edgeType) const;
	};

}
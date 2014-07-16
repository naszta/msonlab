#pragma once
#include "Global.h"
#include "IProcessable.h"
#include "Node.h"
#include "Edge.h"

namespace msonlab
{

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

		const IProcessable::nVect getNodes() const;
		const IProcessable::eVect getEdges() const;

		IProcessable::nVect getInputNodes() const;
		IProcessable::nVect getOutputNodes() const;

		BFSIterator bfsIteratorBegin();
		BFSIterator bfsIteratorEnd();
		DFSIterator dfsIteratorBegin();
		DFSIterator dfsIteratorEnd();

		gPtr getPartialGraphByEdgeType(Edge::EdgeTypeEnum _edgeType) const;
	};

}
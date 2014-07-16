#pragma once
#include "Global.h"
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
		IProcessable::nVect nodes; // vector of nodes
		IProcessable::eVect edges; // vector of edges

		IProcessable::nPtr iteratorEnd; // this is a sign for every iterator

		friend class GeneticAlgorithm;
		friend class GraphAlgorithms;
		friend class GraphIterator;
		friend class BFSIterator;
		friend class DFSIterator;

	public:
		typedef std::unique_ptr<Graph> gPtr;

		Graph(); // empty constructor
		Graph(const Graph& other); // copy constructor
		Graph& operator=(const Graph& other); // assignment operator

		bool addNode(IProcessable::nPtr toAdd); // adds a node to the graph
		bool addEdge(IProcessable::ePtr toAdd); // adds an edge to the graph

		size_t numberOfNodes() const; // gets the # of nodes
		size_t numberOfEdges() const; // gets the # of edges

		const IProcessable::nVect getNodes() const;
		const IProcessable::eVect getEdges() const;

		IProcessable::nVect getInputNodes() const; // gets the input nodes
		IProcessable::nVect getOutputNodes() const; // gets the output nodes

		BFSIterator bfsIteratorBegin(); // gets the begin of the bfs iterator
		BFSIterator bfsIteratorEnd(); // gets the end of the bfs iterator
		DFSIterator dfsIteratorBegin(); // gets the begin of the dfs iterator
		DFSIterator dfsIteratorEnd(); // gets the end of the dfs iterator

		gPtr getPartialGraphByEdgeType(Edge::EdgeTypeEnum _edgeType) const;
	};

}
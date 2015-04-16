#pragma once
#include "Global.h"
#include "IProcessable.h"
#include "Node.h"
#include "Edge.h"

namespace msonlab
{

	class Graph;
	typedef std::unique_ptr<Graph> GraphPtr;

	/// Models a Graph. The graph is consits of nodes connected by edges.
	/// std::enable_shared_from_this allows an object that is currently managed by a
	/// std::shared_ptr named p to safely generate std::shared_ptr instances pt1, pt2 ...
	/// that all share ownership of t with pt.
	class Graph
	{
	private:
		NodeVect _nodes; // vector of nodes
		EdgeVect _edges; // vector of edges

		NodePtr iteratorEnd; // this is a sign for every iterator

		friend class GraphIterator;
		friend class BFSIterator;
		friend class DFSIterator;

	public:
		Graph(); // empty constructor
		Graph(const Graph& other) = delete; // copy constructor
		Graph(Graph&& other);
		Graph& operator=(const Graph& other) = delete; // assignment operator

		bool addNode(NodePtr toAdd); // adds a node to the graph
		bool addEdge(EdgePtr toAdd); // adds an edge to the graph

		size_t size() const; // gets the # of nodes
		size_t numberOfNodes() const; // gets the # of nodes
		size_t numberOfEdges() const; // gets the # of edges

		const NodeVect& getNodes() const;
		const EdgeVect& getEdges() const;
		const NodeVect& nodes() const { return getNodes(); }

		NodeVect getInputNodes() const; // gets the input nodes
		NodeVect getOutputNodes() const; // gets the output nodes

		BFSIterator bfsIteratorBegin(); // gets the begin of the bfs iterator
		BFSIterator bfsIteratorEnd(); // gets the end of the bfs iterator
		DFSIterator dfsIteratorBegin(); // gets the begin of the dfs iterator
		DFSIterator dfsIteratorEnd(); // gets the end of the dfs iterator

		GraphPtr getPartialGraphByEdgeType(Edge::EdgeTypeEnum _edgeType) const;
	};

}
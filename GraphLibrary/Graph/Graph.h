#ifndef GRAPH 
#define GRAPH

#include "../Global.h"
#include "../IProcessable.h"
#include "Edge.h"
#include "Node.h"

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

		void ensureSize(size_t size);

	public:
		// constructs an empty graph
		Graph();
		// constructs a graph from the list of edges.
		Graph(std::initializer_list<EdgePtr> edges_);
		Graph(const Graph& other) = delete; // copy constructor
		Graph(Graph&& other);
		Graph& operator=(const Graph& other) = delete; // assignment operator
		Graph& operator=(const Graph&& other);
		~Graph();

		bool addNode(NodePtr toAdd); // adds a node to the graph
		bool addEdge(EdgePtr toAdd); // adds an edge to the graph

		size_t order() const; // gets the # of nodes
		size_t size() const; // gets the # of edges

		const NodeVect& getNodes() const;
		const EdgeVect& getEdges() const;
		const NodeVect& nodes() const { return getNodes(); }

		NodeVect getInputNodes() const; // gets the input nodes
		NodeVect getOutputNodes() const; // gets the output nodes
		NodeVect onodes() const { return getOutputNodes(); }

		GraphPtr getPartialGraphByEdgeType(Edge::EdgeTypeEnum _edgeType) const;

		struct bfs_iterator {
			bfs_iterator(const Graph& g_);
			bfs_iterator& operator=(const bfs_iterator&) = delete;
			BFSIterator begin() const;
			BFSIterator end() const;
		private:
			const Graph& graph;
		};

		struct dfs_iterator {
			dfs_iterator(const Graph& g_);
			dfs_iterator& operator=(const dfs_iterator&) = delete;
			DFSIterator begin() const;
			DFSIterator end() const;

		private:
			const Graph& graph;
		};

		bfs_iterator bfs() const {
			return bfs_iterator(*this);
		}

		dfs_iterator dfs() const {
			return dfs_iterator(*this);
		}
	};
}
#endif
#ifndef GRAPH_ITERATOR_H
#define GRAPH_ITERATOR_H

#include "Graph.h"
#include <queue>
#include <set>
#include <iterator>

namespace msonlab
{
	using std::queue;
	using std::set;

	/*
	* Class iterates over the Graph as BFS.
	*/
	class GraphIterator : public std::iterator<std::input_iterator_tag, NodePtr>
	{
	protected:
		NodePtr node;
		NodePtr end;
		queue<NodePtr> inputNodes;
		//GraphPtr graph;
		// This method chooses the next node and steps there
		virtual bool moveNext() = 0;
		virtual bool clear() = 0;
	public:
		GraphIterator(NodePtr node, NodePtr end);
		GraphIterator();
		GraphIterator(const GraphIterator& it);
		GraphIterator& operator=(const GraphIterator& it);
		bool operator==(const GraphIterator& it) const;
		bool operator!=(const GraphIterator& it) const;
		NodePtr operator*() const;
		bool setStartNode(NodePtr startNode);
	};
}

#endif
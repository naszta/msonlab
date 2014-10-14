#ifndef BFS_ITERATOR_H
#define BFS_ITERATOR_H

#include "GraphIterator.h"
#include <queue>
#include <set>

namespace msonlab
{
	using std::queue;
	using std::set;
	using std::pair;

	/*
	* Class iterates over the Graph as BFS.
	*/
	class BFSIterator : public GraphIterator
	{
		queue<pair<NodePtr, unsigned>> to_visit; // nodes to visit
		set<NodePtr> visited; // visited nodes
		// This method chooses the next node and steps there
		virtual bool moveNext();
		virtual bool clear();
		unsigned current_depth;
	public:
		BFSIterator(NodePtr endPtr);
		BFSIterator(const Graph& g);
		BFSIterator(const BFSIterator& it);
		BFSIterator& operator=(const BFSIterator& it);
		BFSIterator& operator++(); // prefix
		BFSIterator operator++(int); // postfix
		bool skipActNode();
		bool hasMoreNode();
		unsigned depth() { return current_depth; }
	};
}

#endif
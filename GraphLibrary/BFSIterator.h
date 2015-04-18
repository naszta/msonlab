#ifndef BFS_ITERATOR_H
#define BFS_ITERATOR_H

#include "GraphIterator.h"
#include <queue>

namespace msonlab
{
	using std::queue;
	using std::pair;

	/*
	* Class iterates over the Graph as BFS.
	*/
	class BFSIterator final : public GraphIterator
	{
		queue<pair<NodePtr, unsigned>> to_visit; // nodes to visit
		//set<NodePtr> visited; // visited nodes
		vector<bool> visited;
		// This method chooses the next node and steps there
		void moveNext() override;
		void clear() override;
		unsigned current_depth;
	public:
		BFSIterator(NodePtr endPtr);
		BFSIterator(const Graph& g);
		BFSIterator(const BFSIterator& it);
		BFSIterator& operator=(const BFSIterator& it);
		BFSIterator& operator++(); // prefix
		BFSIterator operator++(int); // postfix
		void skipChildren();
		bool hasMoreNode();
		unsigned depth() { return current_depth; }
	};
}

#endif
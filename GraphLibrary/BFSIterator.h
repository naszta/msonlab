#pragma once
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
		queue<pair<IProcessable::nPtr, unsigned>> to_visit; // nodes to visit
		set<IProcessable::nPtr> visited; // visited nodes
		// This method chooses the next node and steps there
		virtual bool moveNext();
		virtual bool clear();
		unsigned current_depth;
	public:
		BFSIterator(IProcessable::nPtr endPtr);
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
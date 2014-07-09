#pragma once
#include "GraphIterator.h"
#include <queue>
#include <set>

namespace msonlab
{
	using std::queue;
	using std::set;

	/*
	* Class iterates over the Graph as BFS.
	*/
	class BFSIterator : public GraphIterator
	{
		queue<IProcessable::nPtr> toVisit; // nodes to visit
		set<IProcessable::nPtr> visited; // visited nodes
		// This method chooses the next node and steps there
		virtual bool moveNext();
		virtual bool clear();
	public:
		BFSIterator(IProcessable::nPtr endPtr);
		BFSIterator(Graph& g);
		BFSIterator(BFSIterator& it);
		BFSIterator& operator=(BFSIterator& it);
		BFSIterator& operator++(); // prefix
		BFSIterator operator++(int); // postfix
		bool skipActNode();
		bool hasMoreNode();
	};
}
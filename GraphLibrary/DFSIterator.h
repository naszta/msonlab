#pragma once
#include "GraphIterator.h"
#include <queue>
#include <set>
#include <stack>

namespace msonlab
{
	using std::queue;
	using std::set;
	using std::stack;

	/*
	* Class iterates over the Graph as DFS.
	*/
	class DFSIterator : public GraphIterator
	{
		IProcessable::nSet explored;
		IProcessable::nSet discovered;
		stack<IProcessable::nPtr> toDiscover;
		// This method chooses the next node and steps there
		virtual bool moveNext();
		virtual bool clear();
	public:
		DFSIterator(IProcessable::nPtr endPtr);
		DFSIterator(Graph& g);
		DFSIterator(DFSIterator& it);
		DFSIterator& operator=(DFSIterator& it);
		bool operator==(const DFSIterator& it) const;
		bool operator!=(const DFSIterator& it) const;
		DFSIterator& operator++(); // prefix
		DFSIterator operator++(int); // postfix
		bool hasMoreNode();
		bool skipActNode();
	};
}
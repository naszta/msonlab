#pragma once
#include "Global.h"
#include "Graph.h"
#include "GraphIterator.h"
#include "IProcessable.h"
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
		// This method chooses the next node and steps there
		virtual bool moveNext();
	public:
		BFSIterator(Graph::gPtr g);
		BFSIterator(BFSIterator& it);
		BFSIterator& operator=(BFSIterator& it);
		BFSIterator& operator++(); // prefix
		BFSIterator operator++(int); // postfix
		bool hasMoreNode();
	};
}
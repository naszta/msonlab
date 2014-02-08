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
	class DFSIterator : public GraphIterator
	{
		// This method chooses the next node and steps there
		virtual bool moveNext();
	public:
		DFSIterator(Graph::gPtr g);
		DFSIterator(DFSIterator& it);
		DFSIterator& operator=(DFSIterator& it);
		DFSIterator& operator++(); // prefix
		DFSIterator operator++(int); // postfix
		bool hasMoreNode();
	};
}
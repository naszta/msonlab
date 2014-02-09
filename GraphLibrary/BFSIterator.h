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
<<<<<<< HEAD
		queue<IProcessable::nPtr> toVisit; // nodes to visit
		set<IProcessable::nPtr> visited; // visited nodes
		// This method chooses the next node and steps there
		virtual bool moveNext();
		virtual bool clear();
	public:
		BFSIterator(IProcessable::nPtr endPtr);
=======
		// This method chooses the next node and steps there
		virtual bool moveNext();
	public:
>>>>>>> gabooo
		BFSIterator(Graph::gPtr g);
		BFSIterator(BFSIterator& it);
		BFSIterator& operator=(BFSIterator& it);
		BFSIterator& operator++(); // prefix
		BFSIterator operator++(int); // postfix
<<<<<<< HEAD
		bool skipActNode();
=======
>>>>>>> gabooo
		bool hasMoreNode();
	};
}
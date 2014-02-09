#pragma once
#include "Global.h"
#include "Graph.h"
#include "GraphIterator.h"
#include "IProcessable.h"
#include <queue>
#include <set>
<<<<<<< HEAD
#include <stack>
=======
>>>>>>> gabooo

namespace msonlab
{
	using std::queue;
	using std::set;
<<<<<<< HEAD
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
		DFSIterator(Graph::gPtr g);
		DFSIterator(DFSIterator& it);
		DFSIterator& operator=(DFSIterator& it);
		bool operator==(const DFSIterator& it) const;
		bool operator!=(const DFSIterator& it) const;
		DFSIterator& operator++(); // prefix
		DFSIterator operator++(int); // postfix
		bool hasMoreNode();
		bool skipActNode();
=======

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
>>>>>>> gabooo
	};
}
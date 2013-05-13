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
		//IProcessable::nPtr node;
		//IProcessable::nPtr end;
		//queue<IProcessable::nPtr> toVisit; // nodes to visit
		//set<IProcessable::nPtr> visited; // visited nodes
		//Graph::gPtr graph;
		// This method chooses the next node and steps there
		virtual bool moveNext();
		//bool clear();
	public:
		BFSIterator(Graph::gPtr g);
		BFSIterator(BFSIterator& it);
		BFSIterator& operator=(BFSIterator& it);
		BFSIterator& operator++(); // prefix
		BFSIterator operator++(int); // postfix
		bool hasMoreNode();
		//bool operator==(const BFSIterator& it) const;
		//IProcessable::nPtr operator*();
		//bool skipActNode();
		//bool setStartNode(IProcessable::nPtr startNode);
	};
}
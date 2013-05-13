#pragma once
#include "Global.h"
#include "Graph.h"
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
	class GraphIterator
	{
	protected:
		IProcessable::nPtr node;
		IProcessable::nPtr end;
		queue<IProcessable::nPtr> toVisit; // nodes to visit
		queue<IProcessable::nPtr> inputNodes;
		set<IProcessable::nPtr> visited; // visited nodes
		Graph::gPtr graph;
		// This method chooses the next node and steps there
		virtual bool moveNext() = 0;
		bool clear();
	public:
		GraphIterator(Graph::gPtr g);
		bool operator==(const GraphIterator& it) const;
		IProcessable::nPtr operator*();
		bool skipActNode();
		bool setStartNode(IProcessable::nPtr startNode);
	};
}
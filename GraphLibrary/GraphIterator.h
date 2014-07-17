#pragma once
#include "Graph.h"
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
		queue<IProcessable::nPtr> inputNodes;
		//Graph::gPtr graph;
		// This method chooses the next node and steps there
		virtual bool moveNext() = 0;
		virtual bool clear() = 0;
	public:
		GraphIterator(IProcessable::nPtr node, IProcessable::nPtr end);
		GraphIterator();
		GraphIterator(GraphIterator& it);
		GraphIterator& operator=(GraphIterator& it);
		bool operator==(const GraphIterator& it) const;
		IProcessable::nPtr operator*();
		bool setStartNode(IProcessable::nPtr startNode);
	};
}
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
	class DFSIterator final : public GraphIterator
	{
		NodeSet explored;
		NodeSet discovered;
		vector<bool> visited;
		stack<const NodePtr, vector<const NodePtr>> to_visit;
		// This method chooses the next node and steps there
		virtual bool moveNext();
		virtual bool clear();
	public:
		DFSIterator(NodePtr endPtr);
		DFSIterator(const Graph& g);
		DFSIterator(const DFSIterator& it);
		DFSIterator& operator=(const DFSIterator& it);
		bool operator==(const DFSIterator& it) const;
		bool operator!=(const DFSIterator& it) const;
		DFSIterator& operator++(); // prefix
		DFSIterator operator++(int); // postfix
		bool hasMoreNode();
		bool skipActNode();
	};
}
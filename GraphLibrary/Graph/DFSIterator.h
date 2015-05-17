#ifndef DFS_ITERATOR_H
#define DFS_ITERATOR_H

#include <stack>

#include "GraphIterator.h"

namespace msonlab
{
	using std::stack;

	// Iterates over the Graph as DFS.
	class DFSIterator final : public GraphIterator
	{
		vector<bool> visited;
		vector<bool> discovered;
		stack<const NodePtr, vector<const NodePtr>> to_visit;
		// Moves the iterator to the next node
		virtual void moveNext();
		virtual void clear();
	public:
		DFSIterator(NodePtr endPtr);
		DFSIterator(const Graph& g);
		DFSIterator(const DFSIterator& it);
		DFSIterator& operator=(const DFSIterator& it);
		bool operator==(const DFSIterator& it) const;
		bool operator!=(const DFSIterator& it) const;
		DFSIterator& operator++(); // prefix
		DFSIterator operator++(int); // postfix
		void skipChildren();
	};
}

#endif
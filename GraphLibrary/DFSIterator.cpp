#include "IProcessable.h"
#include "DFSIterator.h"
#include <stack>


namespace msonlab
{
	using std::stack;

	DFSIterator::DFSIterator(NodePtr endPtr)
		:GraphIterator(endPtr, endPtr)
	{
	}

	DFSIterator::DFSIterator(const Graph& g)
		: to_visit(stack<const NodePtr, vector<const NodePtr>>(g.getInputNodes())),
		visited(vector<bool>(g.order(), false))
	{
		// end of the iterator is unique for every graph
		// but the same for every iterator on the same graph
		this->end = g.iteratorEnd;

		// adding input nodes to the queue.
		//NodeVect inputNodesVect = g.getInputNodes();
		//this->to_visit = stack(g.getInputNodes());
		//if (inputNodesVect.size() > 0)
		//{
		//	NodeVect::iterator it = inputNodesVect.begin();
		//	this->to_visit.push(*it);
		//	this->node = *it;
		//	++it; // can skip first from the queue
		//	for (; it != inputNodesVect.end(); ++it)
		//	{
		//		this->inputNodes.push(*it);
		//	}
		//}
		//else
		//{
		//	this->node = this->end;
		//}

		this->moveNext();
	}

	// Copy constructor
	DFSIterator::DFSIterator(const DFSIterator& it) : GraphIterator(it)
	{
		if (this != &it)
		{
			*this = it;
		}
	}

	DFSIterator& DFSIterator::operator=(const DFSIterator& it)
	{
		if (this != &it)
		{
			this->clear();
			this->node = it.node;
			this->end = it.end;
			this->inputNodes = it.inputNodes;

			// copy state
			//this->explored = it.explored;
			//this->discovered = it.discovered;
			this->to_visit = it.to_visit;
			this->visited = it.visited;
		}

		return *this;
	}

	/**
	* PRIVATE METHODS
	*/

	bool DFSIterator::moveNext()
	{
		// nothing to do, when it points to end
		if (this->node == this->end) {
			return false;
		}

		if (this->to_visit.empty()) {
			this->node = this->end;
			return false;
		}

		this->node = this->to_visit.top();
		this->to_visit.pop();
		// set the node as visited
		this->visited[this->node->id()] = 1;

		const auto& neighbours = this->node->getSuccessors();
		for (const auto& edge : neighbours) {
			// add the node to the stack if it is not yet visited
			if (!this->visited[edge->getToId()]) {
				this->to_visit.push(edge->getTo());
			}
		}

		return true;
	}

	bool DFSIterator::clear()
	{
		this->node = this->end;
		stack<const NodePtr, NodeVect> emptyStack;
		std::swap(this->to_visit, emptyStack);
		vector<bool> emptyVector;
		std::swap(this->visited, emptyVector);

		return true;
	}

	/**
	* PUBLIC METHODS
	*/
	bool DFSIterator::operator==(const DFSIterator& it) const
	{
		return this->node == it.node;
	}
	bool DFSIterator::operator!=(const DFSIterator& it) const
	{
		return this->node != it.node;
	}

	DFSIterator& DFSIterator::operator++() // prefix
	{
		this->moveNext();
		return *this;
	}

	DFSIterator DFSIterator::operator++(int) // postfix
	{
		DFSIterator dfs_it = *this;
		this->moveNext();
		return dfs_it;
	}
}
#include "DFSIterator.h"

namespace msonlab
{
	using std::stack;

	DFSIterator::DFSIterator(NodePtr endPtr)
		: GraphIterator(endPtr, endPtr)
	{
	}

	DFSIterator::DFSIterator(const Graph& g)
		: to_visit(stack<const NodePtr, vector<const NodePtr>>(g.getInputNodes())),
		visited(vector<bool>(g.order(), false)), discovered(vector<bool>(g.order(), false))
	{
		// end of the iterator is unique for every graph
		// but the same for every iterator on the same graph
		this->end = g.iteratorEnd;
		this->node = nullptr;
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
			this->to_visit = it.to_visit;
			this->visited = it.visited;
			this->discovered = it.discovered;
		}

		return *this;
	}

	/**
	* PRIVATE METHODS
	*/

	void DFSIterator::moveNext()
	{
		// nothing to do, when it points to end
		if (this->node == this->end) {
			return;
		}

		while (!to_visit.empty() && (this->node == nullptr || this->visited[this->node->id()])) {
			this->node = this->to_visit.top();
			this->to_visit.pop();
		}

		if (this->to_visit.empty() && this->visited[this->node->id()]) {
			this->node = this->end;
			return;
		}
		
		// set the node as visited
		this->visited[this->node->id()] = 1;

		const auto& neighbours = this->node->getSuccessors();
		for (const auto& edge : neighbours) {
			// add the node to the stack if it is not yet visited
			if (!this->discovered[edge->getToId()]) {
				this->discovered[edge->getToId()] = 1;
				this->to_visit.push(edge->getTo());
			}
		}
	}

	void DFSIterator::clear()
	{
		this->node = this->end;
		stack<const NodePtr, NodeVect> emptyStack;
		std::swap(this->to_visit, emptyStack);
		vector<bool> emptyVector_1;
		std::swap(this->visited, emptyVector_1);
		vector<bool> emptyVector_2;
		std::swap(this->discovered, emptyVector_2);
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

	// Marks the neighbours of the actual node visited
	// so the iterator will skip them
	void DFSIterator::skipChildren()
	{
		// adding neighbours to visited set
		EdgeVect neighbours = node->getSuccessors();
		for (const auto& edge : neighbours) {
			visited[edge->getToId()] = true;
		}
	}
}
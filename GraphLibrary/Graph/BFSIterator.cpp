#include "BFSIterator.h"

namespace msonlab
{
	BFSIterator::BFSIterator(NodePtr endPtr)
		:GraphIterator(endPtr, endPtr)
	{
	}

	BFSIterator::BFSIterator(const Graph& g) :
		visited(vector<bool>(g.order(), false))
	{
		// end of the iterator is unique for every graph
		// but the same for every iterator on the same graph
		this->end = g.iteratorEnd;
		this->current_depth = 0;

		// adding input nodes to the queue.
		NodeVect inputNodesVect = g.getInputNodes();
		if (inputNodesVect.size() > 0)
		{
			this->node = inputNodesVect[0];
			for (auto node : inputNodesVect)
			{
				this->inputNodes.push(node);
			}
		}
		else
		{
			this->node = this->end;
		}

		this->moveNext();
	}

	// Copy constructor
	BFSIterator::BFSIterator(const BFSIterator& it) : GraphIterator(it)
	{
		if (this != &it)
		{
			*this = it;
		}
	}

	BFSIterator& BFSIterator::operator=(const BFSIterator& it)
	{
		if (this != &it)
		{
			this->clear();
			this->node = it.node;
			this->end = it.end;
			this->inputNodes = it.inputNodes;
			// copy state
			this->visited = it.visited;
			this->to_visit = it.to_visit;
		}

		return *this;
	}

	/**
	* PRIVATE METHODS
	*/

	// steps to the next node
	// return true if there are any node to visit; otherwise false.
	void BFSIterator::moveNext()
	{
		// find a node that haven't been visited yet
		while(visited[node->id()])
		{
			if (node == end || to_visit.empty())
			{
				// use the next input node
				if (!this->inputNodes.empty())
				{
					this->node = this->inputNodes.front();
					this->inputNodes.pop();
				}
				else
				{
					this->node = this->end;
					return;
				}
			}
			else
			{
				// there are nodes in the toVisit queue, choose the first
				auto front = this->to_visit.front();
				this->node = front.first;
				this->current_depth = front.second;
				this->to_visit.pop();
			}
		}

		// add node to visited
		visited[node->id()] = 1;

		// adding neighbours to the toVisit queue
		const EdgeVect &neighbours = node->getSuccessors();
		for (auto& edge : neighbours)
		{
			if (!visited[edge->getToId()])
			{
				to_visit.push(make_pair(edge->getTo(), current_depth +1));
			}
		}
	}

	// Clears the state of the visitor
	void BFSIterator::clear()
	{
		this->node = this->end;
		this->current_depth = 0;
		std::queue<pair<NodePtr, unsigned>> emptyQueue;
		std::swap(this->to_visit, emptyQueue);
		std::vector<bool> emptyVector(this->visited.size(), false);
		std::swap(this->visited, emptyVector);
		std::queue<NodePtr> emptyInputQueue;
		std::swap(this->inputNodes, emptyInputQueue);
	}

	/**
	* PUBLIC METHODS
	*/

	BFSIterator& BFSIterator::operator++() // prefix
	{
		this->moveNext();
		return *this;
	}

	BFSIterator BFSIterator::operator++(int) // postfix
	{
		BFSIterator bfs_it = *this;
		this->moveNext();
		return bfs_it;
	}

	bool BFSIterator::hasMoreNode()
	{
		return this->node != this->end;
	}

	// Marks the neighbours of the actual node visited
	// so the bfs will skip them
	void BFSIterator::skipChildren()
	{
		// adding neighbours to visited set
		EdgeVect neighbours = node->getSuccessors();
		for (const auto& edge : neighbours) {
			visited[edge->getToId()] = true;
		}
	}
}
#include "BFSIterator.h"

namespace msonlab
{
	BFSIterator::BFSIterator(NodePtr endPtr)
		:GraphIterator(endPtr, endPtr)
	{
	}

	BFSIterator::BFSIterator(const Graph& g)
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
			GraphIterator::operator=(it);

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
	bool BFSIterator::moveNext()
	{
		// find a node that haven't been visited yet
		while (visited.count(node) > 0)
		{
			if (node == end || to_visit.empty())
			{
				// use the next input node
				if (this->inputNodes.size() > 0)
				{
					this->node = this->inputNodes.front();
					this->inputNodes.pop();
				}
				else
				{
					this->node = this->end;
					return false;
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
		visited.insert(node);

		// adding neighbours to the toVisit queue
		const EdgeVect &neighbours = node->getSuccessors();
		for (auto& edge : neighbours)
		{
			if (visited.count(edge->getTo()) == 0)
			{
				to_visit.push(make_pair(edge->getTo(), current_depth +1));
			}
		}

		return true;
	}

	// Clears the state of the visitor
	bool BFSIterator::clear()
	{
		this->node = this->end;
		this->current_depth = 0;
		std::queue<pair<NodePtr, unsigned>> emptyQueue;
		std::swap(this->to_visit, emptyQueue);
		std::set<NodePtr> emptySet;
		std::swap(this->visited, emptySet);
		std::queue<NodePtr> emptyInputQueue;
		std::swap(this->inputNodes, emptyInputQueue);

		return true;
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
	bool BFSIterator::skipActNode()
	{
		// adding neighbours to visited set
		EdgeVect::const_iterator it;
		EdgeVect neighbours = node->getSuccessors();
		for (it = neighbours.cbegin(); it != neighbours.cend(); ++it)
		{
			if (visited.count((*it)->getTo()) == 0)
			{
				visited.insert((*it)->getTo());
			}
		}

		return true;
	}
}
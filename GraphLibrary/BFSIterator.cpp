#include "BFSIterator.h"

namespace msonlab
{
	BFSIterator::BFSIterator(IProcessable::nPtr endPtr)
		:GraphIterator(endPtr, endPtr)
	{
	}

	BFSIterator::BFSIterator(Graph::gPtr g) : GraphIterator(g)
	{
		// end of the iterator is unique for every graph
		// but the same for every iterator on the same graph
		this->end = g->iteratorEnd;

		// adding input nodes to the queue.
		IProcessable::nVect inputNodesVect = g->getInputNodes();
		if (inputNodesVect.size() > 0)
		{
			IProcessable::nVect::iterator it = inputNodesVect.begin();
			this->node = *it;
			++it; // can skip first from the queue
			for (; it != inputNodesVect.end(); ++it)
			{
				this->inputNodes.push(*it);
			}
		}
		else
		{
			this->node = this->end;
		}

		this->moveNext();
	}

	// Copy constructor
	BFSIterator::BFSIterator(BFSIterator& it) : GraphIterator(it.graph)
	{
		if (this != &it)
		{
			*this = it;
		}
	}

	BFSIterator& BFSIterator::operator=(BFSIterator& it)
	{
		if (this != &it)
		{
			this->clear();
			this->graph = it.graph;
			this->node = it.node;
			this->end = it.end;

			// copy state
			this->visited = it.visited;
			this->toVisit = it.toVisit;
			this->inputNodes = it.inputNodes;
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
		while (visited.count(node) > 0)
		{
			if (node == end || toVisit.empty())
			{
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
				this->node = this->toVisit.front();
				this->toVisit.pop();
			}
		}

		// add node to visited
		visited.insert(node);

		// adding neighbours to queue
		const IProcessable::eVect &neighbours = node->getSuccessors();
		for (auto it = neighbours.begin(); it != neighbours.end(); ++it)
		{
			if (visited.count((*it)->getTo()) == 0)
			{
				toVisit.push((*it)->getTo());
			}
		}

		return true;
	}

	// Clears the state of the visitor
	bool BFSIterator::clear()
	{
		this->node = this->end;
		std::queue<IProcessable::nPtr> emptyQueue;
		std::swap(this->toVisit, emptyQueue);
		std::set<IProcessable::nPtr> emptySet;
		std::swap(this->visited, emptySet);

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
		IProcessable::eVect::const_iterator it;
		IProcessable::eVect neighbours = node->getSuccessors();
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
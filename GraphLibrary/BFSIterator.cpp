#include "BFSIterator.h"

namespace msonlab
{
	BFSIterator::BFSIterator(Graph::gPtr g) : GraphIterator(g)
	{
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
			this->graph = it.graph;
			this->node = it.node;
			this->end = it.end;
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
		IProcessable::eVect::iterator it;
		IProcessable::eVect &neighbours = node->getSuccessors();
		for (it = neighbours.begin(); it != neighbours.end(); ++it)
		{
			if (visited.count((*it)->getTo()) == 0)
			{
				toVisit.push((*it)->getTo());
			}
		}

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


}
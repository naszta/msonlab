#include "GraphIterator.h"

namespace msonlab
{
	GraphIterator::GraphIterator(IProcessable::nPtr node, IProcessable::nPtr end)
		: node(node), end(end)
	{
	}

	GraphIterator::GraphIterator()
	{
	}

	/**
	* PRIVATE METHODS
	*/

	/**
	* PUBLIC METHODS
	*/

	GraphIterator::GraphIterator(const GraphIterator& it)
	{
		if (this != &it)
		{
			*this = it;
		}
	}

	GraphIterator& GraphIterator::operator=(const GraphIterator& it)
	{
		if (this != &it)
		{
			this->clear();
			this->node = it.node;
			this->end = it.end;

			this->inputNodes = it.inputNodes;
		}

		return *this;
	}

	// Compares by pointer
	bool GraphIterator::operator==(const GraphIterator& it) const
	{
		return this->node == it.node;
	}

	// Returns the actual node
	IProcessable::nPtr GraphIterator::operator*()
	{
		return this->node;
	}

	// Sets the start node of the BFS
	// It is the responsibility of the caller that startnode is part of base graph of this iterator
	bool GraphIterator::setStartNode(IProcessable::nPtr startNode)
	{
		this->clear();
		this->node = startNode;
		return this->moveNext();
	}
}
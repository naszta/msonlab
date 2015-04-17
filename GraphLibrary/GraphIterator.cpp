#include "GraphIterator.h"

namespace msonlab
{
	GraphIterator::GraphIterator(NodePtr node, NodePtr end)
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

	// Compares by pointer
	bool GraphIterator::operator==(const GraphIterator& it) const
	{
		return this->node == it.node;
	}

	bool GraphIterator::operator!=(const GraphIterator& it) const
	{
		return this->node != it.node;
	}

	// Returns the actual node
	NodePtr GraphIterator::operator*() const
	{
		return this->node;
	}

	// Sets the start node of the BFS
	// It is the responsibility of the caller that startnode is part of base graph of this iterator
	bool GraphIterator::setStartNode(NodePtr startNode)
	{
		this->clear();
		this->node = startNode;
		return this->moveNext();
	}
}
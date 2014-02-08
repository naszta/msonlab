#include "GraphIterator.h"

namespace msonlab
{
	GraphIterator::GraphIterator(IProcessable::nPtr node, IProcessable::nPtr end)
		: node(node), end(end)
	{
	}

	GraphIterator::GraphIterator(Graph::gPtr g) : graph(g)
	{
	}

	/**
	* PRIVATE METHODS
	*/

	/**
	* PUBLIC METHODS
	*/

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
	bool GraphIterator::setStartNode(IProcessable::nPtr startNode)
	{
		msonlab::IProcessable::nVect::iterator it;
		it = std::find(this->graph->nodes.begin(),this->graph->nodes.end(), startNode);
		
		if (it == this->graph->nodes.end())
		{
			return false;
		}
		
		this->clear();
		this->node = startNode;
		return this->moveNext();
	}
}
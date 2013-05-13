#include "GraphIterator.h"

namespace msonlab
{
	GraphIterator::GraphIterator(Graph::gPtr g) : graph(g)
	{
		// end of the iterator is unique for every graph
		// but the same for every iterator on the same graph
		this->end = g->iteratorEnd;

		// adding input nodes to the queue.
		IProcessable::nVect inputNodes = g->getInputNodes();
		if (inputNodes.size() > 0)
		{
			IProcessable::nVect::iterator it = inputNodes.begin();
			this->node = *it;
			++it; // can skip first from the queue
			for (; it != inputNodes.end(); ++it)
			{
				this->toVisit.push(*it);
			}
		}
		else
		{
			this->node = this->end;
		}
	}

	/**
	* PRIVATE METHODS
	*/

	// Clears the state of the visitor
	bool GraphIterator::clear()
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

	// Marks the neighbours of the actual node visited
	// so the bfs will skip them
	bool GraphIterator::skipActNode()
	{
		// adding neighbours to visited set
		IProcessable::eVect::iterator it;
		for (it = node->getPredecessors().begin(); it != node->getPredecessors().end(); ++it)
		{
			if (visited.count((*it)->getTo()) == 0)
			{
				visited.insert((*it)->getTo());
			}
		}

		return true;
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
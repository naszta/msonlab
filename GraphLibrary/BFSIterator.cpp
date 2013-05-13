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
				node = end;
				return false;
			}

			node = toVisit.front();
			toVisit.pop();
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

	// Clears the state of the visitor
	//bool BFSIterator::clear()
	//{
	//	this->node = this->end;
	//	std::queue<IProcessable::nPtr> emptyQueue;
	//	std::swap(this->toVisit, emptyQueue);
	//	std::set<IProcessable::nPtr> emptySet;
	//	std::swap(this->visited, emptySet);

	//	return true;
	//}

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

	// Compares by pointer
	//bool BFSIterator::operator==(const BFSIterator& it) const
	//{
	//	return this->node == it.node;
	//}

	// Returns the actual node
	//IProcessable::nPtr BFSIterator::operator*()
	//{
	//	return this->node;
	//}

	// Marks the neighbours of the actual node visited
	// so the bfs will skip them
	//bool BFSIterator::skipActNode()
	//{
	//	// adding neighbours to visited set
	//	IProcessable::eVect::iterator it;
	//	for (it = node->getPredecessors().begin(); it != node->getPredecessors().end(); ++it)
	//	{
	//		if (visited.count((*it)->getTo()) == 0)
	//		{
	//			visited.insert((*it)->getTo());
	//		}
	//	}

	//	return true;
	//}

	// Sets the start node of the BFS
	//bool BFSIterator::setStartNode(IProcessable::nPtr startNode)
	//{
	//	msonlab::IProcessable::nVect::iterator it;
	//	it = std::find(this->graph->nodes.begin(),this->graph->nodes.end(), startNode);
	//	
	//	if (it == this->graph->nodes.end())
	//	{
	//		return false;
	//	}
	//	
	//	this->clear();
	//	this->node = startNode;
	//	return this->moveNext();
	//}
}
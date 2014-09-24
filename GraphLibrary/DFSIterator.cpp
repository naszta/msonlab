#include "IProcessable.h"
#include "DFSIterator.h"
#include <stack>


namespace msonlab
{
	using std::stack;

	DFSIterator::DFSIterator(IProcessable::nPtr endPtr)
		:GraphIterator(endPtr, endPtr)
	{
	}

	DFSIterator::DFSIterator(const Graph& g)
	{
		// end of the iterator is unique for every graph
		// but the same for every iterator on the same graph
		this->end = g.iteratorEnd;

		// adding input nodes to the queue.
		IProcessable::nVect inputNodesVect = g.getInputNodes();
		if (inputNodesVect.size() > 0)
		{
			IProcessable::nVect::iterator it = inputNodesVect.begin();
			this->toDiscover.push(*it);
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
			GraphIterator::operator=(it);

			// copy state
			this->explored = it.explored;
			this->discovered = it.discovered;
			this->toDiscover = it.toDiscover;
		}

		return *this;
	}

	/**
	* PRIVATE METHODS
	*/

	bool DFSIterator::moveNext()
	{
		if (this->node == this->end)
		{
			return false;
		}

		IProcessable::nPtr tempNode = this->node;
		bool explored = false;
		while (!explored)
		{
			while (this->toDiscover.empty())
			{
				if (this->inputNodes.empty())
				{
					// no more node to visit
					this->node = this->end;
					return false;
				}

				this->toDiscover.push(this->inputNodes.front());
				this->inputNodes.pop();
			}

			tempNode = this->toDiscover.top();

			this->discovered.insert(tempNode);
			IProcessable::eVect neighbours = tempNode->getSuccessors();
			IProcessable::eVect::const_iterator cit;
			explored = true;
			for(cit = neighbours.cbegin(); cit != neighbours.cend(); ++cit)
			{
				if (this->discovered.count((*cit)->getTo()) == 0)
				{
					this->toDiscover.push((*cit)->getTo());
					explored = false;
				}
			}

			if (explored)
			{
				this->explored.insert(tempNode);
				this->node = tempNode;
				this->toDiscover.pop();
			}	
		}

		return explored;
	}

	bool DFSIterator::clear()
	{
		this->node = this->end;
		stack<IProcessable::nPtr> emptyStack;
		std::swap(this->toDiscover, emptyStack);
		IProcessable::nSet emptySet;
		std::swap(this->discovered, emptySet);
		IProcessable::nSet emptySet2;
		std::swap(this->explored, emptySet2);

		return true;
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


}
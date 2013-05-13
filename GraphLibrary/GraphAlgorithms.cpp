#include "GraphAlgorithms.h"
#include "BFSIterator.h"
#include <map>
#include <queue>
#include <set>

namespace msonlab
{
	IProcessable::nVect GraphAlgorithms::getTopologicalOrder(Graph::gPtr g)
	{
		IProcessable::nVect order;
		IProcessable::nVect allNodes = g->nodes;
		IProcessable::nVect inputNodes = g->getInputNodes();
		IProcessable::nVect::iterator it;
		std::map< IProcessable::nPtr , int> count;
		std::queue< IProcessable::nPtr> q;

		// pushing input nodes into a queue
		// nodes in this queue have no unprocessed predecessors
		for(it = inputNodes.begin(); it != inputNodes.end(); ++it)
		{
			q.push(*it);
		}

		while(!q.empty())
		{
			IProcessable::nPtr act = q.front();
			q.pop();
			order.push_back(act);
			IProcessable::eVect successors = act->getSuccessors();
			for (unsigned i = 0; i < successors.size(); ++i)
			{
				count[successors[i]->getTo()]++;
				if ( successors[i]->getTo()->getPredecessors().size() == count[successors[i]->getTo()])
				{
					q.push(successors[i]->getTo());
				}
			}
		}

		return order;
	}

	Graph::gPtr  GraphAlgorithms::getChangedGraph(Graph::gPtr g, IProcessable::nSet changed, IProcessable::nSet needed)
	{
		IProcessable::nVect topologicalOrder = getTopologicalOrder(g);
		IProcessable::nSet::iterator it;
		std::map<IProcessable::nPtr, int> topologicalOrderMap;

		// finding the the last needed element in the topological order
		int maxNodeTopValue = -1;
		for (int i = topologicalOrder.size()-1; i >= 0; --i)
		{
			topologicalOrderMap[topologicalOrder[i]] = i;
			if (needed.count(topologicalOrder[i]) > 0 && i > maxNodeTopValue)
			{
				maxNodeTopValue = i;
			}
		}

		// starting bfs from every changed node
		// stops if topological number is bigger than
		// the maximum of needed
		IProcessable::nSet modified;
		BFSIterator bfsItr (g);
		for( it = changed.cbegin(); it != changed.cend(); ++it)
		{
			bfsItr.setStartNode(*it);
			for(; bfsItr.hasMoreNode(); ++bfsItr)
			{
				if (modified.count(*bfsItr) > 0 || topologicalOrderMap[*bfsItr] > maxNodeTopValue)
				{
					bfsItr.skipActNode();
				}
				else
				{
					modified.insert(*bfsItr);
				}
			}
		}

		// collecting modified nodes
		Graph::gPtr changedGraph(new Graph());

		std::queue<IProcessable::nPtr> toVisit;
		IProcessable::nSet visited;
		IProcessable::nPtr node;
		IProcessable::eVect::iterator eIt;
		for (it = needed.begin(); it != needed.end(); ++it)
		{
			toVisit.push(*it);
		}

		while (!toVisit.empty())
		{
			do 
			{
				if (toVisit.empty())
				{
					break;
				}
				else 
				{
					node = toVisit.front();
					toVisit.pop();
				}
			}
			while (visited.count(node) > 0);

			IProcessable::eVect neighbours = node->getPredecessors();
			for (eIt = neighbours.begin(); eIt != neighbours.end(); ++eIt)
			{
				if (modified.count((*eIt)->getFrom()) > 0)
				{
					changedGraph->addEdge(*eIt);
					toVisit.push((*eIt)->getFrom());
				}
			}
		}

		return changedGraph;
	}
}
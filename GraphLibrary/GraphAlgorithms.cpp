#include "GraphAlgorithms.h"
#include "BFSIterator.h"
#include <map>
#include <queue>
#include <set>
<<<<<<< HEAD
#include <iostream>

namespace msonlab
{
	///
	// divides the nodes of the graph into levels
	// there are no edges between nodes on the same level
	// TODO: every node should be on the first possible level
	///
	vector<IProcessable::nVect> GraphAlgorithms::createLeveling(Graph::gPtr g)
	{
		IProcessable::nVect outputNodes = g->getOutputNodes();
		vector<IProcessable::nVect> result;
		result.push_back(outputNodes);
		std::map< IProcessable::nPtr , int> count;

		size_t added = outputNodes.size();
		IProcessable::nVect::iterator it;
		for (int level = 0; ; ++level) {
			result.push_back(IProcessable::nVect());
			for (it = result[level].begin(); it != result[level].end(); ++it)
			{
				IProcessable::nPtr act = *it;
				IProcessable::eVect predecessors = act->getPredecessors();
				for (size_t i = 0; i < predecessors.size(); ++i)
				{
					count[predecessors[i]->getFrom()]++;
					if ( predecessors[i]->getFrom()->getSuccessors().size() == count[predecessors[i]->getFrom()])
					{
						result[level+1].push_back(predecessors[i]->getFrom());
						++added;
					}
				}
			}

			if (added == g->nodes.size())
			{
				break;
			}
		}

		return result;
	}

	vector<IProcessable::nVect> GraphAlgorithms::createBottomLeveling(Graph::gPtr g)
	{
		IProcessable::nVect inputNodes = g->getInputNodes();
		vector<IProcessable::nVect> result;
		// first level, the input nodes
		result.push_back(inputNodes);
		std::map< IProcessable::nPtr , int> count;

		// this number of nodes added to the leveling
		size_t added = inputNodes.size();
		IProcessable::nVect::iterator it;
		for (int level = 0; ; ++level) {
			// adding a new level to the result
			result.push_back(IProcessable::nVect());
			for (it = result[level].begin(); it != result[level].end(); ++it)
			{
				IProcessable::nPtr act = *it;
				IProcessable::eVect successors = act->getSuccessors();
				for (size_t i = 0; i < successors.size(); ++i)
				{
					count[successors[i]->getTo()]++;
					if ( successors[i]->getTo()->getPredecessorsSize() == count[successors[i]->getTo()])
					{
						result[level+1].push_back(successors[i]->getTo());
						++added;
					}
				}
			}

			if (added == g->nodes.size())
			{
				break;
			}
		}

		//for (size_t i = 0; i < result.size(); ++i)
		//{
		//	std::cout << "level " << i << std::endl;
		//	for (it = result[i].begin(); it != result[i].end(); ++it)
		//	{
		//		IProcessable::nPtr act = *it;
		//		std::cout << act->getId() << ", ";
		//	}

		//	std::cout << std::endl;
		//}
		return result;
	}

=======

namespace msonlab
{
>>>>>>> gabooo
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
<<<<<<< HEAD
				if (changed.count((*eIt)->getTo()) == 0)
				{
					changedGraph->addEdge(*eIt);
				}

				if (modified.count((*eIt)->getFrom()) > 0)
				{
					toVisit.push((*eIt)->getFrom());
				}

=======
				if (modified.count((*eIt)->getFrom()) > 0)
				{
					changedGraph->addEdge(*eIt);
					toVisit.push((*eIt)->getFrom());
				}
>>>>>>> gabooo
			}
		}

		return changedGraph;
	}
<<<<<<< HEAD

	int GraphAlgorithms::scheduleGreedy(boost::shared_ptr<Graph> graph, int pus)
	{
		int timeCounter = 0;
		int taskCounter = 0;
		std::map< IProcessable::nPtr , int> count;
		IProcessable::nVect inputNodes = graph->getInputNodes();
		std::queue < IProcessable::nPtr> free;
		for (size_t i = 0; i < inputNodes.size(); ++i)
		{
			free.push(inputNodes[i]);
		}

		while (taskCounter < graph->numberOfNodes())
		{
			vector< IProcessable::nPtr > out;
			int limit = pus;
			while ( !free.empty() && limit > 0 )
			{
				out.push_back(free.front());
				free.pop();
				--limit;
				taskCounter++;
			}
			++timeCounter;
			if (taskCounter == graph->numberOfNodes())
			{
				break;
			}

			for (size_t i = 0; i < out.size(); ++i)
			{
				IProcessable::eVect successors = out[i]->getSuccessors();
				for (size_t j = 0; j < successors.size(); ++j)
				{
					count[successors[j]->getTo()]++;
					if ( successors[j]->getTo()->getPredecessorsSize() == count[successors[j]->getTo()])
					{
						free.push(successors[j]->getTo());
					}
				}	
			}
		}

		return timeCounter;
	}
=======
>>>>>>> gabooo
}
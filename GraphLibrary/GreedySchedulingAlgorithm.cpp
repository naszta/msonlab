#include "GreedySchedulingAlgorithm.h"
#include "GraphAlgorithms.h"
#include <map>
#include <queue>

namespace msonlab
{
	int GreedySchedulingAlgorithm::schedule(Graph::gPtr graph, int pus) const
	{
		int timeCounter = 0;
		unsigned taskCounter = 0;
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
				for (size_t i = 0; i < successors.size(); ++i)
				{
					count[successors[i]->getTo()]++;
					if ( successors[i]->getTo()->getPredecessorsSize() == count[successors[i]->getTo()])
					{
						free.push(successors[i]->getTo());
					}
				}	
			}
		}

		return timeCounter;
	}	
}
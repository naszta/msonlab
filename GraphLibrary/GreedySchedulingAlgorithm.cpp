#include "GreedySchedulingAlgorithm.h"
#include <map>
#include <queue>

namespace msonlab {
	namespace scheduling {

		GreedySchedulingAlgorithm GreedySchedulingAlgorithm::example{ examplar() };

		// schedules using greedy algorithm
		Solution::sPtr GreedySchedulingAlgorithm::schedule(const Graph &graph, Options::oPtr options) const
		{
			unsigned timeCounter = 0;
			unsigned taskCounter = 0;
			std::map< IProcessable::nPtr, int> count;
			IProcessable::nVect inputNodes = graph.getInputNodes();
			std::queue < IProcessable::nPtr> free;
			for (size_t i = 0; i < inputNodes.size(); ++i)
			{
				free.push(inputNodes[i]);
			}

			Solution::sPtr sol = std::make_shared<Solution>(graph.numberOfNodes(), options->getNumberOfPus(), graph.numberOfEdges());
			while (taskCounter < graph.numberOfNodes())
			{
				vector< IProcessable::nPtr > out;
				int limit = options->getNumberOfPus();
				while (!free.empty() && limit > 0)
				{
					IProcessable::nPtr node = free.front();
					sol->mapping[taskCounter] = limit - 1;
					sol->scheduling[taskCounter] = node;

					out.push_back(node);
					free.pop();
					--limit;
					taskCounter++;
				}
				++timeCounter;

				if (taskCounter == graph.numberOfNodes())
				{
					break;
				}

				for (size_t i = 0; i < out.size(); ++i)
				{
					IProcessable::eVect successors = out[i]->getSuccessors();
					for (size_t i = 0; i < successors.size(); ++i)
					{
						count[successors[i]->getTo()]++;
						if (successors[i]->getTo()->getPredecessorsSize() == count[successors[i]->getTo()])
						{
							free.push(successors[i]->getTo());
						}
					}
				}
			}

			return sol;
		}

		//virtual constructor
		SchedulingAlgorithm::ptr GreedySchedulingAlgorithm::build(Options::oPtr opt) const
		{
			if (opt->getAlgorithm().compare("greedy") == 0)
			{
				return std::move(std::make_unique<GreedySchedulingAlgorithm>());
			}

			return nullptr;
		}
	}
}
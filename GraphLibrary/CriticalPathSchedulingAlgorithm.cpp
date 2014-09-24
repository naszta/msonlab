#include "CriticalPathSchedulingAlgorithm.h"
#include "Edge.h"
#include "Algorithms.h"
#include <memory>
#include <algorithm>
#include <vector>

namespace msonlab {
	namespace scheduling {

		using std::shared_ptr;

		using namespace msonlab;

		CriticalPathSchedulingAlgorithm::CriticalPathSchedulingAlgorithm() {}

		//virtual constructor
		SchedulingAlgorithm::ptr CriticalPathSchedulingAlgorithm::build(Options::oPtr opt) const
		{
			if (opt->getAlgorithm().compare("criticalpath") == 0)
			{
				return std::move(std::make_unique<CriticalPathSchedulingAlgorithm>());
			}

			return nullptr;
		}

		void CriticalPathSchedulingAlgorithm::determineCosts(const Graph &graph, vector<unsigned>& costs) const 
		{
			if (costs.size() != graph.numberOfNodes()) {
				costs.resize(graph.numberOfNodes());
			}
			
			auto levels = graph::algorithms::partialTopologicalSort(graph);

			// finding the max time needed to compute (distance)
			// O(|V| + |E|)

			for (auto i = levels[0].begin(); i != levels[0].end(); ++i)
			{
				costs[(*i)->getId()] = (*i)->getComputationTime();
			}

			for (size_t i = 1; i < levels.size(); ++i)
			{
				for (auto& node : levels[i])
				{
					unsigned max = 0;
					unsigned count = node->getSuccessorsSize();
					for (auto& edge : node->getSuccessors()) {
						if (costs[edge->getToId()] > max) {
							max = costs[edge->getToId()];
						}
					}

					costs[node->getId()] = i + max + node->getComputationTime();
				}
			}
		}
	}
}
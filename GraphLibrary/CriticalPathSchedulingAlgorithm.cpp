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

		CriticalPathSchedulingAlgorithm CriticalPathSchedulingAlgorithm::example{ examplar() };

		//virtual constructor
		SchedulingAlgorithmPtr CriticalPathSchedulingAlgorithm::build(OptionsPtr opt) const
		{
			if (opt->getAlgorithm().compare("criticalPath") == 0)
			{
				return std::move(std::make_unique<CriticalPathSchedulingAlgorithm>());
			}

			return nullptr;
		}

		void CriticalPathSchedulingAlgorithm::determineCosts(const lw::lwgraph &graph, vector<unsigned>& costs) const 
		{
			if (costs.size() != graph.size()) {
				costs.resize(graph.size());
			}
			
			vector<vector<const lw::lwnode*>> levels;
			graph::algorithms::partialTopologicalSort<lw::lwgraph, const lw::lwnode*>(graph, levels);

			// finding the max time needed to compute (distance)
			// O(|V| + |E|)

			for (auto node : levels[0])
			{
				costs[node->id()] = node->cptime();
			}

			for (size_t i = 1; i < levels.size(); ++i)
			{
				for (auto node : levels[i])
				{
					unsigned max = 0;
					for (auto successor : node->successors()) {
						if (costs[successor->id()] > max) {
							max = costs[successor->id()];
						}
					}

					costs[node->id()] = i + max + node->cptime();
				}
			}
		}
	}
}
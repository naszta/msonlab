#include "CriticalPathSchedulingAlgorithm.h"
#include "Edge.h"
#include "Algorithms.h"
#include <memory>
#include <algorithm>
#include <vector>

namespace msonlab { namespace scheduling {

	using std::shared_ptr;
	using namespace msonlab;

	//virtual constructor
	SchedulingAlgorithmPtr CriticalPathSchedulingAlgorithm::build(OptionsPtr opt) const
	{
		if (opt->getAlgorithm().compare("criticalPath") == 0)
		{
			return std::move(std::make_unique<CriticalPathSchedulingAlgorithm>());
		}

		return nullptr;
	}

	void CriticalPathSchedulingAlgorithm::determineCosts(const lite::litegraph &graph, vector<unsigned>& costs) const 
	{
		if (costs.size() != graph.order()) {
			costs.resize(graph.order());
		}
			
		vector<vector<const lite::litenode*>> layers;
		graph::algorithms::constructLayeredOrder<lite::litegraph, const lite::litenode*>(graph, layers);

		// finding the max time needed to compute (distance)
		// O(|V| + |E|)

		for (const auto& node : layers[0])
		{
			costs[node->id()] = node->cptime();
		}

		for (size_t i = 1; i < layers.size(); ++i)
		{
			for (const auto& node : layers[i])
			{
				unsigned max = 0;
				for (const auto& successor : node->successors()) {
					if (costs[successor->id()] > max) {
						max = costs[successor->id()];
					}
				}

				costs[node->id()] = i + max + node->cptime();
			}
		}
	}
}}
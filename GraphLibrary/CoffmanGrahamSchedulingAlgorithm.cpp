#include "CoffmanGrahamSchedulingAlgorithm.h"
#include "Algorithms.h"
#include <algorithm>
#include "lwgraph.h"

namespace msonlab {
	namespace scheduling {

		using std::pair;
		using std::vector;
		using std::make_pair;

		using msonlab::lw::lwgraph;
		using msonlab::lw::lwnode;

		// lexigraphically compare
		bool vector_lexicographic(const pair<vector<unsigned>, const lwnode*>& a, const pair<vector<unsigned>, const lwnode*>& b)
		{
			size_t limit = std::min(a.first.size(), b.first.size());
			for (size_t i = 0; i < limit; ++i) {
				if (a.first[i] == b.first[i])
					continue;

				return a.first[i] < b.first[i];
			}

			return a.first.size() ==  b.first.size() ? a.second->id() > b.second->id() 
													 : a.first.size() < b.first.size();
		}

		bool decreasing(const unsigned& a, const unsigned& b) {
			return a > b;
		}

		void CoffmanGrahamSchedulingAlgorithm::determineCosts(const lwgraph &graph, vector<unsigned>& costs) const 
		{
			if (costs.size() != graph.order()) {
				costs.resize(graph.order());
			}

			vector<vector<const lwnode*>> levels;
			graph::algorithms::constructLayeredOrder<lwgraph, const lwnode*>(graph, levels);

			int counter = 0;
			//auto nodes = graph.order() - 1;
			for (const auto& node : levels[0]) {
				costs[node->id()] = counter;
				++counter;
			}

			// over all levels
			for (size_t i = 1; i < levels.size(); ++i) {
				vector<pair<vector<unsigned>, const lw::lwnode*>> order;
				unsigned top = 1;
				if (levels[i].size() > 1) {
					for (auto& node : levels[i]) {
						vector<unsigned> norder;
						// iterate over the successors
						for (auto successor : node->successors()) {
							// add successor points to list
							norder.push_back(costs[successor->id()]);
						}

						// sort in decreasing order
						std::sort(norder.begin(), norder.end(), decreasing);
						order.push_back(make_pair(norder, node));
					}

					std::sort(order.begin(), order.end(), vector_lexicographic);
					for (auto& pair : order) {
						costs[pair.second->id()] = counter;
						++counter;
					}
				}
				else {
					costs[levels[i][0]->id()] = counter;
					++counter;
				}
			}
		}

		//virtual constructor
		SchedulingAlgorithmPtr CoffmanGrahamSchedulingAlgorithm::build(OptionsPtr opt) const
		{
			if (opt->getAlgorithm().compare("coffman") == 0)
			{
				std::cout << "Initializing CoffmanGrahamSchedulingAlgorithm." << std::endl;
				return std::move(std::make_unique<CoffmanGrahamSchedulingAlgorithm>());
			}

			return nullptr;
		}
	}
}
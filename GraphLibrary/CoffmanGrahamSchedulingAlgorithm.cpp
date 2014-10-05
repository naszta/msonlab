#include "CoffmanGrahamSchedulingAlgorithm.h"
#include "Algorithms.h"
#include <algorithm>

namespace msonlab {
	namespace scheduling {

		using std::pair;
		using std::vector;
		using std::make_pair;

		CoffmanGrahamSchedulingAlgorithm CoffmanGrahamSchedulingAlgorithm::example{ examplar() };
		
		// lexigraphically compare
		bool vector_lexicographic(const pair<vector<unsigned>, NodePtr>& a, const pair<vector<unsigned>, NodePtr>& b)
		{
			size_t limit = std::min(a.first.size(), b.first.size());
			for (size_t i = 0; i < limit; ++i) {
				if (a.first[i] == b.first[i])
					continue;

				return a.first[i] < b.first[i];
			}

			return a.first.size() ==  b.first.size() ? a.second->getId() > b.second->getId() 
													 : a.first.size() < b.first.size();
		}

		bool decreasing(const unsigned& a, const unsigned& b) {
			return a > b;
		}

		void CoffmanGrahamSchedulingAlgorithm::determineCosts(const Graph &graph, vector<unsigned>& costs) const 
		{
			if (costs.size() != graph.numberOfNodes()) {
				costs.resize(graph.numberOfNodes());
			}

			auto levels = graph::algorithms::partialTopologicalSort(graph);

			int counter = 0;
			auto nodes = graph.numberOfNodes() - 1;
			for (auto& node : levels[0]) {
				costs[node->getId()] = counter;
				++counter;
			}

			// over all levels
			for (size_t i = 1; i < levels.size(); ++i) {
				vector<pair<vector<unsigned>, NodePtr>> order;
				unsigned top = 1;
				if (levels[i].size() > 1) {
					for (auto& node : levels[i]) {
						vector<unsigned> norder;
						// iterate over the successors
						for (auto& edge : node->getSuccessors()) {
							// add successor points to list
							norder.push_back(costs[edge->getToId()]);
						}

						// sort in decreasing order
						std::sort(norder.begin(), norder.end(), decreasing);
						order.push_back(make_pair(norder, node));
					}

					std::sort(order.begin(), order.end(), vector_lexicographic);
					for (auto& pair : order) {
						costs[pair.second->getId()] = counter;
						++counter;
					}
				}
				else {
					costs[levels[i][0]->getId()] = counter;
					++counter;
				}
			}
		}

		//virtual constructor
		SchedulingAlgorithm::ptr CoffmanGrahamSchedulingAlgorithm::build(OptionsPtr opt) const
		{
			if (opt->getAlgorithm().compare("coffman") == 0)
			{
				return std::move(std::make_unique<CoffmanGrahamSchedulingAlgorithm>());
			}

			return nullptr;
		}
	}
}
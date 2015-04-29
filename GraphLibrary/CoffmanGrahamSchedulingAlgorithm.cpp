#include "CoffmanGrahamSchedulingAlgorithm.h"
#include "Algorithms.h"
#include <algorithm>
#include "litegraph.h"

namespace msonlab { namespace scheduling {

	using std::pair;
	using std::vector;
	using std::make_pair;

	using msonlab::lite::litegraph;
	using msonlab::lite::litenode;

	// lexigraphically compare - comparing vector of integers. If they are the same, decision is made according to the id of the node.
	bool vector_lexicographic(const pair<vector<unsigned>, const litenode*>& a, const pair<vector<unsigned>, const litenode*>& b)
	{
		size_t limit = std::min(a.first.size(), b.first.size());
		for (size_t i = 0; i < limit; ++i) {
			if (a.first[i] == b.first[i])
				continue;

			return a.first[i] < b.first[i];
		}

		return a.first.size() == b.first.size() ? a.second->cptime() > b.second->cptime() 
												: a.first.size() < b.first.size();
	}

	// function used for sorting in decreasing order
	bool decreasing(const unsigned& a, const unsigned& b) {
		return a > b;
	}

	void CoffmanGrahamSchedulingAlgorithm::determineCosts(const litegraph &graph, vector<unsigned>& costs) const
	{
		if (costs.size() != graph.order()) {
			costs.resize(graph.order());
		}

		vector<vector<const litenode*>> layers;
		graph::algorithms::constructLayeredOrder<litegraph, const litenode*>(graph, layers);

		// order the first layer decreasing by computation time, so
		// node with higher computation time receives higher priority
		std::sort(begin(layers[0]), end(layers[0]), [](const litenode* n1, const litenode* n2)
		{return n1->cptime() > n2->cptime(); });
		// we can safely set the priorities of the output nodes, because no other nodes 
		unsigned priority = 0;
		for (const auto& node : layers[0]) {
			costs[node->id()] = priority;
			++priority;
		}

		// Setting the priority laer by layer
		// A node in the n+1th layer have to have higher rank, than
		// a node in the nth layer, because we place a node as close
		// to the output nodes as possible.
		for (size_t i = 1; i < layers.size(); ++i) {
			vector<pair<vector<unsigned>, const lite::litenode*>> order;
			if (layers[i].size() > 1) {
				for (auto& node : layers[i]) {
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
					costs[pair.second->id()] = priority;
					++priority;
				}
			}
			// if there is only one the priority is unambigous
			else {
				costs[layers[i][0]->id()] = priority;
				++priority;
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
}}
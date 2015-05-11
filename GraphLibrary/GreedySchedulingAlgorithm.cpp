#include "GreedySchedulingAlgorithm.h"
#include "litegraph.h"
#include "SchedulingUtils.h"
#include <map>
#include <queue>
#include <deque>

namespace msonlab { namespace scheduling {

	using namespace msonlab::lite;

	SchedulingResultPtr<const NodePtr> GreedySchedulingAlgorithm::schedule(const Graph &graph, const Options &options) const
	{
		if (graph.order() == 0) {
			return std::make_shared<SchedulingResult<const NodePtr>>(vector < unsigned > {}, vector < const NodePtr > {}, 0, options.getNumberOfPus());
		}

		litegraph liteg(graph);
		const auto &nodes = liteg.nodes();

		NodeVect hwnodes(graph.order());
		for (const auto& hwnode : graph.nodes()) {
			hwnodes[hwnode->id()] = hwnode;
		}

		unsigned taskCounter = 0;
		auto tasks = graph.order();

		// counts the available inputs of the nodess
		//std::map< unsigned, int> count;
		vector<unsigned> count(graph.order());
		vector<unsigned> RT(options.getNumberOfPus(), 0);

		// list of input nodes
		const auto &inputNodes = liteg.inodes();

		// the ready to process nodes, initialized with the input nodes
		std::queue < unsigned > free(std::deque< unsigned >(inputNodes.begin(), inputNodes.end()));
			
		vector<unsigned> mapping(tasks);
		NodeVect scheduling(tasks);
		// todo rewrite this
		while (taskCounter < liteg.order())
		{
			vector<unsigned> scheduled_node_ids;
			auto min_PU_id = std::distance(std::min_element(RT.begin(), RT.end()), RT.begin());
			auto node_id = free.front();
			mapping[taskCounter] = min_PU_id;
			scheduling[taskCounter] = hwnodes[node_id];
			RT[min_PU_id] += hwnodes[node_id]->cptime();
			scheduled_node_ids.push_back(node_id);
			free.pop();
			taskCounter++;

			// if all the nodes are scheduled, stop the algo
			if (taskCounter == graph.order())
			{
				break;
			}

			// udpate the free queue
			const auto& node = nodes[node_id];
			// iterate over the successors
			for (size_t i = 0; i < node.s_size(); ++i)
			{
				const auto suc_node = node.get_successor(i);
				auto processed_successors = ++count[suc_node->id()];
				if (suc_node->p_size() == processed_successors)
				{
					free.push(suc_node->id());
				}
			}
		}

		// calculate fitness here
		auto result = std::make_shared<SchedulingResult<const NodePtr>>(std::move(mapping), std::move(scheduling), 0, options.getNumberOfPus());
		auto length = computeLength<SchedulingResult<const NodePtr>>(*result, options);
		result->fitness(length);
		return result;
	}

	//virtual constructor
	SchedulingAlgorithmPtr GreedySchedulingAlgorithm::build(const Options& opt) const {
		if (opt.getAlgorithm().compare("greedy") == 0) {
			DEBUGLN("Initializing GreedySchedulingAlgorithm.");
			return std::move(std::make_unique<GreedySchedulingAlgorithm>());
		}

		return nullptr;
	}
}}
#include "ListSchedulingAlgorithm.h"
#include "Algorithms.h"
#include "SchedulingResult.h"
#include "SchedulingHelper.h"
#include <algorithm>
#include "lwgraph.h"
#include "lwnode.h"

namespace msonlab {
	namespace scheduling {

		using namespace lw;

		// costs matches the node id to the cost of the task
		// the higher the cost of the node, the sooner it should be executed
		unsigned ListSchedulingAlgorithm::findNextToSchedule(const vector<int>& dependencies, const vector<unsigned>& costs) const
		{
			return graph::algorithms::findMaxCostWithoutDependency(dependencies, costs);
		}

		void ListSchedulingAlgorithm::determineCosts(const lwgraph &graph, vector<unsigned>& costs) const 
		{
			if (costs.size() != graph.order()) {
				costs.resize(graph.order());
			}

			// the cost is simply the computation time.
			std::transform(begin(graph.nodes()), end(graph.nodes()), costs.begin(), [](const lwnode &node) { return node.cptime(); });
		}

		SchedulingResultPtr<const NodePtr> ListSchedulingAlgorithm::schedule(const Graph &hwgraph, const Options &options) const
		{
			lwgraph graph(hwgraph);
			vector<vector<const lwnode*>> levels;
			graph::algorithms::partialTopologicalSort<lwgraph, const lwnode*>(graph, levels);

			NodeVect hwnodes(hwgraph.order());
			for (const auto& hwnode : hwgraph.nodes()) {
				hwnodes[hwnode->id()] = hwnode;
			}

			auto tasks = graph.order();
			vector<unsigned> costs(tasks);

			// determine the costs of the nodes
			// the higher cost means earlier execution
			this->determineCosts(graph, costs);

			// counts the number of dependencies of each graph
			vector<int> dependencies(tasks);
			graph::algorithms::createDependencyVector<lw::lwgraph>(graph, dependencies);

			int comm = options.getCommOverhead();
			vector<unsigned> mapping(tasks);
			NodeVect scheduling(tasks);

			vector<unsigned> RT(options.getNumberOfPus());
			vector<unsigned> ST(tasks); // start time of the tasks
			vector<unsigned> FT(tasks); // finish time of the tasks
			vector<unsigned> DAT(options.getNumberOfPus()); // Data Arrival Time
			vector<unsigned> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j]
			int next = 0; // next scheduled node
			for (unsigned i = 0; i < tasks; ++i) {
				next = this->findNextToSchedule(dependencies, costs);
				const lw::lwnode* actNode = &graph.nodes()[next];

				// calculating data arrival time
				size_t predecessorSize = actNode->p_size();
				for (const auto& predecessor : actNode->predecessors())
				{
					unsigned id = predecessor->id();
					for (unsigned actPU = 0; actPU < options.getNumberOfPus(); ++actPU) {
						unsigned dat = FT[id] + (actPU != idPuMapping[id] ? comm : 0);
						DAT[actPU] = DAT[actPU] > dat ? DAT[actPU] : dat;
					}
				}

				for (unsigned actPU = 0; actPU < options.getNumberOfPus(); ++actPU) {
					DAT[actPU] = std::max(DAT[actPU], RT[actPU]);
				}

				int pu = std::distance(begin(DAT), std::min_element(begin(DAT), end(DAT)));
				ST[next] = DAT[pu];
				FT[next] = ST[next] + actNode->cptime();
				RT[pu] = FT[next];
				idPuMapping[next] = pu;
				mapping[i] = pu;
				scheduling[i] = hwnodes[actNode->id()];

				graph::algorithms::computeNextFreeNodes<const lw::lwnode*>(dependencies, actNode);
			}

			auto a = SchedulingResult < const NodePtr >(std::move(mapping), std::move(scheduling), 0);
			auto result = std::make_shared<SchedulingResult<const NodePtr>>(std::move(mapping), std::move(scheduling), 0);
			auto length = computeLength<SchedulingResult<const NodePtr>>(*result, options);
			result->fitness(length);
			return result;
		}
	}
}
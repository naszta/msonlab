#include "ListSchedulingAlgorithm.h"
#include "Algorithms.h"
#include "SchedulingResult.h"
#include "SchedulingHelper.h"
#include <algorithm>
#include "litegraph.h"
#include "litenode.h"

namespace msonlab {
	namespace scheduling {

		using namespace lite;

		// costs matches the node id to the cost of the task
		// the higher the cost of the node, the sooner it should be executed
		unsigned ListSchedulingAlgorithm::findNextToSchedule(const vector<int>& dependencies, const vector<unsigned>& costs) const
		{
			return graph::algorithms::findMaxCostWithoutDependency(dependencies, costs);
		}

		void ListSchedulingAlgorithm::determineCosts(const litegraph &graph, vector<unsigned>& costs) const 
		{
			if (costs.size() != graph.order()) {
				costs.resize(graph.order());
			}

			// the cost is simply the computation time.
			std::transform(begin(graph.nodes()), end(graph.nodes()), begin(costs), [](const litenode &node) { return node.cptime(); });
		}

		SchedulingResultPtr<const NodePtr> ListSchedulingAlgorithm::schedule(const Graph &hwgraph, const Options &options) const
		{
			if (hwgraph.order() == 0) {
				return std::make_shared<SchedulingResult<const NodePtr>>(vector < unsigned > {}, vector < const NodePtr > {}, 0);
			}

			litegraph graph(hwgraph);
			vector<vector<const litenode*>> levels;
			graph::algorithms::constructLayeredOrder(graph, levels);

			NodeVect hwnodes(hwgraph.order());
			for (const auto& hwnode : hwgraph.nodes()) {
				hwnodes[hwnode->id()] = hwnode;
			}

			auto tasks = graph.order();
			vector<unsigned> costs(tasks);

			// determine the costs of the nodes
			// the higher cost means earlier execution
			this->determineCosts(graph, costs);
			for (size_t i = 0; i < graph.order(); ++i)
			{
				std::wcout << i << " " << hwnodes[i]->getLabel() << " => " << costs[i] << std::endl;
			}

			// counts the number of dependencies of each graph
			vector<int> dependencies(tasks);
			graph::algorithms::createDependencyVector(graph, dependencies);

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
				const lite::litenode& actNode = graph.nodes()[next];

				// calculating data arrival time
				size_t predecessorSize = actNode.p_size();
				for (const auto& predecessor : actNode.predecessors())
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
				FT[next] = ST[next] + actNode.cptime();
				RT[pu] = FT[next];
				idPuMapping[next] = pu;
				mapping[i] = pu;
				scheduling[i] = hwnodes[actNode.id()];

				graph::algorithms::computeNextFreeNodes<const lite::litenode*>(dependencies, &actNode);
			}

			// find last finish time
			auto last_finish = *std::max_element(begin(FT), end(FT));

			//auto a = SchedulingResult < const NodePtr >(std::move(mapping), std::move(scheduling), 0);
			auto result = std::make_shared<SchedulingResult<const NodePtr>>(std::move(mapping), std::move(scheduling), 0);
			auto length = computeLength<SchedulingResult<const NodePtr>>(*result, options);
			std::cout << "LF=" << last_finish << ", length=" << length << std::endl;
			result->fitness(length);
			return result;
		}

		SchedulingAlgorithmPtr ListSchedulingAlgorithm::build(OptionsPtr opt) const
		{
			if (opt->getAlgorithm().compare("list") == 0)
			{
				return std::move(std::make_unique<ListSchedulingAlgorithm>());
			}

			return nullptr;
		}
	}
}
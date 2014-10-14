#include "ListSchedulingAlgorithm.h"
#include "Algorithms.h"
#include <algorithm>

namespace msonlab {
	namespace scheduling {
		// costs matches the node id to the cost of the task
		// the higher the cost of the node, the sooner it should be executed
		unsigned ListSchedulingAlgorithm::findNextToSchedule(const vector<int>& dependencies, const vector<unsigned>& costs) const
		{
			return graph::algorithms::findMaxCostWithoutDependency(dependencies, costs);
		}

		void ListSchedulingAlgorithm::determineCosts(const lw::lwgraph &graph, vector<unsigned>& costs) const 
		{
			if (costs.size() != graph.size()) {
				costs.resize(graph.size());
			}

			// the cost is simply the computation time.
			std::transform(graph.nodes().begin(), graph.nodes().end(), costs.begin(), [](lw::lwnode node) { return node.cptime(); });
			/*for (auto node : graph.nodes()) {
				costs[node.id()] = node.cptime();
			}*/
		}

		SolutionPtr ListSchedulingAlgorithm::schedule(const Graph &hwgraph, OptionsPtr options) const
		{
			lw::lwgraph graph(hwgraph);
			vector<vector<const lw::lwnode*>> levels;
			graph::algorithms::partialTopologicalSort<lw::lwgraph, const lw::lwnode*>(graph, levels);

			int tasks = graph.size();
			vector<unsigned> costs(tasks);

			// determine the costs of the nodes
			// the higher cost means earlier execution
			this->determineCosts(graph, costs);

			// counts the number of dependencies of each graph
			vector<int> dependencies(tasks);
			graph::algorithms::createDependencyVector<lw::lwgraph>(graph, dependencies);

			int comm = options->getCommOverhead();
			SolutionPtr result = std::make_shared<Solution>(tasks, options->getNumberOfPus(), graph.edge_size());
			vector<unsigned> RT(options->getNumberOfPus());
			vector<unsigned> ST(tasks); // start time of the tasks
			vector<unsigned> FT(tasks); // finish time of the tasks
			vector<unsigned> DAT(options->getNumberOfPus()); // Data Arrival Time
			vector<unsigned> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j]
			int next = 0;
			for (int i = 0; i < tasks; ++i) {
				next = this->findNextToSchedule(dependencies, costs);
				const lw::lwnode* actNode = &graph.nodes()[next];
				// calculating data arrival time
				size_t predecessorSize = actNode->p_size();
				for (unsigned j = 0; j < predecessorSize; ++j)
				{
					unsigned id = actNode->get_predecessor(j)->id();
					for (unsigned actPU = 0; actPU < options->getNumberOfPus(); ++actPU) {
						unsigned dat = FT[id] + (actPU != idPuMapping[id] ? comm : 0);
						DAT[actPU] = std::max(DAT[actPU], dat);
					}
				}

				for (unsigned actPU = 0; actPU < options->getNumberOfPus(); ++actPU) {
					DAT[actPU] = std::max(DAT[actPU], RT[actPU]);
				}

				int pu = std::distance(DAT.begin(), std::min_element(DAT.begin(), DAT.end()));
				ST[next] = DAT[pu];
				FT[next] = ST[next] + actNode->cptime();
				RT[pu] = FT[next];
				idPuMapping[next] = pu;
				result->_mapping[i] = pu;
				result->_scheduling[i] = actNode;

				graph::algorithms::computeNextFreeNodes<const lw::lwnode*>(dependencies, actNode);
			}

			return result;
		}
	}
}
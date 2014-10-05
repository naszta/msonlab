#include "ListSchedulingAlgorithm.h"
#include "Algorithms.h"

namespace msonlab {
	namespace scheduling {
		// costs matches the node id to the cost of the task
		// the higher the cost of the node, the sooner it should be executed
		unsigned ListSchedulingAlgorithm::findNextToSchedule(const vector<int>& dependencies, const vector<unsigned>& costs) const
		{
			return graph::algorithms::findMaxCostWithoutDependency(dependencies, costs);
		}

		void ListSchedulingAlgorithm::determineCosts(const Graph &graph, vector<unsigned>& costs) const {
			if (costs.size() != graph.numberOfNodes()) {
				costs.resize(graph.numberOfNodes());
			}

			// the cost is simply the computation time.
			for (auto node : graph.getNodes()) {
				costs[node->getId()] = node->getComputationTime();
			}
		}

		SolutionPtr ListSchedulingAlgorithm::schedule(const Graph &graph, OptionsPtr options) const
		{
			auto levels = graph::algorithms::partialTopologicalSort(graph);
			vector<unsigned> costs(graph.numberOfNodes());

			// determine the costs of the nodes
			// the higher cost means earlier execution
			this->determineCosts(graph, costs);

			// counts the number of dependencies of each graph
			vector<int> dependencies(graph.numberOfNodes());
			graph::algorithms::createDependencyVector(graph, dependencies);

			vector<NodePtr> nodes(graph.numberOfNodes());
			graph::algorithms::list_nodes(graph, nodes);

			int tasks = graph.numberOfNodes();
			int comm = options->getCommOverhead();
			SolutionPtr result = std::make_shared<Solution>(tasks, options->getNumberOfPus(), graph.numberOfEdges());
			vector<unsigned> RT(options->getNumberOfPus());
			vector<unsigned> ST(tasks); // start time of the tasks
			vector<unsigned> FT(tasks); // finish time of the tasks
			vector<unsigned> DAT(options->getNumberOfPus()); // Data Arrival Time
			vector<unsigned> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j]
			int next = 0;
			for (int i = 0; i < tasks; ++i) {
				next = this->findNextToSchedule(dependencies, costs);
				auto actNode = nodes[next];
				// calculating data arrival time
				size_t predecessorSize = actNode->getPredecessorsSize();
				for (unsigned j = 0; j < predecessorSize; ++j)
				{
					unsigned id = actNode->getPredecessor(j)->getFromId();
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
				FT[next] = ST[next] + nodes[next]->getComputationTime();
				RT[pu] = FT[next];
				idPuMapping[next] = pu;
				result->mapping[i] = pu;
				result->scheduling[i] = actNode;

				graph::algorithms::computeNextFreeNodes(dependencies, actNode);
			}

			return result;
		}
	}
}
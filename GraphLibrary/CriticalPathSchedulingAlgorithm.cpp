#include "CriticalPathSchedulingAlgorithm.h"
#include "Edge.h"
#include "Algorithms.h"
#include <memory>
#include <algorithm>
#include <vector>

namespace msonlab  {
	namespace scheduling {

		using std::shared_ptr;
		using std::cout;
		using std::endl;

		using namespace msonlab;

		unsigned CriticalPathSchedulingAlgorithm::findNextToSchedule(const vector<unsigned>& dependencies, const vector<unsigned>& distances) const
		{
			unsigned id;
			unsigned max = 0;
			for (unsigned i = 0; i < distances.size(); ++i) {
				if (dependencies[i] == 0 && (max == 0 || distances[i] > max)) {
					id = i;
					max = distances[i];
				}
			}

			return id;
		}

		Solution::sPtr CriticalPathSchedulingAlgorithm::schedule(Graph::gPtr& graph, Options::oPtr options) const
		{
			auto levels = graph::algorithms::partialTopologicalSort(graph);
			vector<unsigned> distance(graph->numberOfNodes());

			// finding the max time needed to compute (distance)
			// O(|V| + |E|)

			for (auto i = levels[0].begin(); i != levels[0].end(); ++i)
			{
				distance[(*i)->getId()] = (*i)->getComputationTime();
			}

			for (size_t i = 1; i < levels.size(); ++i)
			{
				for (auto& node : levels[i])
				{
					unsigned max = 0;
					for (auto& edge : node->getSuccessors()) {
						if (distance[edge->getToId()] > max) {
							max = distance[edge->getToId()];
						}
					}

					distance[node->getId()] = max + node->getComputationTime();
				}
			}

			// counts the number of dependencies of each graph
			vector<int> dependencies(graph->numberOfNodes());
			graph::algorithms::createDependencyVector(graph, dependencies);

			vector<Node::nPtr> nodes(graph->numberOfNodes());
			graph::algorithms::list_nodes(graph, nodes);

			int tasks = graph->numberOfNodes();
			int comm = options->getCommOverhead();
			Solution::sPtr result = std::make_shared<Solution>(tasks, options->getNumberOfPus(), graph->numberOfEdges());
			vector<unsigned> RT(options->getNumberOfPus());
			vector<unsigned> ST(tasks); // start time of the tasks
			vector<unsigned> FT(tasks); // finish time of the tasks
			vector<unsigned> DAT(options->getNumberOfPus()); // Data Arrival Time
			vector<unsigned> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j]
			int next = 0;
			for (int i = 0; i < tasks; ++i) {
				next = graph::algorithms::findMaxDistanceWithoutDependency(dependencies, distance);
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
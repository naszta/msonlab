#include "HusSchedulingAlgorithm.h"
#include "Edge.h"
#include <memory>
#include <algorithm>
#include <vector>

namespace msonlab 
{
	using std::shared_ptr;
	using std::cout;
	using std::endl;

	unsigned HusSchedulingAlgorithm::findNextToSchedule(const vector<unsigned>& dependencies, const vector<unsigned>& distances) const
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

	Chromosome::cPtr HusSchedulingAlgorithm::schedule(Graph::gPtr& graph, Options::oPtr options) const 
	{
		auto levels = algorithms.partialTopologicalSort(graph);
		vector<unsigned> distance(graph->numberOfNodes());

		for (auto i = levels[0].begin(); i != levels[0].end(); ++i)
		{
			distance[(*i)->getId()] = (*i)->getComputationTime();
		}

		// finding the max time needed to compute (distance)
		// O(|V| + |E|)
		for (size_t i = 1; i < levels.size(); ++i)
		{
			for (auto it = levels[i].begin(); it != levels[i].end(); ++it)
			{
				unsigned max = 0;
				for (auto it_e = (*it)->getSuccessors().begin();
					it_e != (*it)->getSuccessors().end(); ++it_e) {
					if (distance[(*it_e)->getToId()] > max) {
						max = distance[(*it_e)->getToId()];
					}
				}

				distance[(*it)->getId()] = max + (*it)->getComputationTime();
			}
		}

		vector<int> dependencies(graph->numberOfNodes());
		algorithms.createDependencyVector(graph, dependencies);

		vector<Node::nPtr> nodes(graph->numberOfNodes());
		GraphAlgorithms::listNodes(graph, nodes);

		int tasks = graph->numberOfNodes();
		int comm = options->getCommOverhead();
		Chromosome::cPtr result = std::make_shared<Chromosome>(tasks, options->getNumberOfPus());
		vector<unsigned> RT(options->getNumberOfPus());
		vector<unsigned> ST(tasks); // start time of the tasks
		vector<unsigned> FT(tasks); // finish time of the tasks
		vector<unsigned> DAT(options->getNumberOfPus()); // Data Arrival Time
		vector<unsigned> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j]
		int next = 0;
		for (int i = 0; i < tasks; ++i) {
			next = algorithms.findMaxDistanceWithoutDependency(dependencies, distance);
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

			// TODO: update distance vector
			algorithms.computeNextFreeNodes(dependencies, actNode);
		}

		return result;
	}
}
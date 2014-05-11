#include "HusSchedulingAlgorithm.h"
#include "Edge.h"
#include <memory>
#include <algorithm>

namespace msonlab 
{
	using std::shared_ptr;
	using std::cout;
	using std::endl;

	unsigned HusSchedulingAlgorithm::findNextToSchedule(const vector<unsigned>& dependencies, const vector<unsigned>& distances) const
	{
		unsigned id;
		unsigned max;
		for (unsigned i = 0; i < distances.size(); ++i) {
			if (dependencies[i] == 0 && (max == 0 || distances[i] > max)) {
				id = i;
				max = distances[i];
			}
		}

		return id;
	}

	Chromosome::cPtr HusSchedulingAlgorithm::schedule(Graph::gPtr graph, Options::oPtr options) const 
	{
		auto levels = algorithms.createLeveling(graph);
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

		// print out distance vector
		std::copy(distance.begin(), distance.end(), std::ostream_iterator<unsigned>(std::cout, " "));

		vector<int> dependencies(graph->numberOfNodes());
		algorithms.createDependencyVector(graph, dependencies);

		vector<Node::nPtr> nodes(graph->numberOfNodes());
		algorithms.listNodes(graph, nodes);

		int tasks = graph->numberOfNodes();
		int comm = options->getCommOverhead();
		cout << "tasks " << tasks << endl;
		cout << "comm " << comm << endl;
		Chromosome::cPtr result(new Chromosome(tasks, options->getNumberOfPus()));
		vector<unsigned> RT(options->getNumberOfPus());
		vector<unsigned> ST(tasks); // start time of the tasks
		vector<unsigned> FT(tasks); // finish time of the tasks
		vector<unsigned> DAT(options->getNumberOfPus()); // Data Arrival Time
		vector<unsigned> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j]
		int next = 0;
		for (int i = 0; i < tasks; ++i) {
			next = algorithms.findMaxDistanceWithoutDependency(dependencies, distance);
			cout << "next " << next << endl;
			auto actNode = nodes[next];
			unsigned st = 0;
			// calculating data arrival time
			printf_s("calculating data arrival time\n");
			size_t predecessorSize = actNode->getPredecessorsSize();
			for (unsigned j = 0; j < predecessorSize; ++j)
			{
				unsigned id = actNode->getPredecessor(j)->getFromId();
				for (unsigned actPU = 0; actPU < options->getNumberOfPus(); ++actPU) {
					unsigned dat = FT[id] + (actPU != idPuMapping[id] ? comm : 0);
					DAT[actPU] = std::max(DAT[actPU], dat);
					printf_s("DAT[%d]=%d\n", actPU, DAT[actPU]);
				}
			}

			for (unsigned actPU = 0; actPU < options->getNumberOfPus(); ++actPU) {
				DAT[actPU] = std::max(DAT[actPU], RT[actPU]);
				printf_s("DAT[%d]=%d\n", actPU, DAT[actPU]);
			}

			int pu = std::distance(DAT.begin(), std::min_element(DAT.begin(), DAT.end()));
			printf_s("Scheduling to %d\n", pu);
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
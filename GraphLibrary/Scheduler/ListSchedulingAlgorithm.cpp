#include <algorithm>
#include <stdexcept>
#include "ListSchedulingAlgorithm.h"
#include "../Algorithms.h"
#include "SchedulingResult.h"
#include "SchedulingUtils.h"
#include "../litegraph.h"
#include "../litenode.h"

namespace msonlab { namespace scheduling {

	using namespace lite;

	// costs matches the node id to the cost of the task
	// the higher the cost of the node, the sooner it should be executed
	unsigned ListSchedulingAlgorithm::findNextToSchedule(const vector<int>& dependencies, const vector<unsigned>& costs) const {
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

	SchedulingResultPtr<const lite::litenode* > ListSchedulingAlgorithm::scheduleLite(const lite::litegraph &graph, const Options &options) const {
		vector<vector<const litenode*>> levels;
		graph::algorithms::constructLayeredOrder(graph, levels);

		const auto TASKS = graph.order();
		vector<unsigned> costs(TASKS);

		// determine the costs of the nodes
		// the higher cost means earlier execution
		this->determineCosts(graph, costs);
		// counts the number of dependencies of each graph
		vector<int> dependencies(TASKS);
		graph::algorithms::createDependencyVector(graph, dependencies);

		const int comm = options.commOverhead();
		vector<unsigned> mapping(TASKS);
		vector<const litenode*> scheduling(TASKS);

		const unsigned MAX_PU = options.numberOfPus();
		vector<unsigned> RT(options.numberOfPus());
		vector<unsigned> ST(TASKS); // start time of the tasks
		vector<unsigned> FT(TASKS); // finish time of the tasks
		vector<unsigned> DAT(options.numberOfPus()); // Data Arrival Time
		vector<unsigned> idPuMapping(TASKS, MAX_PU); // idPuMapping[i] = j means that node[i] is processed by pu[j]
		for (unsigned i = 0; i < TASKS; ++i) {
			const auto next = this->findNextToSchedule(dependencies, costs);
			const auto& actNode = graph.nodes()[next];

			// calculating data arrival time
			//size_t predecessorSize = actNode.p_size();
			if (actNode.predecessors().empty()) {
				DAT = RT;
			}
			else {
				std::fill(begin(DAT), end(DAT), 0);
				for (const auto& predecessor : actNode.predecessors()) {
					unsigned id = predecessor->id();
					for (unsigned actPU = 0; actPU < options.numberOfPus(); ++actPU) {
						if (idPuMapping[id] == MAX_PU)
							throw std::logic_error("Non-scheduled predecessor");
						unsigned dat = FT[id] + (actPU != idPuMapping[id] ? comm : 0);
						DAT[actPU] = std::max({ dat, DAT[actPU], RT[actPU] });
					}
				}
			}

			auto pu = std::distance(begin(DAT), std::min_element(begin(DAT), end(DAT)));
			ST[next] = DAT[pu];
			FT[next] = ST[next] + actNode.cptime();
			RT[pu] = FT[next];
			idPuMapping[next] = pu;
			mapping[i] = pu;
			scheduling[i] = &graph.nodes()[actNode.id()];

			graph::algorithms::computeNextFreeNodes(dependencies, &actNode);
		}

		// find last finish time
		auto last_finish = *std::max_element(begin(FT), end(FT));
		return std::make_shared<SchedulingResult<const litenode*>>(std::move(mapping), std::move(scheduling), last_finish, options.numberOfPus());
	}

	SchedulingResultPtr<const NodePtr> ListSchedulingAlgorithm::schedule(const Graph &hwgraph, const Options &options) const
	{
		if (hwgraph.order() == 0) {
			return std::make_shared<SchedulingResult<const NodePtr>>(vector < unsigned > {}, vector < const NodePtr > {}, 0, options.numberOfPus());
		}

		litegraph graph(hwgraph);
		vector<vector<const litenode*>> levels;
		graph::algorithms::constructLayeredOrder(graph, levels);

		const auto& hwnodes = hwgraph.nodes();

		const auto TASKS = graph.order();
		vector<unsigned> costs(TASKS);

		// determine the costs of the nodes
		// the higher cost means earlier execution
		this->determineCosts(graph, costs);
		// counts the number of dependencies of each graph
		vector<int> dependencies(TASKS);
		graph::algorithms::createDependencyVector(graph, dependencies);

		const int comm = options.commOverhead();
		vector<unsigned> mapping(TASKS);
		NodeVect scheduling(TASKS);

		const unsigned MAX_PU = options.numberOfPus();
		vector<unsigned> RT(options.numberOfPus());
		vector<unsigned> ST(TASKS); // start time of the tasks
		vector<unsigned> FT(TASKS); // finish time of the tasks
		vector<unsigned> DAT(options.numberOfPus()); // Data Arrival Time
		vector<unsigned> idPuMapping(TASKS, MAX_PU); // idPuMapping[i] = j means that node[i] is processed by pu[j]
		for (unsigned i = 0; i < TASKS; ++i) {
			const auto next = this->findNextToSchedule(dependencies, costs);
			const auto& actNode = graph.nodes()[next];

			// calculating data arrival time
			//size_t predecessorSize = actNode.p_size();
			if (actNode.predecessors().empty()) {
				DAT = RT;
			}
			else {
				std::fill(begin(DAT), end(DAT), 0);
				for (const auto& predecessor : actNode.predecessors()) {
					unsigned id = predecessor->id();
					for (unsigned actPU = 0; actPU < options.numberOfPus(); ++actPU) {
						if (idPuMapping[id] == MAX_PU)
							throw std::logic_error("Non-scheduled predecessor");
						unsigned dat = FT[id] + (actPU != idPuMapping[id] ? comm : 0);
						DAT[actPU] = std::max({ dat, DAT[actPU], RT[actPU] });
					}
				}
			}

			auto pu = std::distance(begin(DAT), std::min_element(begin(DAT), end(DAT)));
			ST[next] = DAT[pu];
			FT[next] = ST[next] + actNode.cptime();
			RT[pu] = FT[next];
			idPuMapping[next] = pu;
			mapping[i] = pu;
			scheduling[i] = hwnodes[actNode.id()];

			graph::algorithms::computeNextFreeNodes(dependencies, &actNode);
		}

		// find last finish time
		auto last_finish = *std::max_element(begin(FT), end(FT));
		return std::make_shared<SchedulingResult<const NodePtr>>(std::move(mapping), std::move(scheduling), last_finish, options.numberOfPus());
	}

	SchedulingAlgorithmPtr ListSchedulingAlgorithm::build(const Options& opt) const {
		if (opt.algorithm().compare("list") == 0) {
			DEBUGLN("Initializing ListSchedulingAlgorithm.");
			return std::move(std::make_unique<ListSchedulingAlgorithm>());
		}

		return nullptr;
	}
}}
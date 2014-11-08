#include "GreedySchedulingAlgorithm.h"
#include "lwgraph.h"
#include "SchedulingHelper.h"
#include <map>
#include <queue>
#include <deque>

namespace msonlab {
	namespace scheduling {

		using namespace msonlab::lw;

		GreedySchedulingAlgorithm GreedySchedulingAlgorithm::example{ examplar() };

		// schedules using greedy algorithm
		SolutionPtr GreedySchedulingAlgorithm::schedule(const Graph &graph, OptionsPtr options) const
		{
			lwgraph lwg(graph);
			const vector<lwnode> &nodes = lwg.nodes();

			//auto hwnodes = graph.getNodes();

			unsigned timeCounter = 0;
			unsigned taskCounter = 0;
			vector<unsigned> schedule(lwg.size());
			
			// counts the available inputs of the nodess
			//std::map< NodePtr, int> count;
			std::map< unsigned, int> count;

			// list of input nodes
			//NodeVect inputNodes = graph.getInputNodes();
			auto inputNodes = lwg.inodes();

			// the ready to process nodes, initialized with the input nodes
			//std::queue < NodePtr> free;
			std::queue < unsigned > free( std::deque< unsigned >(inputNodes.begin(), inputNodes.end()) );

			//SolutionPtr sol = std::make_shared<Solution>(graph.numberOfNodes(), options->getNumberOfPus(), graph.numberOfEdges());
			SolutionPtr sol = std::make_shared<Solution>(lwg.size(), options->getNumberOfPus(), graph.numberOfEdges());
			//while (taskCounter < graph.numberOfNodes())
			while (taskCounter < lwg.size())
			{
				//vector< NodePtr > scheduled_nodes;
				vector<unsigned> scheduled_node_ids;
				int limit = options->getNumberOfPus();
				while (!free.empty() && limit > 0)
				{
					auto node_id = free.front();
					sol->_mapping[taskCounter] = limit - 1;
					sol->_scheduling[taskCounter] = &nodes[node_id];

					scheduled_node_ids.push_back(node_id);
					free.pop();
					--limit;
					taskCounter++;
				}
				++timeCounter;

				// if all the nodes are scheduled, stop the algo
				if (taskCounter == graph.numberOfNodes())
				{
					break;
				}

				// udpate the free queue
				//for (size_t i = 0; i < scheduled_nodes.size(); ++i)
				for (auto scheduled_node_id : scheduled_node_ids)
				{
					auto& node = nodes[scheduled_node_id];
					//EdgeVect successors = out[i]->getSuccessors();

					// iterate over the successors
					//for (size_t i = 0; i < successors.size(); ++i)
					for (size_t i = 0; i < node.s_size(); ++i)
					{
						auto suc_node = node.get_successor(i);
						auto processed_successors = ++count[suc_node->id()];
						if (suc_node->p_size() == processed_successors)
						{
							free.push(suc_node->id());
						}
					}
				}
			}

			return sol;
		}

		SchedulingResultPtr GreedySchedulingAlgorithm::schedule(const Graph &graph, const Options &options) const
		{
			lwgraph lwg(graph);
			const vector<lwnode> &nodes = lwg.nodes();

			const auto &hwnodes = graph.getNodes();

			unsigned timeCounter = 0;
			unsigned taskCounter = 0;
			auto tasks = graph.size();

			// counts the available inputs of the nodess
			std::map< unsigned, int> count;

			// list of input nodes
			const auto &inputNodes = lwg.inodes();

			// the ready to process nodes, initialized with the input nodes
			std::queue < unsigned > free(std::deque< unsigned >(inputNodes.begin(), inputNodes.end()));
			
			vector<unsigned> mapping(tasks);
			NodeVect scheduling(tasks);

			//while (taskCounter < graph.numberOfNodes())
			while (taskCounter < lwg.size())
			{
				//vector< NodePtr > scheduled_nodes;
				vector<unsigned> scheduled_node_ids;
				int limit = options.getNumberOfPus();
				while (!free.empty() && limit > 0)
				{
					auto node_id = free.front();
					mapping[taskCounter] = limit - 1;
					scheduling[taskCounter] = hwnodes[node_id];

					scheduled_node_ids.push_back(node_id);
					free.pop();
					--limit;
					taskCounter++;
				}
				++timeCounter;

				// if all the nodes are scheduled, stop the algo
				if (taskCounter == graph.numberOfNodes())
				{
					break;
				}

				// udpate the free queue
				//for (size_t i = 0; i < scheduled_nodes.size(); ++i)
				for (auto scheduled_node_id : scheduled_node_ids)
				{
					const auto& node = nodes[scheduled_node_id];

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
			}

			// calculate fitness here
			auto result = std::make_shared<SchedulingResult<NodePtr>>(std::move(mapping), std::move(scheduling), 0);
			auto length = computeLength<SchedulingResult<NodePtr>>(*result, options);
			result->fitness(length);
			return result;
		}

		//virtual constructor
		SchedulingAlgorithmPtr GreedySchedulingAlgorithm::build(OptionsPtr opt) const
		{
			if (opt->getAlgorithm().compare("greedy") == 0)
			{
				std::cout << "Initializing GreedySchedulingAlgorithm." << std::endl;
				return std::move(std::make_unique<GreedySchedulingAlgorithm>());
			}

			return nullptr;
		}
	}
}
#include "Algorithms.h"
#include "BFSIterator.h"
#include <queue>
#include <set>
//#include <map>
#include <memory>

namespace msonlab {
	namespace graph {
		namespace algorithms {
			using std::queue;
			using std::set;
			//using std::map;
			using std::unique_ptr;

			// creates partial topological order starting with the outputs
			//template <class GraphType, class NodeType>
			//void partialTopologicalSort(const GraphType &graph, vector<vector<NodeType>>& result)
			//{
			//	const vector<NodeType>& outputNodes = graph.onodes();
			//	result.push_back(outputNodes);
			//	std::map< NodeType, unsigned> count;

			//	size_t added = outputNodes.size();
			//	//NodeVect::iterator it;
			//	unsigned graphSize = graph.order();
			//	for (int level = 0; added < graphSize; ++level) {
			//		result.push_back(vector<NodeType>());
			//		for (auto act : result[level])
			//		{
			//			vector<NodeType> predecessors = act->nodes();
			//			//for (size_t i = 0; i < predecessors.size(); ++i)
			//			for (auto pre_node : predecessors)
			//			{
			//				count[pre_node]++;
			//				if (pre_node.s_size() == count[pre_node])
			//				{
			//					result[level + 1].push_back(pre_node);
			//					++added;
			//				}
			//			}
			//		}
			//	}

			//	return result;
			//}

			// creates partial topological order starting with the inputs
			vector<NodeVect> partialTopologicalSortFromBottom(const Graph &graph)
			{
				NodeVect inputNodes = graph.getInputNodes();
				vector<NodeVect> result;
				// first level, the input nodes
				result.push_back(inputNodes);
				std::map< NodePtr, unsigned> count;

				// this number of nodes added to the leveling
				size_t added = inputNodes.size();
				NodeVect::iterator it;
				unsigned graphSize = graph.getNodes().size();
				for (int level = 0; added < graphSize; ++level) {
					// adding a new level to the result
					result.push_back(NodeVect());
					for (auto& act : result[level])
					{
						EdgeVect successors = act->getSuccessors();
						for (size_t i = 0; i < successors.size(); ++i)
						{
							count[successors[i]->getTo()]++;
							if (successors[i]->getTo()->getPredecessorsSize() == count[successors[i]->getTo()])
							{
								result[level + 1].push_back(successors[i]->getTo());
								++added;
							}
						}
					}
				}

				return result;
			}

			// creates a topological sort from the graph
			NodeVect topologicalSort(const Graph &graph) {
				NodeVect order;
				NodeVect inputNodes = graph.getInputNodes();
				NodeVect::iterator it;
				std::map< NodePtr, unsigned> count;
				// initializing queue with the input nodes
				// nodes without dependencies
				std::queue< NodePtr, std::deque<NodePtr>> q(std::deque<NodePtr>(inputNodes.begin(), inputNodes.end()));

				while (!q.empty())
				{
					NodePtr act = q.front();
					q.pop();
					order.push_back(act);
					EdgeVect successors = act->getSuccessors();
					for (unsigned i = 0; i < successors.size(); ++i)
					{
						count[successors[i]->getTo()]++;
						if (successors[i]->getTo()->getPredecessors().size() == count[successors[i]->getTo()])
						{
							q.push(successors[i]->getTo());
						}
					}
				}

				return order;
			}

			Graph&& computeChangedGraph(const Graph &graph, NodeSet changed, NodeSet needed)
			{
				NodeVect topsort = topologicalSort(graph);
				NodeSet::iterator it;
				std::map<NodePtr, int> topologicalOrderMap;

				// finding the the last needed element in the topological order
				int maxNodeTopValue = -1;
				for (int i = topsort.size() - 1; i >= 0; --i)
				{
					topologicalOrderMap[topsort[i]] = i;
					if (needed.count(topsort[i]) > 0 && i > maxNodeTopValue)
					{
						maxNodeTopValue = i;
					}
				}

				// starting bfs from every changed node
				// stops if topological number is bigger than
				// the maximum of needed
				NodeSet modified;
				BFSIterator bfsItr(graph);
				for (it = changed.cbegin(); it != changed.cend(); ++it)
				{
					bfsItr.setStartNode(*it);
					for (; bfsItr.hasMoreNode(); ++bfsItr)
					{
						if (modified.count(*bfsItr) > 0 || topologicalOrderMap[*bfsItr] > maxNodeTopValue)
						{
							bfsItr.skipChildren();
						}
						else
						{
							modified.insert(*bfsItr);
						}
					}
				}

				std::queue<NodePtr> toVisit;
				NodeSet visited;
				NodePtr node;
				EdgeVect::iterator eIt;
				for (it = needed.begin(); it != needed.end(); ++it)
				{
					toVisit.push(*it);
				}

				// collecting modified nodes
				Graph changedgraph;

				while (!toVisit.empty())
				{
					do
					{
						if (toVisit.empty())
						{
							break;
						}
						else
						{
							node = toVisit.front();
							toVisit.pop();
						}
					} while (visited.count(node) > 0);

					EdgeVect neighbours = node->getPredecessors();
					for (eIt = neighbours.begin(); eIt != neighbours.end(); ++eIt)
					{
						if (changed.count((*eIt)->getTo()) == 0)
						{
							changedgraph.addEdge(*eIt);
						}

						if (modified.count((*eIt)->getFrom()) > 0)
						{
							toVisit.push((*eIt)->getFrom());
						}

					}
				}

				return std::move(changedgraph);
			}

			//template <class NodeType>
			//void computeNextFreeNodes(vector<int>& dependencies, NodeType node)
			//{
			//	for (auto successor : node->successors())
			//	{
			//		--dependencies[successor->id()];
			//	}

			//	dependencies[node->getId()] = -1;
			//}

			unsigned findMaxCostWithoutDependency(const vector<int>& dependencies, const vector<unsigned>& costs)
			{
				unsigned id = -1;
				unsigned max = 0;
				for (unsigned i = 0; i < costs.size(); ++i) {
					if (dependencies[i] == 0 && (id == -1 || costs[i] > max)) {
						id = i;
						max = costs[i];
					}
				}

				return id;
			}

			// sets the number of dependencies of each node in to the passed vector
			//template <class GraphType>
			//void createDependencyVector(const GraphType &graph, vector<int>& dependencies)
			//{
			//	if (dependencies.size() != graph.order()) {
			//		dependencies.resize(graph.order());
			//	}

			//	for (auto node : graph.nodes()) {
			//		dependencies[node.id()] = static_cast<int>(node.p_size());
			//	}
			//}

			void list_nodes(const Graph &graph, vector<NodePtr>& nodes) {
				if (nodes.size() != graph.order()) {
					nodes.resize(graph.order());
				}

				for (auto& node : graph.nodes()) {
					nodes[node->id()] = node;
				}
			}

			GraphPtr transitive_reduction(const GraphPtr& g) {
				unsigned reductions = 0;
				GraphPtr g2 = std::make_unique<Graph>();
				for (auto& node : g->getNodes()) {
					g2->addNode(node->clone());
				}

				unsigned id = g->order();

				BFSIterator itr(*g);

				for (auto& node : g->getNodes()) {
					map < NodePtr, EdgePtr> suc_map;
					// add all successors to the set
					for (auto& suc : node->getSuccessors()) {
						suc_map.insert(std::make_pair(suc->getTo(), suc));
					}

					for (auto& edge : node->getSuccessors()) {
						itr.setStartNode(edge->getTo());
						while (itr.hasMoreNode())
						{
							if (itr.depth() > 0 && suc_map.count(*itr) > 0) {
								suc_map.erase(*itr);
								++reductions;
							}

							++itr;
						}
					}

					for (auto& suc : suc_map) {
						auto edge = std::make_shared<Edge>(id, suc.second->getLabel(), suc.second->getValue(), node, suc.first);
						g2->addEdge(edge);
						++id;
					}

				}

				std::cout << "#" << reductions << " reductions made.\n";
				return g2;
			}

			unique_ptr<set<NodePtr>> get_all_dependencies(const NodePtr& node) {
				auto result = std::make_unique<set<NodePtr>>();

				return result;
			}
		}
	}
}
#include "Algorithms.h"
#include "BFSIterator.h"
#include <queue>
#include <set>
#include <map>
#include <memory>
#include <climits>

namespace msonlab {
	namespace graph {
		namespace algorithms {
			using std::queue;
			using std::set;
			using std::map;
			using std::unique_ptr;

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

			Graph computeChangedGraph(const Graph &graph, NodeSet changed, NodeSet needed)
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
				auto bfsItr = graph.bfs().begin();
				auto bfsEndItr = graph.bfs().end();
				for (it = changed.cbegin(); it != changed.cend(); ++it)
				{
					bfsItr.setStartNode(*it);
					for (; bfsItr != bfsEndItr; ++bfsItr)
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
				EdgeVect newEdges;
				NodeVect newNodes;
				// mapping the old node id-s to the new one
				vector<unsigned> idMapping(graph.order(), graph.order()+1);
				unsigned id_counter = 0;

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
					for (const auto& edge : neighbours)
					{
						if (changed.count(edge->getTo()) == 0)
						{
							NodePtr to = nullptr;
							if (idMapping[edge->getToId()] > graph.order()) {
								// new node not yet created
								to = edge->getTo()->clone();
								to->_id = id_counter++;
								newNodes.push_back(to);
								idMapping[edge->getToId()] = to->id();
							}
							else {
								to = newNodes[idMapping[edge->getToId()]];
							}
							NodePtr from = nullptr;
							if (idMapping[edge->getFromId()] > graph.order()) {
								// new node not yet created
								from = edge->getFrom()->clone();
								from->_id = id_counter++;
								newNodes.push_back(from);
								idMapping[edge->getFromId()] = from->id();
							}
							else {
								from = newNodes[idMapping[edge->getFromId()]];
							}
							auto newEdge = std::make_shared<Edge>(edge->id(), edge->getLabel(), edge->getValue(), from, to);
							changedgraph.addEdge(newEdge);
						}

						if (modified.count(edge->getFrom()) > 0)
						{
							toVisit.push(edge->getFrom());
						}

					}
				}

				return changedgraph;
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
				unsigned id = UINT_MAX;
				unsigned max = 0;
				for (unsigned i = 0; i < costs.size(); ++i) {
					if (dependencies[i] == 0 && (id == UINT_MAX || costs[i] > max)) {
						id = i;
						max = costs[i];
					}
				}

				return id;
			}

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
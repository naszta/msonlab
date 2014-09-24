#include "Algorithms.h"
#include "BFSIterator.h"
#include <queue>
#include <set>
#include <map>
#include <memory>

namespace msonlab {
	namespace graph {
		namespace algorithms {
			using std::queue;
			using std::set;
			using std::map;
			using std::unique_ptr;

			// creates partial topological order starting with the outputs
			vector<IProcessable::nVect> partialTopologicalSort(const Graph &graph)
			{
				IProcessable::nVect outputNodes = graph.getOutputNodes();
				vector<IProcessable::nVect> result;
				result.push_back(outputNodes);
				std::map< IProcessable::nPtr, unsigned> count;

				size_t added = outputNodes.size();
				IProcessable::nVect::iterator it;
				unsigned graphSize = graph.getNodes().size();
				for (int level = 0; added < graphSize; ++level) {
					result.push_back(IProcessable::nVect());
					for (auto& act : result[level])
					{
						IProcessable::eVect predecessors = act->getPredecessors();
						for (size_t i = 0; i < predecessors.size(); ++i)
						{
							count[predecessors[i]->getFrom()]++;
							if (predecessors[i]->getFrom()->getSuccessors().size() == count[predecessors[i]->getFrom()])
							{
								result[level + 1].push_back(predecessors[i]->getFrom());
								++added;
							}
						}
					}
				}

				return result;
			}

			// creates partial topological order starting with the inputs
			vector<IProcessable::nVect> partialTopologicalSortFromBottom(const Graph &graph)
			{
				IProcessable::nVect inputNodes = graph.getInputNodes();
				vector<IProcessable::nVect> result;
				// first level, the input nodes
				result.push_back(inputNodes);
				std::map< IProcessable::nPtr, unsigned> count;

				// this number of nodes added to the leveling
				size_t added = inputNodes.size();
				IProcessable::nVect::iterator it;
				unsigned graphSize = graph.getNodes().size();
				for (int level = 0; added < graphSize; ++level) {
					// adding a new level to the result
					result.push_back(IProcessable::nVect());
					for (auto& act : result[level])
					{
						IProcessable::eVect successors = act->getSuccessors();
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
			IProcessable::nVect topologicalSort(const Graph &graph) {
				IProcessable::nVect order;
				IProcessable::nVect inputNodes = graph.getInputNodes();
				IProcessable::nVect::iterator it;
				std::map< IProcessable::nPtr, unsigned> count;
				// initializing queue with the input nodes
				// nodes without dependencies
				std::queue< IProcessable::nPtr, std::deque<IProcessable::nPtr>> q(std::deque<IProcessable::nPtr>(inputNodes.begin(), inputNodes.end()));

				while (!q.empty())
				{
					IProcessable::nPtr act = q.front();
					q.pop();
					order.push_back(act);
					IProcessable::eVect successors = act->getSuccessors();
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

			Graph&& computeChangedGraph(const Graph &graph, IProcessable::nSet changed, IProcessable::nSet needed)
			{
				IProcessable::nVect topsort = topologicalSort(graph);
				IProcessable::nSet::iterator it;
				std::map<IProcessable::nPtr, int> topologicalOrderMap;

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
				IProcessable::nSet modified;
				BFSIterator bfsItr(graph);
				for (it = changed.cbegin(); it != changed.cend(); ++it)
				{
					bfsItr.setStartNode(*it);
					for (; bfsItr.hasMoreNode(); ++bfsItr)
					{
						if (modified.count(*bfsItr) > 0 || topologicalOrderMap[*bfsItr] > maxNodeTopValue)
						{
							bfsItr.skipActNode();
						}
						else
						{
							modified.insert(*bfsItr);
						}
					}
				}

				std::queue<IProcessable::nPtr> toVisit;
				IProcessable::nSet visited;
				IProcessable::nPtr node;
				IProcessable::eVect::iterator eIt;
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

					IProcessable::eVect neighbours = node->getPredecessors();
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

			void computeNextFreeNodes(vector<int>& dependencies, Node::nPtr node)
			{
				for (auto& edge : node->getSuccessors())
				{
					unsigned id = edge->getToId();
					--dependencies[id];
				}

				dependencies[node->getId()] = -1;
			}

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

			void createDependencyVector(const Graph &graph, vector<int>& dependencies)
			{
				if (dependencies.size() != graph.numberOfNodes()) {
					dependencies.resize(graph.numberOfNodes());
				}

				for (auto& node : graph.getNodes()) {
					dependencies[node->getId()] = node->getPredecessorsSize();
				}
			}

			void list_nodes(const Graph &graph, vector<Node::nPtr>& nodes) {
				if (nodes.size() != graph.numberOfNodes()) {
					nodes.resize(graph.numberOfNodes());
				}

				for (auto& node : graph.getNodes()) {
					nodes[node->getId()] = node;
				}
			}

			Graph::gPtr transitive_reduction(const Graph::gPtr& g) {
				unsigned reductions = 0;
				Graph::gPtr g2 = std::make_unique<Graph>();
				for (auto& node : g->getNodes()) {
					g2->addNode(node->clone());
				}

				unsigned id = g->numberOfNodes();

				BFSIterator itr(*g);

				for (auto& node : g->getNodes()) {
					map < Node::nPtr, Edge::ePtr> suc_map;
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

			unique_ptr<set<Node::nPtr>> get_all_dependencies(const Node::nPtr& node) {
				auto result = std::make_unique<set<Node::nPtr>>();

				return result;
			}
		}
	}
}
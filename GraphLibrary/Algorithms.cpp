#include "Algorithms.h"
#include "BFSIterator.h"
#include <queue>

namespace msonlab {
	namespace graph {
		namespace algorithms {
			using std::queue;

			// creates partial topological order starting with the outputs
			vector<IProcessable::nVect> partialTopologicalSort(const Graph::gPtr& graph)
			{
				IProcessable::nVect outputNodes = graph->getOutputNodes();
				vector<IProcessable::nVect> result;
				result.push_back(outputNodes);
				std::map< IProcessable::nPtr, unsigned> count;

				size_t added = outputNodes.size();
				IProcessable::nVect::iterator it;
				unsigned graphSize = graph->getNodes().size();
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
			vector<IProcessable::nVect> partialTopologicalSortFromBottom(const Graph::gPtr& graph) 
			{
				IProcessable::nVect inputNodes = graph->getInputNodes();
				vector<IProcessable::nVect> result;
				// first level, the input nodes
				result.push_back(inputNodes);
				std::map< IProcessable::nPtr, unsigned> count;

				// this number of nodes added to the leveling
				size_t added = inputNodes.size();
				IProcessable::nVect::iterator it;
				unsigned graphSize = graph->getNodes().size();
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
			IProcessable::nVect topologicalSort(const Graph::gPtr& graph) {
				IProcessable::nVect order;
				IProcessable::nVect inputNodes = graph->getInputNodes();
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

			Graph::gPtr computeChangedGraph(const Graph::gPtr& graph, IProcessable::nSet changed, IProcessable::nSet needed)
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
				BFSIterator bfsItr(*graph);
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

				// collecting modified nodes
				Graph::gPtr changedGraph = std::make_unique<Graph>();

				std::queue<IProcessable::nPtr> toVisit;
				IProcessable::nSet visited;
				IProcessable::nPtr node;
				IProcessable::eVect::iterator eIt;
				for (it = needed.begin(); it != needed.end(); ++it)
				{
					toVisit.push(*it);
				}

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
							changedGraph->addEdge(*eIt);
						}

						if (modified.count((*eIt)->getFrom()) > 0)
						{
							toVisit.push((*eIt)->getFrom());
						}

					}
				}

				return changedGraph;
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

			unsigned findMaxDistanceWithoutDependency(const vector<int>& dependencies, const vector<unsigned>& distances)
			{
				unsigned id = -1;
				unsigned max = 0;
				for (unsigned i = 0; i < distances.size(); ++i) {
					if (dependencies[i] == 0 && (id == -1 || distances[i] > max)) {
						id = i;
						max = distances[i];
					}
				}

				return id;
			}

			void createDependencyVector(const Graph::gPtr& graph, vector<int>& dependencies)
			{
				if (dependencies.size() != graph->numberOfNodes()) {
					dependencies.resize(graph->numberOfNodes());
				}

				for (auto& node : graph->getNodes()) {
					dependencies[node->getId()] = node->getPredecessorsSize();
				}
			}

			void list_nodes(const Graph::gPtr& graph, vector<Node::nPtr>& nodes) {
				if (nodes.size() != graph->numberOfNodes()) {
					nodes.resize(graph->numberOfNodes());
				}

				for (auto& node : graph->getNodes()) {
					nodes[node->getId()] = node;
				}
			}
		}
	}
}
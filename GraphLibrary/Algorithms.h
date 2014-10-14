#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H



#include "Graph.h"
#include "lwgraph.h"
#include <vector>
#include <map>

namespace msonlab {
	namespace graph {
		namespace algorithms {
			using std::map;
			using std::vector;
			using msonlab::Graph;

			///
			/// Splits the nodes of the graph into levels.
			///
			/// Nodes on the same level cannot be connected with an edge.
			/// Each node is as close to the output nodes as possible. 
			///
			/// @param graph The input graph.
			/// @return vector of vectors containing the nodes.
			template <class GraphType, class NodeType>
			void partialTopologicalSort(const GraphType &graph, vector<vector<NodeType>>& result);
			//vector<NodeVect> partialTopologicalSort(const lw::lwgraph &graph);

			///
			/// Splits the nodes of the graph into levels.
			///
			/// Nodes on the same level cannot be connected with an edge.
			/// Each node is as close to the input nodes as possible. 
			///
			/// @param graph The input graph.
			/// @return vector of vectors containing the nodes.
			vector<NodeVect> partialTopologicalSortFromBottom(const Graph &graph);

			///
			/// Creates a topological order of the input graph.
			///
			/// A topological order is an ordering of the nodes. ..
			///
			/// @param graph The input graph.
			/// @return Vector of the graph's node in topological order.
			NodeVect topologicalSort(const Graph &graph);

			///
			/// With a given change in some nodes, this method computes the graph, that needs to be rerun
			/// to get the results of the needed nodes.
			///
			/// @param graph The input graph.
			/// @param changed Set of changed nodes.
			/// @param needed Set of nodes those values are needed.
			/// @return The graph to rerun.
			Graph&& computeChangedGraph(const Graph &graph, NodeSet changed, NodeSet needed);

			///
			/// Creates a dependency vector from the graph.
			///
			/// A dependency vector is described as the following. The i-th corresponds to the
			/// node with ID i. Each element contains an integer, which corresponds to the
			/// number of nodes the element depends on.
			///
			/// @param graph The input graph.
			/// @param dependencies The output vector.
			template <class GraphType>
			void createDependencyVector(const GraphType &graph, vector<int>& dependencies);

			///
			/// Updates the dependency vector after processing a node.
			///
			/// @param dependencies The dependency vector.
			/// @param node The node processed.
			template <class NodeType>
			void computeNextFreeNodes(vector<int>& dependencies, NodeType node);

			///
			/// Finds the maximum of the distance vector amongst the indexes, where
			/// the dependcy value is zero.
			///
			/// @param dependencies The actual dependency vector.
			/// @param distances The distance of each node to the furthest output node.
			unsigned findMaxCostWithoutDependency(const vector<int>& dependencies, const vector<unsigned>& costs);

			///
			/// Puts the nodes into a vector.
			///
			/// Each node is placed in the vector by its id. As node[i] is the
			/// i-th element in the vector.
			/// @param graph The grpah its nodes to list.
			/// @param nodes The vector of nodes.
			void list_nodes(const Graph &graph, vector<NodePtr>& nodes);

			///
			/// This method calculates the length of execution using greedy scheduling.
			///
			/// @param graph The input graph.
			/// @param pus Number of PUs.
			/// @return The execution time.
			int scheduleGreedy(const Graph &graph, int pus);

			///
			/// Creates a new graph, with the transitive dependecies removed.
			/// 
			/// 
			GraphPtr transitive_reduction(const GraphPtr&);

//#include "Algorithms.cpp"

			template <class GraphType, class NodeType>
			void partialTopologicalSort(const GraphType &graph, vector<vector<NodeType>>& result)
			{
				const vector<NodeType>& outputNodes = graph.onodes();
				result.push_back(outputNodes);
				map< NodeType, unsigned> count;

				size_t added = outputNodes.size();
				//NodeVect::iterator it;
				unsigned graphSize = graph.size();
				for (int level = 0; added < graphSize; ++level) {
					result.push_back(vector<NodeType>());
					for (auto act : result[level])
					{
						const vector<NodeType> &predecessors = act->predecessors();
						//for (size_t i = 0; i < predecessors.size(); ++i)
						for (auto pre_node : predecessors)
						{
							count[pre_node]++;
							if (pre_node->s_size() == count[pre_node])
							{
								result[level + 1].push_back(pre_node);
								++added;
							}
						}
					}
				}
			}

			template <class NodeType>
			void computeNextFreeNodes(vector<int>& dependencies, NodeType node)
			{
				for (auto successor : node->successors())
				{
					--dependencies[successor->id()];
				}

				dependencies[node->id()] = -1;
			}

			template <class GraphType>
			void createDependencyVector(const GraphType &graph, vector<int>& dependencies)
			{
				if (dependencies.size() != graph.size()) {
					dependencies.resize(graph.size());
				}

				for (auto node : graph.nodes()) {
					dependencies[node.id()] = static_cast<int>(node.p_size());
				}
			}

		}
	}
}

#endif
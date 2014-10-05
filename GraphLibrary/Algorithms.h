#pragma once

#include "Graph.h"

namespace msonlab {
	namespace graph {
		namespace algorithms {
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
			vector<NodeVect> partialTopologicalSort(const Graph &graph);

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
			void createDependencyVector(const Graph &graph, vector<int>& dependencies);

			///
			/// Updates the dependency vector after processing a node.
			///
			/// @param dependencies The dependency vector.
			/// @param node The node processed.
			void computeNextFreeNodes(vector<int>& dependencies, NodePtr node);

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
		}
	}
}
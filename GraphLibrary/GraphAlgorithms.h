#pragma once
#include "Chromosome.h"
#include "Options.h"
#include <memory>
#include <vector>
#include "Graph.h"

namespace msonlab
{
	class GraphAlgorithms
	{
		static unsigned int doComputeLengthSTAndRT(Chromosome::ccPtr chromosome, Options::oPtr options,
		vector<unsigned>& ST, vector<unsigned>& RT);
	public:
		///
		/// Splits the nodes of the graph into levels.
		///
		/// Nodes on the same level cannot be connected with an edge.
		/// Each node is as close to the output nodes as possible. 
		///
		/// @param graph The input graph.
		/// @return vector of vectors containing the nodes.
		vector<IProcessable::nVect> partialTopologicalSort(const Graph::gPtr& graph) const;

		///
		/// Splits the nodes of the graph into levels.
		///
		/// Nodes on the same level cannot be connected with an edge.
		/// Each node is as close to the input nodes as possible. 
		///
		/// @param graph The input graph.
		/// @return vector of vectors containing the nodes.
		vector<IProcessable::nVect> partialTopologicalSortFromBottom(const Graph::gPtr& graph) const;

		///
		/// Creates a topological order of the input graph.
		///
		/// A topological order is an ordering of the nodes. ..
		///
		/// @param graph The input graph.
		/// @return Vector of the graph's node in topological order.
		IProcessable::nVect topologicalSort(const Graph::gPtr& graph) const;
		
		///
		/// With a given change in some nodes, this method computes the graph, that needs to be rerun
		/// to get the results of the needed nodes.
		///
		/// @param graph The input graph.
		/// @param changed Set of changed nodes.
		/// @param needed Set of nodes those values are needed.
		/// @return The graph to rerun.
		Graph::gPtr computeChangedGraph(const Graph::gPtr& graph, IProcessable::nSet changed, IProcessable::nSet needed) const;
		
		///
		/// Creates a dependency vector from the graph.
		///
		/// A dependency vector is described as the following. The i-th corresponds to the
		/// node with ID i. Each element contains an integer, which corresponds to the
		/// number of nodes the element depends on.
		///
		/// @param graph The input graph.
		/// @param dependencies The output vector.
		void createDependencyVector(const Graph::gPtr& graph, vector<int>& dependencies) const;

		///
		/// Updates the dependency vector after processing a node.
		///
		/// @param dependencies The dependency vector.
		/// @param node The node processed.
		void computeNextFreeNodes(vector<int>& dependencies, Node::nPtr node) const;

		///
		/// Finds the maximum of the distance vector amongst the indexes, where
		/// the dependcy value is zero.
		///
		/// @param dependencies The actual dependency vector.
		/// @param distances The distance of each node to the furthest output node.
		unsigned findMaxDistanceWithoutDependency(const vector<int>& dependencies, const vector<unsigned>& distances) const;

		///
		/// Puts the nodes into a vector.
		///
		/// Each node is placed in the vector by its id. As node[i] is the
		/// i-th element in the vector.
		/// @param graph The grpah its nodes to list.
		/// @param nodes The vector of nodes.
		static void listNodes(const Graph::gPtr& graph, vector<Node::nPtr>& nodes);

		///
		/// This method calculates the length of execution using greedy scheduling.
		///
		/// @param graph The input graph.
		/// @param pus Number of PUs.
		/// @return The execution time.
		int scheduleGreedy(const Graph::gPtr& graph, int pus) const;

		///
		/// Computes the length of the chromosome with the given option.
		///
		/// @param chromosome the result to use
		/// @param options the options to use
		/// @return the length
		static unsigned int computeLength(Chromosome::ccPtr chromosome, const Options::oPtr options);

		static unsigned int computeLengthAndST(Chromosome::ccPtr chromosome, const Options::oPtr options,
			vector<unsigned>& ST);

		static unsigned int computeLengthAndRT(Chromosome::ccPtr chromosome, const Options::oPtr options,
			vector<unsigned>& RT);

		static unsigned int computeLengthSTAndRT(Chromosome::ccPtr chromosome, const Options::oPtr options,
			vector<unsigned>& ST, vector<unsigned>& RT);

		static unsigned int computeLengthAndReuseIdleTime(Chromosome::cPtr& chromosome, const Options::oPtr& options);
	};
}
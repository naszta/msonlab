#pragma once
#include "Global.h"
#include "Graph.h"
#include "IProcessable.h"

namespace msonlab
{
	class GraphAlgorithms
	{
	public:
		///
		/// Splits the nodes of the graph into levels.
		///
		/// Nodes on the same level cannot be connected with an edge.
		/// Eash node is as close to the output nodes as possible. 
		///
		/// @param graph The input graph.
		/// @return vector of vectors containing the nodes.
		vector<IProcessable::nVect> createLeveling(const Graph::gPtr graph) const;

		///
		/// Splits the nodes of the graph into levels.
		///
		/// Nodes on the same level cannot be connected with an edge.
		/// Eash node is as close to the input nodes as possible. 
		///
		/// @param graph The input graph.
		/// @return vector of vectors containing the nodes.
		vector<IProcessable::nVect> createBottomLeveling(const Graph::gPtr graph) const;

		///
		/// Creates a topological order of the input graph.
		///
		/// A topological order is an ordering of the nodes. ..
		///
		/// @param graph The input graph.
		/// @return Vector of the graph's node in topological order.
		IProcessable::nVect createTopologicalOrder(const Graph::gPtr graph) const;
		
		///
		/// With a given change in some nodes, this method computes the graph, that needs to be rerun
		/// to get the results of the needed nodes.
		///
		/// @param graph The input graph.
		/// @param changed Set of changed nodes.
		/// @param needed Set of nodes those values are needed.
		/// @return The graph to rerun.
		Graph::gPtr computeChangedGraph(const Graph::gPtr graph, IProcessable::nSet changed, IProcessable::nSet needed) const;
		
		///
		/// This method calculates the length of execution using greedy scheduling.
		///
		/// @param graph The input graph.
		/// @param pus Number of PUs.
		/// @return The execution time.
		int scheduleGreedy(Graph::gPtr graph, int pus) const;
	};
}
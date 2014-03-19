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
		vector<IProcessable::nVect> createLeveling(Graph::gPtr graph);

		///
		/// Splits the nodes of the graph into levels.
		///
		/// Nodes on the same level cannot be connected with an edge.
		/// Eash node is as close to the input nodes as possible. 
		///
		/// @param graph The input graph.
		/// @return vector of vectors containing the nodes.
		vector<IProcessable::nVect> createBottomLeveling(Graph::gPtr graph);

		///
		/// Creates a topological order of the input graph.
		///
		/// A topological order is an ordering of the nodes. ..
		///
		/// @param The input graph.
		/// @return Vector of the graph's node in topological order.
		IProcessable::nVect createTopologicalOrder(Graph::gPtr graph);
		Graph::gPtr getChangedGraph(Graph::gPtr graph, IProcessable::nSet changed, IProcessable::nSet needed);
		int scheduleGreedy(std::shared_ptr<Graph> graph, int pus);
	};
}
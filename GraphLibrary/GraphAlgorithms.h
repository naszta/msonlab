#pragma once
#include "Global.h"
#include "Graph.h"
#include "IProcessable.h"

namespace msonlab
{

	class GraphAlgorithms
	{
	public:
		vector<IProcessable::nVect> createLeveling(Graph::gPtr g);
		vector<IProcessable::nVect> createBottomLeveling(Graph::gPtr g);
		IProcessable::nVect getTopologicalOrder(Graph::gPtr g);
		Graph::gPtr getChangedGraph(Graph::gPtr g, IProcessable::nSet changed, IProcessable::nSet needed);
		int scheduleGreedy(boost::shared_ptr<Graph> graph, int pus);
	};
}
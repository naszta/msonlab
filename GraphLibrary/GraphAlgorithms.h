#pragma once
#include "Global.h"
#include "Graph.h"
#include "IProcessable.h"

namespace msonlab
{

	class GraphAlgorithms
	{
	public:
		IProcessable::nVect getTopologicalOrder(Graph::gPtr g);
		Graph::gPtr getChangedGraph(Graph::gPtr g, IProcessable::nSet changed, IProcessable::nSet needed);
	};
}
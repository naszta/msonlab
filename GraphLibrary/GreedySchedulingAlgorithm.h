#pragma once

#include "Graph.h"

namespace msonlab
{
	class GreedySchedulingAlgorithm
	{
	public:
		int schedule(Graph::gPtr graph, int pus) const;
	};
}
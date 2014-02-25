#pragma once

#include "Graph.h"

namespace msonlab
{
	class GreedySchedulingAlgorithm
	{
	public:
		int schedule(std::shared_ptr<Graph> graph, int pus);
	};
}
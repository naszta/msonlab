#pragma once

#include <boost\enable_shared_from_this.hpp>
#include "Graph.h"

namespace msonlab
{
	class GreedySchedulingAlgorithm
	{
	public:
		int schedule(std::shared_ptr<Graph> graph, int pus);
	};
}
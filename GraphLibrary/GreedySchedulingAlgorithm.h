#pragma once

#include <boost\enable_shared_from_this.hpp>
#include "Graph.h"

namespace msonlab
{
	class GreedySchedulingAlgorithm
	{
	public:
		int schedule(boost::shared_ptr<Graph> graph, int pus);
	};
}
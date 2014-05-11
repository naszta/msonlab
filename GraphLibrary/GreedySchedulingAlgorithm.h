#pragma once

#include "SchedulingAlgorithm.h"

namespace msonlab
{
	class GreedySchedulingAlgorithm : SchedulingAlgorithm
	{
	public:
		Chromosome::cPtr schedule(Graph::gPtr graph, Options::oPtr options) const;
	};
}
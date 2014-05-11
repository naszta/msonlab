#pragma once

#include "SchedulingAlgorithm.h"

namespace msonlab
{
	class GreedySchedulingAlgorithm : public SchedulingAlgorithm
	{
	public:
		Chromosome::cPtr schedule(Graph::gPtr graph, Options::oPtr options) const;
	};
}
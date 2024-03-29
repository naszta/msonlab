#pragma once

#include "SchedulingAlgorithm.h"

namespace msonlab
{
	///
	/// This class implements a greedy scheduling of the graph.
	///
	/// Greedy scheduling means, it always schedules the next
	/// ready to process node to the next available PU.
	class GreedySchedulingAlgorithm : public SchedulingAlgorithm
	{
	public:
		Chromosome::cPtr schedule(Graph::gPtr& graph, Options::oPtr options) const;
	};
}
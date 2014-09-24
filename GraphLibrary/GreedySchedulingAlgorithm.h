#pragma once

#include "SchedulingAlgorithm.h"

namespace msonlab {
	namespace scheduling {

		using msonlab::Graph;

		///
		/// This class implements a greedy scheduling of the graph.
		///
		/// Greedy scheduling means, it always schedules the next
		/// ready to process node to the next available PU.
		class GreedySchedulingAlgorithm : public SchedulingAlgorithm
		{
		public:
			Solution::sPtr schedule(const Graph &graph, Options::oPtr options) const;
			virtual ptr build(Options::oPtr) const;
			virtual ~GreedySchedulingAlgorithm() = default;
		};
	}
}
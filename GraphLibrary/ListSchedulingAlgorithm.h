#pragma once
#include "SchedulingAlgorithm.h"
#include "lwgraph.h"

namespace msonlab {
	namespace scheduling {
		class ListSchedulingAlgorithm : public SchedulingAlgorithm {
		protected:
			virtual unsigned findNextToSchedule(const vector<int>& dependencies, const vector<unsigned>& costs) const;
			virtual void determineCosts(const lw::lwgraph &graph, vector<unsigned>& costs) const;
		public:
			SchedulingResultPtr schedule(const Graph &graph, const Options &options) const;
		};
	}
}
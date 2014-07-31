#pragma once
#include "SchedulingAlgorithm.h"

namespace msonlab {
	namespace scheduling {
		class ListSchedulingAlgorithm : public SchedulingAlgorithm {
		protected:
			virtual unsigned findNextToSchedule(const vector<int>& dependencies, const vector<unsigned>& costs) const;
			virtual void determineCosts(const Graph::gPtr& graph, vector<unsigned>& costs) const;
		public:
			Solution::sPtr schedule(Graph::gPtr& graph, Options::oPtr options) const;
		};
	}
}
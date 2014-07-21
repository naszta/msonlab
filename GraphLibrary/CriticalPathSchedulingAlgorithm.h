#ifndef HUS_SCHEDULERALGORITHM_H
#define HUS_SCHEDULERALGORITHM_H

#include "SchedulingAlgorithm.h"

namespace msonlab {
	namespace scheduling {

		using namespace msonlab;

		class CriticalPathSchedulingAlgorithm : public SchedulingAlgorithm
		{
			unsigned findNextToSchedule(const vector<unsigned>& dependencies, const vector<unsigned>& distances) const;

		public:
			Solution::sPtr schedule(Graph::gPtr& graph, Options::oPtr options) const;
		};
	}
}
#endif
#ifndef HUS_SCHEDULERALGORITHM_H
#define HUS_SCHEDULERALGORITHM_H

#include "SchedulingAlgorithm.h"
#include "GraphAlgorithms.h"

namespace msonlab {

	class HusSchedulingAlgorithm : public SchedulingAlgorithm
	{
		GraphAlgorithms algorithms;

		unsigned findNextToSchedule(const vector<unsigned>& dependencies, const vector<unsigned>& distances) const;

	public:
		Solution::sPtr schedule(Graph::gPtr& graph, Options::oPtr options) const;
	};

}
#endif
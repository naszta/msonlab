#ifndef HUS_SCHEDULERALGORITHM_H
#define HUS_SCHEDULERALGORITHM_H

#include "GraphAlgorithms.h"
#include "SchedulingAlgorithm.h"

namespace msonlab {

	class HusSchedulingAlgorithm : SchedulingAlgorithm
	{
		GraphAlgorithms algorithms;

		unsigned findNextToSchedule(const vector<unsigned>& dependencies, const vector<unsigned>& distances) const;

	public:
		Chromosome::cPtr schedule(Graph::gPtr graph, Options::oPtr options) const;
	};

}
#endif
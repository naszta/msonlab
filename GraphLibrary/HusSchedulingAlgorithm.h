#ifndef HUS_SCHEDULERALGORITHM_H
#define HUS_SCHEDULERALGORITHM_H

#include "SchedulingAlgorithm.h"

namespace msonlab {

	class HusSchedulingAlgorithm : public SchedulingAlgorithm
	{
		GraphAlgorithms algorithms;

		unsigned findNextToSchedule(const vector<unsigned>& dependencies, const vector<unsigned>& distances) const;

	public:
		Chromosome::cPtr schedule(Graph::gPtr graph, Options::oPtr options) const;
	};

}
#endif
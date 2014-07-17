#ifndef GRAPHLIB_SCHEDULERALGORITHM_H
#define GRAPHLIB_SCHEDULERALGORITHM_H

#include <memory>
#include "Solution.h"
#include "Graph.h"

namespace msonlab {
	namespace scheduling {
		
		using namespace msonlab;

		///
		/// Abstract class for schedule algorithms.
		///
		class SchedulingAlgorithm
		{
		public:
			typedef std::shared_ptr<SchedulingAlgorithm> algPtr;
			virtual Solution::sPtr schedule(Graph::gPtr& graph, Options::oPtr options) const = 0;
		};
	}
}

#endif
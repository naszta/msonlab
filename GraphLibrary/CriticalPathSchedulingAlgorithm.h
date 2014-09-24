#ifndef HUS_SCHEDULERALGORITHM_H
#define HUS_SCHEDULERALGORITHM_H

#include "ListSchedulingAlgorithm.h"

namespace msonlab {
	namespace scheduling {

		using namespace msonlab;

		class CriticalPathSchedulingAlgorithm : public ListSchedulingAlgorithm
		{
		protected:
			virtual void determineCosts(const Graph &graph, vector<unsigned>& costs) const;
		public:
			CriticalPathSchedulingAlgorithm();
			virtual SchedulingAlgorithm::ptr build(Options::oPtr) const;
			virtual ~CriticalPathSchedulingAlgorithm() = default;
		};
	}
}
#endif
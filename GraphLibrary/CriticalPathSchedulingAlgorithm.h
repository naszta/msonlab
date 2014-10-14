#ifndef HUS_SCHEDULERALGORITHM_H
#define HUS_SCHEDULERALGORITHM_H

#include "ListSchedulingAlgorithm.h"

namespace msonlab {
	namespace scheduling {

		using namespace msonlab;

		class CriticalPathSchedulingAlgorithm : public ListSchedulingAlgorithm
		{
		protected:
			virtual void determineCosts(const lw::lwgraph &graph, vector<unsigned>& costs) const;
		public:
			static CriticalPathSchedulingAlgorithm example;
			CriticalPathSchedulingAlgorithm() = default;
			CriticalPathSchedulingAlgorithm(examplar e) { SchedulingAlgorithm::add_scheduling_algorithm(this); }
			virtual SchedulingAlgorithm::ptr build(OptionsPtr) const;
			virtual ~CriticalPathSchedulingAlgorithm() = default;
		};
	}
}
#endif
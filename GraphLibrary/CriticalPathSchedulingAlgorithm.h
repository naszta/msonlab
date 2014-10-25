#ifndef HUS_SCHEDULERALGORITHM_H
#define HUS_SCHEDULERALGORITHM_H

#include "ListSchedulingAlgorithm.h"

namespace msonlab {
	namespace scheduling {

		using namespace msonlab;

		class CriticalPathSchedulingAlgorithm : public ListSchedulingAlgorithm
		{
			static CriticalPathSchedulingAlgorithm example;
		protected:
			virtual void determineCosts(const lw::lwgraph &graph, vector<unsigned>& costs) const;
		public:
			CriticalPathSchedulingAlgorithm() = default;
			CriticalPathSchedulingAlgorithm(examplar e) { SchedulingAlgorithm::add_scheduling_algorithm(this); }
			virtual SchedulingAlgorithmPtr build(OptionsPtr) const;
			virtual ~CriticalPathSchedulingAlgorithm() = default;
		};
	}
}
#endif
#ifndef COFFMANGRAHAM_SCHEDULERALGORITHM_H
#define COFFMANGRAHAM_SCHEDULERALGORITHM_H

#include "ListSchedulingAlgorithm.h"

namespace msonlab {
	namespace scheduling {

		using namespace msonlab;

		class CoffmanGrahamSchedulingAlgorithm : public ListSchedulingAlgorithm
		{
			static CoffmanGrahamSchedulingAlgorithm example;
			virtual void determineCosts(const lw::lwgraph& graph, vector<unsigned>& costs) const;
		public:
			CoffmanGrahamSchedulingAlgorithm() = default;
			CoffmanGrahamSchedulingAlgorithm(examplar e) { SchedulingAlgorithm::add_scheduling_algorithm(this); }
			virtual ptr build(OptionsPtr) const;
			virtual ~CoffmanGrahamSchedulingAlgorithm() = default;
		};
	}
}
#endif
#pragma once
#include "SchedulingAlgorithm.h"

namespace msonlab {
	namespace scheduling {
		class ListSchedulingAlgorithm : public SchedulingAlgorithm {
		protected:
			virtual unsigned findNextToSchedule(const vector<int>& dependencies, const vector<unsigned>& costs) const;
			virtual void determineCosts(const Graph &graph, vector<unsigned>& costs) const;
		public:
			SolutionPtr schedule(const Graph &graph, OptionsPtr options) const;
		};
	}
}
#ifndef COFFMANGRAHAM_SCHEDULERALGORITHM_H
#define COFFMANGRAHAM_SCHEDULERALGORITHM_H

#include "ListSchedulingAlgorithm.h"

namespace msonlab {
	namespace scheduling {

		using namespace msonlab;

		class CoffmanGrahamSchedulingAlgorithm : public ListSchedulingAlgorithm
		{
			virtual void determineCosts(const Graph& graph, vector<unsigned>& costs) const;
		public:
			virtual ptr build(Options::oPtr) const;
			virtual ~CoffmanGrahamSchedulingAlgorithm() = default;
		};
	}
}
#endif
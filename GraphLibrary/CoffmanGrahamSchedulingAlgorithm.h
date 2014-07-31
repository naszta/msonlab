#ifndef COFFMANGRAHAM_SCHEDULERALGORITHM_H
#define COFFMANGRAHAM_SCHEDULERALGORITHM_H

#include "ListSchedulingAlgorithm.h"

namespace msonlab {
	namespace scheduling {

		using namespace msonlab;

		class CoffmanGrahamSchedulingAlgorithm : public ListSchedulingAlgorithm
		{
		protected:
			virtual void determineCosts(const Graph::gPtr& graph, vector<unsigned>& costs) const;
		};
	}
}
#endif
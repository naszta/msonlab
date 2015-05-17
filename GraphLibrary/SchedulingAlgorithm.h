#ifndef GRAPHLIB_SCHEDULERALGORITHM_H
#define GRAPHLIB_SCHEDULERALGORITHM_H

#include <string>
#include <memory>
#include "SchedulingResult.h"
#include "Graph/Graph.h"

namespace msonlab {
	namespace scheduling {

		using msonlab::Graph;
		using std::unique_ptr;
		using std::vector;

		class SchedulingAlgorithm;
		typedef unique_ptr<SchedulingAlgorithm> SchedulingAlgorithmPtr;

		///
		/// Abstract class for schedule algorithms.
		///
		class SchedulingAlgorithm
		{
			friend class SchedulingAlgorithmBuilder;
			virtual unique_ptr<SchedulingAlgorithm> build(const Options&) const = 0;
		public:
			
			virtual SchedulingResultPtr<const NodePtr> schedule(const Graph &graph, const Options &options) const = 0;
		};
	}
}

#endif
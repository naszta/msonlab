#ifndef GRAPHLIB_SCHEDULERALGORITHM_H
#define GRAPHLIB_SCHEDULERALGORITHM_H

#include <memory>
#include "Chromosome.h"
#include "Graph.h"

namespace msonlab
{
	///
	/// Abstract class for schedule algorithms.
	///
	class SchedulingAlgorithm
	{
	public:
		typedef std::shared_ptr<SchedulingAlgorithm> algPtr;
		virtual Chromosome::cPtr schedule(Graph::gPtr& graph, Options::oPtr options) const = 0;
	};
}

#endif
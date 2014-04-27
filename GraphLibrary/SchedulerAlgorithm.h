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
	class ShedulerAlgorithm
	{
		virtual Chromosome::cPtr shedule(Graph::gPtr graph) const = 0;
	};
}

#endif
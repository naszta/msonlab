#pragma once

#include "SchedulingAlgorithm.h"
#include "SchedulingAlgorithmBuilder.h"

namespace msonlab {
	namespace scheduling {

		using msonlab::Graph;

		///
		/// This class implements a greedy scheduling of the graph.
		///
		/// Greedy scheduling means, it always schedules the next
		/// ready to process node to the next available PU.
		class GreedySchedulingAlgorithm : public SchedulingAlgorithm
		{
		public:
			//static GreedySchedulingAlgorithm example;
			GreedySchedulingAlgorithm() = default;
			GreedySchedulingAlgorithm(exemplar e) { SchedulingAlgorithmBuilder::add_scheduling_algorithm(this); }
			//SolutionPtr schedule(const Graph &graph, OptionsPtr options) const;
			SchedulingResultPtr<const NodePtr> schedule(const Graph &graph, const	Options &options) const;
			virtual SchedulingAlgorithmPtr build(OptionsPtr) const;
			virtual ~GreedySchedulingAlgorithm() = default;
		};
	}
}
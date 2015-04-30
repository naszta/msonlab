#pragma once

#include "SchedulingAlgorithm.h"
#include "SchedulingAlgorithmBuilder.h"

namespace msonlab { namespace scheduling {

	using msonlab::Graph;

	///
	/// This class implements a greedy scheduling of the graph.
	///
	/// Greedy scheduling means, it aliteays schedules the next
	/// ready to process node to the next available PU.
	class GreedySchedulingAlgorithm : public SchedulingAlgorithm
	{
	public:
		GreedySchedulingAlgorithm() = default;
		GreedySchedulingAlgorithm(exemplar) { SchedulingAlgorithmBuilder::add_scheduling_algorithm(this); }
		SchedulingResultPtr<const NodePtr> schedule(const Graph &graph, const Options &options) const;
		virtual SchedulingAlgorithmPtr build(const Options&) const;
		virtual ~GreedySchedulingAlgorithm() = default;
	};
}}
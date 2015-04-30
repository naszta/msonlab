#pragma once
#include "SchedulingAlgorithm.h"
#include "SchedulingAlgorithmBuilder.h"
#include "litegraph.h"

namespace msonlab { namespace scheduling {
	class ListSchedulingAlgorithm : public SchedulingAlgorithm {
	protected:
		virtual unsigned findNextToSchedule(const vector<int>& dependencies, const vector<unsigned>& costs) const;
		virtual void determineCosts(const lite::litegraph &graph, vector<unsigned>& costs) const;
	public:
		virtual unique_ptr<SchedulingAlgorithm> build(const Options&) const override;
		SchedulingResultPtr<const NodePtr> schedule(const Graph &graph, const Options &options) const override;
	};
}}
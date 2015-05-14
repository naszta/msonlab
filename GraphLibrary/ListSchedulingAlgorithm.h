#ifndef LIST_SEDULING_ALGORITHM_H
#define LIST_SEDULING_ALGORITHM_H

#include "SchedulingAlgorithm.h"
#include "SchedulingAlgorithmBuilder.h"
#include "litegraph.h"

namespace msonlab { namespace scheduling {
	class ListSchedulingAlgorithm : public SchedulingAlgorithm {
		friend class GeneticAlgorithm;
	protected:
		virtual unsigned findNextToSchedule(const vector<int>& dependencies, const vector<unsigned>& costs) const;
		virtual void determineCosts(const lite::litegraph &graph, vector<unsigned>& costs) const;
		SchedulingResultPtr<const lite::litenode* > scheduleLite(const lite::litegraph &graph, const Options &options) const;
	public:
		virtual unique_ptr<SchedulingAlgorithm> build(const Options&) const override;
		SchedulingResultPtr<const NodePtr> schedule(const Graph &graph, const Options &options) const override;
	};
}}
#endif
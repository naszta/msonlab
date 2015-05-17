#ifndef COFFMANGRAHAM_SCHEDULERALGORITHM_H
#define COFFMANGRAHAM_SCHEDULERALGORITHM_H

#include "ListSchedulingAlgorithm.h"

namespace msonlab { namespace scheduling {

	using namespace msonlab;

	class CoffmanGrahamSchedulingAlgorithm : public ListSchedulingAlgorithm
	{
	protected:
		virtual void determineCosts(const lite::litegraph& graph, vector<unsigned>& costs) const;
	public:
		CoffmanGrahamSchedulingAlgorithm() = default;
		CoffmanGrahamSchedulingAlgorithm(exemplar) { SchedulingAlgorithmBuilder::add_scheduling_algorithm(this); }
		virtual SchedulingAlgorithmPtr build(const Options&) const;
		virtual ~CoffmanGrahamSchedulingAlgorithm() = default;
	};
}}
#endif
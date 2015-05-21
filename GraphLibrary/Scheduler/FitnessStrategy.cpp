#include "FitnessStrategy.h"
#include "SchedulingUtils.h"
#include <algorithm>
#include <numeric>

namespace msonlab { namespace scheduling {

	using std::make_unique;

	// storing pointers to the example instances
	vector<FitnessStrategy*> FitnessStrategy::examplars;
		
	LengthFitnessStartegy LengthFitnessStartegy::example{ Examplar{} };
	RescheduleIdleTimeFitnessStartegy RescheduleIdleTimeFitnessStartegy::example{ Examplar{} };
	PUUsageFitnessStrategy PUUsageFitnessStrategy::example{ Examplar{} };
	LoadBalanceFitnessStrategy LoadBalanceFitnessStrategy::example{ Examplar{} };


	unsigned int LengthFitnessStartegy::fitness(SchedulingResult<const lite::litenode*> &solution, const Options& options) const {
		return computeLength<SchedulingResult<const lite::litenode*>>(solution, options);
	}

	FSPtr LengthFitnessStartegy::build(string name) const {
		if (name.compare("length") == 0) {
			DEBUGLN("LengthFitnessStrategy.");
			return make_unique<LengthFitnessStartegy>();
		}

		return nullptr;
	}

	unsigned int RescheduleIdleTimeFitnessStartegy::fitness(SchedulingResult<const lite::litenode*> &solution, const Options& options) const
	{
		auto rescheduled = msonlab::scheduling::computeLengthAndReuseIdleTime(solution, options);
		solution._mapping = std::move(rescheduled._mapping);
		solution._scheduling = std::move(rescheduled._scheduling);
		solution._fitness = rescheduled._fitness;
		solution._pus = rescheduled._pus;
		return solution.fitness();
	}

	FSPtr RescheduleIdleTimeFitnessStartegy::build(string name) const {
		if (name.compare("reschedule") == 0) {
			DEBUGLN("RescheduleIdleTimeFitnessStartegy.");
			return make_unique<RescheduleIdleTimeFitnessStartegy>();
		}

		return nullptr;
	}

	unsigned int PUUsageFitnessStrategy::fitness(SchedulingResult<const lite::litenode*> &solution, const Options& options) const {
		unsigned length = computeLength(solution, options);

		const auto &scheduling = solution.scheduling();
		unsigned sumUsedTime = length * options.numberOfPus();
		unsigned sumWorkTime = 0;
		for (auto node : scheduling) {
			sumWorkTime += node->cptime();
		}

		// returning the percentage of time spent idle
		return ((sumUsedTime - sumWorkTime) * 1000) / sumUsedTime;
	}

	FSPtr PUUsageFitnessStrategy::build(string name) const {
		if (name.compare("puusage") == 0) {
			DEBUGLN("PUUsageFitnessStrategy");
			return make_unique<PUUsageFitnessStrategy>(Examplar{});
		}

		return nullptr;
	}

	unsigned int LoadBalanceFitnessStrategy::fitness(SchedulingResult<const lite::litenode*> &solution, const Options& options) const
	{
		vector<unsigned> RT(options.numberOfPus());
		int length = computeLengthAndRT(solution, options, RT);
		double avg = std::accumulate(RT.begin(), RT.end(), 0) / options.numberOfPus();
		double load_balance = length / avg;
		return (unsigned)(load_balance * 1000);
	}

	FSPtr LoadBalanceFitnessStrategy::build(string name) const {
		if (name.compare("loadbalance") == 0) {
			DEBUGLN("LoadBalanceFitnessStrategy");
			return make_unique<LoadBalanceFitnessStrategy>(Examplar());
		}

		return nullptr;
	}
}}
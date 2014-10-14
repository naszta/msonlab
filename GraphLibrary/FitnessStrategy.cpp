#include "FitnessStrategy.h"
#include "SchedulingHelper.h"
#include <algorithm>
#include <numeric>

namespace msonlab {
	namespace scheduling {

		using std::make_unique;

		// storing pointers to the example instances
		vector<FitnessStrategy*> FitnessStrategy::examplars;
		LengthFitnessStartegy LengthFitnessStartegy::example{ Examplar() };
		RescheduleIdleTimeFitnessStartegy RescheduleIdleTimeFitnessStartegy::example{ Examplar() };
		PUUsageFitnessStrategy PUUsageFitnessStrategy::example { Examplar() };
		LoadBalanceFitnessStrategy LoadBalanceFitnessStrategy::example { Examplar() };


		unsigned int LengthFitnessStartegy::fitness(const Solution &solution, const OptionsPtr options) const
		{
			return computeLength(solution, options);
		}

		FSPtr LengthFitnessStartegy::build(string name) const
		{
			if (name.compare("length") == 0) {
				return make_unique<LengthFitnessStartegy>(Examplar());
			}

			return nullptr;
		}

		unsigned int RescheduleIdleTimeFitnessStartegy::fitness(const Solution &solution, const OptionsPtr options) const
		{
			Solution &s = const_cast<Solution &>(solution);
			return computeLengthAndReuseIdleTime(s, *options);
		}

		FSPtr RescheduleIdleTimeFitnessStartegy::build(string name) const
		{
			if (name.compare("reschedule") == 0) {
				return make_unique<RescheduleIdleTimeFitnessStartegy>(Examplar());
			}

			return nullptr;
		}

		unsigned int PUUsageFitnessStrategy::fitness(const Solution &solution, const OptionsPtr options) const
		{
			unsigned length = computeLength(solution, options);

			lw::LWNodeVect scheduling = solution.scheduling();
			unsigned sumUsedTime = length * options->getNumberOfPus();
			unsigned sumWorkTime = 0;
			for (auto node : scheduling) {
				sumWorkTime += node->cptime();
			}


			// returning the percentage of time spent idle
			return ((sumUsedTime - sumWorkTime) * 1000) / sumUsedTime;
		}

		FSPtr PUUsageFitnessStrategy::build(string name) const
		{
			if (name.compare("puusage") == 0) {
				return make_unique<PUUsageFitnessStrategy>(Examplar());
			}

			return nullptr;
		}

		unsigned int LoadBalanceFitnessStrategy::fitness(const Solution &solution, const OptionsPtr options) const
		{
			vector<unsigned> RT(options->getNumberOfPus());
			int length = computeLengthAndRT(solution, options, RT);
			double avg = std::accumulate(RT.begin(), RT.end(), 0) / options->getNumberOfPus();
			double load_balance = length / avg;
			return (unsigned)(load_balance * 1000);
		}

		FSPtr LoadBalanceFitnessStrategy::build(string name) const
		{
			if (name.compare("loadbalance") == 0) {
				return make_unique<LoadBalanceFitnessStrategy>(Examplar());
			}

			return nullptr;
		}

		unsigned int OpenEdgesFitnessStrategy::fitness(const Solution &solution, const OptionsPtr options) const
		{
			return 0;
		}
	}
}
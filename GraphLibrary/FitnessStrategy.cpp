#include "FitnessStrategy.h"
#include "SchedulingHelper.h"
#include <algorithm>
#include <numeric>

namespace msonlab {
	namespace scheduling {

		using std::shared_ptr;
		using std::make_unique;

		vector<FitnessStrategy*> FitnessStrategy::examplars;
		LengthFitnessStartegy LengthFitnessStartegy::example{ Examplar() };
		RescheduleIdleTimeFitnessStartegy RescheduleIdleTimeFitnessStartegy::example{ Examplar() };
		PUUsageFitnessStrategy PUUsageFitnessStrategy::example { Examplar() };
		LoadBalanceFitnessStrategy LoadBalanceFitnessStrategy::example { Examplar() };


		unsigned int LengthFitnessStartegy::fitness(Solution::sPtr solution, const Options::oPtr options) const
		{
			return SchedulingHelper::computeLength(solution, options);
		}

		FitnessStrategy::fsPtr LengthFitnessStartegy::build(string name) const
		{
			if (name.compare("length") == 0) {
				return make_unique<LengthFitnessStartegy>(Examplar());
			}

			return nullptr;
		}

		unsigned int RescheduleIdleTimeFitnessStartegy::fitness(Solution::sPtr solution, const Options::oPtr options) const
		{
			return SchedulingHelper::computeLengthAndReuseIdleTime(solution, options);
		}

		FitnessStrategy::fsPtr RescheduleIdleTimeFitnessStartegy::build(string name) const
		{
			if (name.compare("reschedule") == 0) {
				return make_unique<RescheduleIdleTimeFitnessStartegy>(Examplar());
			}

			return nullptr;
		}

		unsigned int PUUsageFitnessStrategy::fitness(Solution::sPtr solution, const Options::oPtr options) const
		{
			unsigned length = SchedulingHelper::computeLength(solution, options);

			unsigned sumUsedTime = length * options->getNumberOfPus();
			unsigned sumWorkTime = 0;

			auto scheduling = solution->getScheduling();

			for (auto it = scheduling.begin(); it != scheduling.end(); ++it)
			{
				sumWorkTime += (*it)->getComputationTime();
			}

			// returning the percentage of time spent idle
			return ((sumUsedTime - sumWorkTime) * 1000) / sumUsedTime;
		}

		FitnessStrategy::fsPtr PUUsageFitnessStrategy::build(string name) const
		{
			if (name.compare("puusage") == 0) {
				return make_unique<PUUsageFitnessStrategy>(Examplar());
			}

			return nullptr;
		}

		unsigned int LoadBalanceFitnessStrategy::fitness(Solution::sPtr solution, const Options::oPtr options) const
		{
			vector<unsigned> RT(options->getNumberOfPus());
			int length = SchedulingHelper::computeLengthAndRT(solution, options, RT);
			double avg = std::accumulate(RT.begin(), RT.end(), 0) / options->getNumberOfPus();
			double load_balance = length / avg;
			return (unsigned)(load_balance * 1000);
		}

		FitnessStrategy::fsPtr LoadBalanceFitnessStrategy::build(string name) const
		{
			if (name.compare("loadbalance") == 0) {
				return make_unique<LoadBalanceFitnessStrategy>(Examplar());
			}

			return nullptr;
		}

		unsigned int OpenEdgesFitnessStrategy::fitness(Solution::sPtr solution, const Options::oPtr options) const
		{
			return 0;
		}
	}
}
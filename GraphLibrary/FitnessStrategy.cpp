#include "FitnessStrategy.h"
#include "GraphAlgorithms.h"
#include <algorithm>
#include <numeric>

namespace msonlab {
	namespace scheduling {

		using msonlab::GraphAlgorithms;

		unsigned int LengthFitnessStartegy::fitness(Solution::sPtr solution, const Options::oPtr options)
		{
			return GraphAlgorithms::computeLength(solution, options);
		}

		unsigned int RescheduleIdleTimeFitnessStartegy::fitness(Solution::sPtr solution, const Options::oPtr options)
		{
			return GraphAlgorithms::computeLengthAndReuseIdleTime(solution, options);
		}

		unsigned int PUUsageFitnessStrategy::fitness(Solution::sPtr solution, const Options::oPtr options)
		{
			unsigned length = GraphAlgorithms::computeLength(solution, options);

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

		unsigned int LoadBalanceFitnessStrategy::fitness(Solution::sPtr solution, const Options::oPtr options)
		{
			vector<unsigned> RT(options->getNumberOfPus());
			int length = GraphAlgorithms::computeLengthAndRT(solution, options, RT);
			double avg = std::accumulate(RT.begin(), RT.end(), 0) / options->getNumberOfPus();
			double load_balance = length / avg;
			return (unsigned)(load_balance * 1000);
		}

		unsigned int OpenEdgesFitnessStrategy::fitness(Solution::sPtr solution, const Options::oPtr options)
		{
			return 0;
		}
	}
}
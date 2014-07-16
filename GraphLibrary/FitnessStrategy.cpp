#include "FitnessStrategy.h"
#include "GraphAlgorithms.h"
#include <algorithm>
#include <numeric>

namespace msonlab
{
	unsigned int LengthFitnessStartegy::fitness(Chromosome::cPtr chromosome, const Options::oPtr options)
	{
		return GraphAlgorithms::computeLength(chromosome, options);
	}

	unsigned int RescheduleIdleTimeFitnessStartegy::fitness(Chromosome::cPtr chromosome, const Options::oPtr options)
	{
		return GraphAlgorithms::computeLengthAndReuseIdleTime(chromosome, options);
	}

	unsigned int PUUsageFitnessStrategy::fitness(Chromosome::cPtr chromosome, const Options::oPtr options)
	{
		unsigned length = GraphAlgorithms::computeLength(chromosome, options);
		
		unsigned sumUsedTime = length * options->getNumberOfPus();
		unsigned sumWorkTime = 0;

		auto scheduling = chromosome->getScheduling();

		for (auto it = scheduling.begin(); it != scheduling.end(); ++it)
		{
			sumWorkTime += (*it)->getComputationTime();
		}

		// returning the percentage of time spent idle
		return ((sumUsedTime - sumWorkTime)*1000)/sumUsedTime;
	}

	unsigned int LoadBalanceFitnessStrategy::fitness(Chromosome::cPtr chromosome, const Options::oPtr options)
	{
		vector<unsigned> RT(options->getNumberOfPus());
		int length = GraphAlgorithms::computeLengthAndRT(chromosome, options, RT);
		double avg = std::accumulate(RT.begin(), RT.end(), 0) / options->getNumberOfPus();
		double load_balance = length / avg;
		return (unsigned)(load_balance * 1000);
	}

	unsigned int OpenEdgesFitnessStrategy::fitness(Chromosome::cPtr chromosome, const Options::oPtr options)
	{
		return 0;
	}

}
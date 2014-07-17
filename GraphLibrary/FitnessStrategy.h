#ifndef GRAPHLIB_FITNESSSTRATEGY_H
#define GRAPHLIB_FITNESSSTRATEGY_H

#include "Solution.h"
#include "Options.h"

namespace msonlab {
	namespace scheduling {
		///
		/// This class is a base for the Strategy pattern
		/// to calculate the fitness of a chromsome.
		class FitnessStrategy
		{
		public:
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options) = 0;

			typedef std::shared_ptr<FitnessStrategy> fsPtr;
		};

		class LengthFitnessStartegy : public FitnessStrategy
		{
		public:
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options);
		};

		class RescheduleIdleTimeFitnessStartegy : public FitnessStrategy
		{
		public:
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options);
		};

		class PUUsageFitnessStrategy : public FitnessStrategy
		{
		public:
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options);
		};

		class LoadBalanceFitnessStrategy : public FitnessStrategy
		{
		public:
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options);
		};

		class OpenEdgesFitnessStrategy : public FitnessStrategy
		{
		public:
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options);
		};
	}
}

#endif
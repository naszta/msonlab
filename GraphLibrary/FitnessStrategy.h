#ifndef GRAPHLIB_FITNESSSTRATEGY_H
#define GRAPHLIB_FITNESSSTRATEGY_H

#include "Chromosome.h"
#include "Options.h"

namespace msonlab
{
	///
	/// This class is a base for the Strategy pattern
	/// to calculate the fitness of a chromsome.
	class FitnessStrategy
	{
	protected:
		unsigned int max(unsigned int a, unsigned int b) {
			return a > b ? a : b;
		}
	public:
		virtual unsigned int fitness(Chromosome::cPtr chromosome, Options::oPtr options) = 0;

		typedef std::shared_ptr<FitnessStrategy> fsPtr;
	};

	class LengthFitnessStartegy : public FitnessStrategy
	{
		bool punishCommunication;
	public:
		LengthFitnessStartegy();
		LengthFitnessStartegy(bool punishCommunication);
		virtual unsigned int fitness(Chromosome::cPtr chromosome, Options::oPtr options);
	};

	class PUUsageFitnessStrategy : public LengthFitnessStartegy
	{
	public:
		virtual unsigned int fitness(Chromosome::cPtr chromosome, Options::oPtr options);
	};

	class OpenEdgesFitnessStrategy : public FitnessStrategy
	{
	public:
		virtual unsigned int fitness(Chromosome::cPtr chromosome, Options::oPtr options);
	};
}

#endif
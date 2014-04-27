#ifndef GRAPHLIB_FITNESSSTRATEGY_H
#define GRAPHLIB_FITNESSSTRATEGY_H

#include "Chromosome.h"
#include "GAOptions.h"

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
		virtual unsigned int fitness(Chromosome::cPtr chromosome, GAOptions::gaPtr options) = 0;
		virtual void communication(unsigned nodeFromId, unsigned nodeToId) = 0;
		virtual void startTimeSet(unsigned nodeId, unsigned time) = 0;

		typedef std::shared_ptr<FitnessStrategy> fsPtr;
	};

	class LengthFitnessStartegy : public FitnessStrategy
	{
		std::shared_ptr<FitnessStrategy> handler;
	public:
		virtual unsigned int fitness(Chromosome::cPtr chromosome, GAOptions::gaPtr options);
		virtual void communication(unsigned nodeFromId, unsigned nodeToId);
		virtual void startTimeSet(unsigned nodeId, unsigned time);
		void setHandler(std::shared_ptr<FitnessStrategy> strategy) {
			this->handler = strategy;
		}
	};

	class LeastCutFitnessStrategy : public LengthFitnessStartegy
	{
	public:
		virtual unsigned int fitness(Chromosome::cPtr chromosome, GAOptions::gaPtr options);
		virtual void communication(unsigned nodeFromId, unsigned nodeToId);
		virtual void startTimeSet(unsigned nodeId, unsigned time);
	};

	class OpenEdgesFitnessStrategy : public FitnessStrategy
	{
	public:
		virtual unsigned int fitness(Chromosome::cPtr chromosome, GAOptions::gaPtr options);
		virtual void communication(unsigned nodeFromId, unsigned nodeToId);
		virtual void startTimeSet(unsigned nodeId, unsigned time);
	};
}

#endif
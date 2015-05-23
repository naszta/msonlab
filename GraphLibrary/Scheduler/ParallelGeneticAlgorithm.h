#ifndef GRAPHLIB_SCHEDULER_PARALLELGENETICALGORITHM_H
#define GRAPHLIB_SCHEDULER_PARALLELGENETICALGORITHM_H

#include "../Graph/Node.h"
#include "GeneticAlgorithm.h"
#include "SchedulingResult.h"

namespace msonlab { namespace scheduling {

	class ParallelGeneticAlgorithm : public GeneticAlgorithm {
	public:
		// build function for creating instance
		virtual SchedulingAlgorithmPtr build(const Options&) const override;
		// constructor for the example instance
		ParallelGeneticAlgorithm(exemplar e) : GeneticAlgorithm(e) { }
		// contructor for normal use
		ParallelGeneticAlgorithm(FSPtr);
		// descturctor
		virtual ~ParallelGeneticAlgorithm() {}
	private:
		friend class round_simulator;

		virtual void simulateMating(SolutionSet& set, int offsprings, bool doOrderCrossover, const Options& options) const override;
	};
}}

#endif
#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <memory>
#include <vector>
#include "../litegraph.h"
#include "SolutionSet.h"
#include "SchedulingAlgorithm.h"
#include "SchedulingAlgorithmBuilder.h"
#include "FitnessStrategy.h"
#include "SchedulingResult.h"

// GeneticAlgoritm implementation for project laboratory
// created by Zoltan Szekeres

namespace msonlab { namespace scheduling {

	using msonlab::Graph;

	using std::vector;
	using std::shared_ptr;
	using std::unique_ptr;

	class GeneticAlgorithm : public SchedulingAlgorithm
	{
		friend class ParallelGeneticAlgorithm;
		friend class round_simulator;
	public:
		// build function for creating instance
		virtual SchedulingAlgorithmPtr build(const Options&) const override;
		// constructor for the example instance
		GeneticAlgorithm(exemplar) { SchedulingAlgorithmBuilder::add_scheduling_algorithm(this); }
		// contructor for normal use
		GeneticAlgorithm(FSPtr);
		// schedule the given graph with the given options
		virtual SchedulingResultPtr<const NodePtr> schedule(const Graph& graph, const Options &options) const override;
		// descturctor
		virtual ~GeneticAlgorithm() {}
	private:
		typedef unsigned int uint;

		// algorithm parameters
		FSPtr fsstrategy;

		// entry method for generating initial solution
		SolutionSet generateInitialSolution(const lite::litegraph &graph, const Options &options) const;
		// generate a soulution set based on CP methods
		SolutionSet generateCPSolution(const lite::litegraph &graph, const Options &options) const;
		// generate a solution set randomly
		SolutionSet generateRndSolution(const lite::litegraph &graph, const Options &options) const;

		// creates one random solution
		SchedulingResultPtr<const lite::litenode*> createRandomSolution(const lite::litegraph &graph, const Options &options, vector<vector<const lite::litenode*>>& layers) const;

		// crossover operations
		SchedulingResultPtr<const lite::litenode*> crossoverMap(SchedulingResultPtr<const lite::litenode*> father, SchedulingResultPtr<const lite::litenode*> mother) const;// TODO
		SchedulingResultPtr<const lite::litenode*> crossoverOrder(SchedulingResultPtr<const lite::litenode*> father, SchedulingResultPtr<const lite::litenode*> mother, const vector<unsigned>& levelingLimits) const;// TODO
			
		// mutation operations
		void mutateMapping(SchedulingResultPtr<const lite::litenode*> offspring) const;// TODO
		void mutateSheduling(SchedulingResultPtr<const lite::litenode*> offspring, const vector<unsigned>& levelingLimits) const;// TODO

		virtual void simulateMating(SolutionSet& set, int offsprings, bool doOrderCrossover, const Options& options) const;
		unsigned int fitness(SchedulingResultPtr<const lite::litenode*> solution, const Options& options) const; // TODO

		//void parallelSimulateMating(SolutionSetPtr& set, int offsprings, bool doOrderCrossover, const Options& options) const;// TODO

		// Gets a greedy solution for the given graph
		SchedulingResultPtr<const lite::litenode*> greedySolution(const lite::litegraph &graph, const Options& options) const;
		// experimental
		void swapMutateScheduling(SchedulingResultPtr<const lite::litenode*> offspring, const Options& options) const;
	};
}}
#endif
#pragma once

#include <memory>
#include <vector>
#include "SolutionSet.h"
#include "SchedulingAlgorithm.h"
#include "FitnessStrategy.h"
#include "lwgraph.h"
#include "SchedulingResult.h"

// GeneticAlgoritm implementation for project laboratory
// created by Zoltan Szekeres

namespace msonlab {
	namespace scheduling {

		using msonlab::Graph;

		using std::vector;
		using std::shared_ptr;
		using std::unique_ptr;

		class GeneticAlgorithm : public SchedulingAlgorithm
		{
		public:
			//typedef vector< SolutionPtr > cVect;

			// example instance for construction purposes
			static GeneticAlgorithm example;
			// build function for creating instance
			virtual SchedulingAlgorithmPtr build(OptionsPtr) const override;
			// constructor for the example instance
			GeneticAlgorithm(examplar e) { SchedulingAlgorithm::add_scheduling_algorithm(this); }
			// contructor for normal use
			GeneticAlgorithm(OptionsPtr, FSPtr);
			// schedule the given graph with the given options
			virtual SchedulingResultPtr<const NodePtr> schedule(const Graph& graph, const Options &options) const override;
		private:
			typedef unsigned int uint;

			// algorithm parameters
			OptionsPtr options;
			FSPtr fsstrategy;

			// entry method for generating initial solution
			SolutionSetPtr generateInitialSolution(const lw::lwgraph &graph, const Options &options) const;
			// generate a soulution set based on CP methods
			SolutionSetPtr generateCPSolution(const lw::lwgraph &graph, const Options &options) const;
			// generate a solution set randomly
			SolutionSetPtr generateRndSolution(const lw::lwgraph &graph, const Options &options) const;

			// crossover operations
			SchedulingResultPtr<const lw::lwnode*> crossoverMap(SchedulingResultPtr<const lw::lwnode*> father, SchedulingResultPtr<const lw::lwnode*> mother) const;// TODO
			SchedulingResultPtr<const lw::lwnode*> crossoverOrder(SchedulingResultPtr<const lw::lwnode*> father, SchedulingResultPtr<const lw::lwnode*> mother, const vector<unsigned>& levelingLimits) const;// TODO
			
			// mutation operations
			void mutateMapping(SchedulingResultPtr<const lw::lwnode*> offspring) const;// TODO
			void mutateSheduling(SchedulingResultPtr<const lw::lwnode*> offspring, const vector<unsigned>& levelingLimits) const;// TODO

			void simulateMating(const SolutionSetPtr& set, int offsprings, bool doOrderCrossover) const;// TODO
			unsigned int fitness(SchedulingResultPtr<const lw::lwnode*> solution) const; // TODO

			void parallelSimulateMating(SolutionSetPtr& set, int offsprings, bool doOrderCrossover) const;// TODO
			friend class round_simulator;

			// Gets a greedy solution for the given graph
			SchedulingResultPtr<const lw::lwnode*> greedySolution(const lw::lwgraph &graph) const;
			// experimental
			void swapMutateScheduling(SchedulingResultPtr<const lw::lwnode*> offspring) const;
		};
	}
}
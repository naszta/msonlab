#pragma once

#include <memory>
#include <vector>
#include "SolutionSet.h"
#include "SchedulingAlgorithm.h"
#include "FitnessStrategy.h"
#include "lwgraph.h"

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
			typedef unsigned int uint;
			// algorithm parameters
			OptionsPtr options;
			FSPtr fsstrategy;

			SolutionSet::setPtr generateInitialSolution(const lw::lwgraph &graph, OptionsPtr options) const;
			SolutionSet::setPtr generateCPSolution(const lw::lwgraph &graph, OptionsPtr options) const;
			SolutionSet::setPtr generateRndSolution(const lw::lwgraph &graph, OptionsPtr options) const;

			// crossover operations
			SolutionPtr crossoverMap(SolutionPtr father, SolutionPtr mother) const;
			SolutionPtr crossoverOrder(SolutionPtr father, SolutionPtr mother, const vector<unsigned>& levelingLimits) const;
			
			// mutation operations
			void mutateMapping(SolutionPtr offspring) const;
			void mutateSheduling(SolutionPtr offspring, const vector<unsigned>& levelingLimits) const;

			void simulateMating(SolutionSet::setPtr& set, int offsprings, bool doOrderCrossover) const;
			unsigned int fitness(SolutionPtr solution) const;

			void parallelSimulateMating(SolutionSet::setPtr& set, int offsprings, bool doOrderCrossover) const;
			friend class round_simulator;

			// building instance for virtual constructor
			SolutionPtr greedySolution(const lw::lwgraph &graph) const;

		public:
			typedef vector< SolutionPtr > cVect;

			static GeneticAlgorithm example;
			GeneticAlgorithm(examplar e) { SchedulingAlgorithm::add_scheduling_algorithm(this); }
			GeneticAlgorithm(OptionsPtr, FSPtr);
			virtual SchedulingAlgorithmPtr build(OptionsPtr) const;

			virtual SolutionPtr schedule(const Graph& graph, OptionsPtr options) const;

			static void transfromResult(SolutionPtr c, vector<unsigned>& result);

			// experimental
			void swapMutateScheduling(SolutionPtr offspring) const;
		};
	}
}
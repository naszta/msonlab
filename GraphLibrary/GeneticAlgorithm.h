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
		private:
			typedef unsigned int uint;
			typedef SchedulingResult<const lwnnode*> SchedulingResultLW;
			typedef shared_ptr<SchedulingResultLW> SchedulingResultLWPtr;
			// algorithm parameters
			OptionsPtr options;
			FSPtr fsstrategy;

			SolutionSet::setPtr generateInitialSolution(const lw::lwgraph &graph, OptionsPtr options) const;// TODO
			SolutionSet::setPtr generateCPSolution(const lw::lwgraph &graph, OptionsPtr options) const;// TODO
			SolutionSet::setPtr generateRndSolution(const lw::lwgraph &graph, OptionsPtr options) const;// TODO

			// crossover operations
			SolutionPtr crossoverMap(SolutionPtr father, SolutionPtr mother) const;// TODO
			SolutionPtr crossoverOrder(SolutionPtr father, SolutionPtr mother, const vector<unsigned>& levelingLimits) const;// TODO
			
			// mutation operations
			void mutateMapping(SolutionPtr offspring) const;// TODO
			void mutateSheduling(SolutionPtr offspring, const vector<unsigned>& levelingLimits) const;// TODO

			void simulateMating(SolutionSet::setPtr& set, int offsprings, bool doOrderCrossover) const;// TODO
			unsigned int fitness(SchedulingResultLWPtr solution) const; // TODO

			void parallelSimulateMating(SolutionSet::setPtr& set, int offsprings, bool doOrderCrossover) const;// TODO
			friend class round_simulator;

			// building instance for virtual constructor
			SchedulingResultLWPtr greedySolution(const lw::lwgraph &graph) const;
		};
	}
}
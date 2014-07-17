#pragma once

#include <memory>
#include <vector>
#include "SolutionSet.h"
#include "SchedulingAlgorithm.h"
#include "FitnessStrategy.h"
#include "GraphAlgorithms.h"


// GeneticAlgoritm implementation for project laboratory
// created by Zoltan Szekeres

namespace msonlab {
	namespace scheduling {

		using msonlab::Graph;
		using msonlab::GraphAlgorithms;

		using std::vector;
		using std::shared_ptr;
		using std::unique_ptr;

		class GeneticAlgorithm : public SchedulingAlgorithm
		{
			typedef unsigned int uint;
			// num of PUs
			GraphAlgorithms algorithms;

			// algorithm parameters
			Options::oPtr options;
			FitnessStrategy::fsPtr fsstrategy;
			// remove from here
			//vector<unsigned> levelingLimits;
		public:
			//typedef shared_ptr<Solution> cPtr;
			typedef vector< Solution::sPtr > cVect;
			//typedef unique_ptr<Population> pPtr;

			GeneticAlgorithm(Options::oPtr options, FitnessStrategy::fsPtr strategy);

			virtual Solution::sPtr schedule(Graph::gPtr& graph, Options::oPtr options) const;

			Solution::sPtr greedySolution(Graph::gPtr& graph) const;
			SolutionSet::setPtr generateInitialSolution(Graph::gPtr& graph, Options::oPtr options) const;
			SolutionSet::setPtr generateCPSolution(Graph::gPtr& graph, Options::oPtr options) const;
			SolutionSet::setPtr generateRndSolution(Graph::gPtr& graph, Options::oPtr options) const;
			unsigned int fitness(Solution::sPtr solution) const;

			Solution::sPtr crossoverMap(Solution::sPtr father, Solution::sPtr mother) const;
			Solution::sPtr crossoverOrder(Solution::sPtr father, Solution::sPtr mother, const vector<unsigned>& levelingLimits) const;
			void mutateMapping(Solution::sPtr& offspring) const;
			void mutateSheduling(Solution::sPtr offspring, const vector<unsigned>& levelingLimits) const;

			void simulateMating(SolutionSet::setPtr& set, int offsprings, bool doOrderCrossover) const;

			static void transfromResult(Solution::sPtr c, vector<unsigned>& result);

			// experimental
			void swapMutateScheduling(Solution::sPtr offspring) const;
		};
	}
}
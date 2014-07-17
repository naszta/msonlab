#pragma once

#include <memory>
#include <vector>
#include "Population.h"
#include "SchedulingAlgorithm.h"
#include "FitnessStrategy.h"
#include "GraphAlgorithms.h"


// GeneticAlgoritm implementation for project laboratory
// created by Zoltan Szekeres

namespace msonlab
{
	using std::vector;
	using std::shared_ptr;
	using std::unique_ptr;
	using msonlab::Population;

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
		Population::pPtr generateInitialSolution(Graph::gPtr& graph, Options::oPtr options) const;
		Population::pPtr generateCPSolution(Graph::gPtr& graph, Options::oPtr options) const;
		Population::pPtr generateRndSolution(Graph::gPtr& graph, Options::oPtr options) const;
		unsigned int fitness(Solution::sPtr solution) const;

		Solution::sPtr crossoverMap(Solution::sPtr father, Solution::sPtr mother) const;
		Solution::sPtr crossoverOrder(Solution::sPtr father, Solution::sPtr mother, const vector<unsigned>& levelingLimits) const;
		void mutateMapping(Solution::sPtr& offspring) const;
		void mutateSheduling(Solution::sPtr offspring, const vector<unsigned>& levelingLimits) const;

		void simulateMating(Population::pPtr& population, int offsprings, bool doOrderCrossover) const;

		static void transfromResult(Solution::sPtr c, vector<unsigned>& result);

		// experimental
		void swapMutateScheduling(Solution::sPtr offspring) const;
	};
}
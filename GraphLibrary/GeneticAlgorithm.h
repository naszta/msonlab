#pragma once

#include <memory>
#include <vector>
#include "Population.h"
#include "SchedulingAlgorithm.h"
#include "FitnessStrategy.h"
#include "GraphAlgorithms.h"

using std::vector;
using std::shared_ptr;

// GeneticAlgoritm implementation for project laboratory
// created by Zoltan Szekeres

namespace msonlab
{
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
		//typedef shared_ptr<Chromosome> cPtr;
		typedef vector< Chromosome::cPtr > cVect;
		typedef shared_ptr<Population> pPtr;

		GeneticAlgorithm(Options::oPtr options, FitnessStrategy::fsPtr strategy);

		virtual Chromosome::cPtr schedule(Graph::gPtr graph, Options::oPtr options) const;

		shared_ptr<Chromosome> greedyChromosome(Graph::gPtr graph) const;
		shared_ptr<Population> generateInitialSolution(Graph::gPtr graph) const;
		unsigned int fitness(cPtr chromosome) const;

		cPtr crossoverMap(cPtr father, cPtr mother) const;
		cPtr crossoverOrder(cPtr father, cPtr mother, const vector<unsigned>& levelingLimits) const;
		void mutateMapping(cPtr offspring) const;
		void mutateSheduling(cPtr offspring, const vector<unsigned>& levelingLimits) const;

		void simulateMating(pPtr population, int offsprings) const;

		void transfromResult(Chromosome::cPtr c, vector<unsigned>& result) const;
	};
}
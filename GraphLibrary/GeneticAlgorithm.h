#pragma once

#include <memory>
#include <vector>
#include "GraphAlgorithms.h"
#include "Population.h"
#include "GAOptions.h"
#include "SchedulerAlgorithm.h"
#include "FitnessStrategy.h"

using std::vector;
using std::shared_ptr;

// GeneticAlgoritm implementation for project laboratory
// created by Zoltan Szekeres

namespace msonlab
{
	class GeneticAlgorithm : ShedulerAlgorithm
	{
		typedef unsigned int uint;
		// num of PUs
		GraphAlgorithms algorithms;

		// algorithm parameters
		GAOptions::gaPtr gaoptions;
		FitnessStrategy::fsPtr fsstrategy;
		vector<unsigned> levelingLimits;
	public:
		typedef shared_ptr<Chromosome> cPtr;
		typedef vector< cPtr > cVect;
		typedef shared_ptr<Population> pPtr;

		GeneticAlgorithm(GAOptions::gaPtr options, FitnessStrategy::fsPtr strategy);

		virtual Chromosome::cPtr shedule(Graph::gPtr graph);

		shared_ptr<Chromosome> greedyChromosome(Graph::gPtr graph);
		shared_ptr<Population> generateInitialSolution(Graph::gPtr graph);
		unsigned int fitness(cPtr chromosome);

		cPtr crossoverMap(cPtr father, cPtr mother);
		cPtr crossoverOrder(cPtr father, cPtr mother);
		void mutateMapping(cPtr offspring);
		void mutateSheduling(cPtr offspring);

		void simulateMating(pPtr population, int offsprings);
	};
}
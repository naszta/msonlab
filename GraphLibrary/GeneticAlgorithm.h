#pragma once

#include <memory>
#include <vector>
#include "GraphAlgorithms.h"
#include "IProcessable.h"
#include "Graph.h"
#include "Chromosome.h"
#include "Population.h"
#include "GAOptions.h"

using std::vector;
using std::shared_ptr;

// GeneticAlgoritm implementation for project laboratory
// created by Zoltan Szekeres

namespace msonlab
{
	class GeneticAlgorithm 
	{
		typedef unsigned int uint;
		// num of PUs
		GraphAlgorithms algorithms;

		// algorithm parameters
		shared_ptr<GAOptions> gaoptions;
		vector<unsigned> levlingLimits;
	public:
		typedef shared_ptr<Chromosome> cPtr;
		typedef vector< cPtr > cVect;
		typedef shared_ptr<Population> pPtr;

		GeneticAlgorithm();
		GeneticAlgorithm(shared_ptr<GAOptions> options);

		shared_ptr<Chromosome> greedyChromosome(Graph::gPtr graph);
		shared_ptr<Population> generateInitialSolution(Graph::gPtr graph);
		unsigned int fitness(cPtr chromosome);

		cPtr crossover_map(cPtr father, cPtr mother);
		cPtr crossover_order(cPtr father, cPtr mother);
		void mutate(cPtr offspring);
		void mutateSheduling(cPtr offspring);

		void simulateMating(pPtr population, int offsprings);
	};
}
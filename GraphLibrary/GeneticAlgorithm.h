#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include "GraphAlgorithms.h"
#include "IProcessable.h"
#include "Graph.h"
#include "Chromosome.h"
#include "Population.h"
#include "GAOptions.h"

using std::vector;
using boost::shared_ptr;

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

		GeneticAlgorithm();
		GeneticAlgorithm(shared_ptr<GAOptions> options);

		shared_ptr<Chromosome> greedyChromosome(boost::shared_ptr<Graph> graph);
		shared_ptr<Population> generateInitialSolution(Graph::gPtr);
		unsigned int fitness(cPtr chromosome);

		cPtr crossover_map(cPtr father, cPtr mother);
		cPtr crossover_order(cPtr father, cPtr mother);
		void mutate(cPtr offspring);
		void mutateSheduling(cPtr offspring);

		void simulateMating(shared_ptr<Population> population, int offsprings);
	};
}
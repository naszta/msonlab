#pragma once

//#include <boost/enable_shared_from_this.hpp>
#include <memory>
#include <vector>
#include <queue>
#include "Chromosome.h"

using std::vector;
using std::shared_ptr;

/*********************************
* This class represents a population
* of chromosomes.
**********************************/

namespace msonlab
{
	class chrComparator
	{
	public:
		bool operator() (const shared_ptr<Chromosome>& a, const shared_ptr<Chromosome>& b) const
		{
			return a->getFitness() > b->getFitness();
		}
	};

	class Population
	{
		vector<shared_ptr<Chromosome>> solution;
		typedef shared_ptr<Chromosome> cPtr;
		std::priority_queue<cPtr, vector<cPtr>, chrComparator> newGeneration;
		const size_t POPMAXSIZE;
		const size_t KEEP;
		const size_t KEEPBEST;
		
	public:
		typedef shared_ptr<Chromosome> cPtr;
		Population(vector<cPtr> sol, size_t keepSize, size_t popMaxSize, size_t keepBest);

		vector<cPtr> getPopulation();

		// returns a chromosome from the solution
		cPtr getParent();
		// adding a new offspring to the population
		void addOffspring(cPtr offspring);
		// adds one year to the age of all the chromosome
		void ageChromosomes();
		// limits the number of chromosomes to the initial
		void limit();
		// gets the best solution in the population
		cPtr best();

		unsigned avarageFittness();
	};
}
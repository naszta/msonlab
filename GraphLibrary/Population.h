#pragma once

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
		/// this contains the size of the graph's levels
		vector<size_t> levelSize;
		vector<shared_ptr<Chromosome>> solution;
		typedef shared_ptr<Chromosome> cPtr;
		std::priority_queue<cPtr, vector<cPtr>, chrComparator> newGeneration;
		const size_t POPMAXSIZE;
		const size_t KEEP;
		const size_t KEEPBEST;
		
	public:
		typedef shared_ptr<Population> pPtr;
		Population(const vector<cPtr>& sol, size_t keepSize, size_t popMaxSize, size_t keepBest);

		const vector<cPtr>& getPopulation() const;

		/// Gets a random chromosome from the solution.
		cPtr getParent() const;
		/// Adds a new offspring to the population.
		void addOffspring(cPtr offspring);
		/// Adds one year to the age of all the chromosome
		void ageChromosomes();
		/// Limits the number of chromosomes to the initial.
		void limit();
		/// Gets one solution with the best fitness in the population.
		cPtr best() const;

		/// Sets the levels' size's.
		void setLevelSize(const vector<size_t>& sizes);
		/// Gets the number of levels.
		// TODO: return a pointer to the vector
		const vector<unsigned>& getLevels() const { return this->levelSize; }

		unsigned avarageFittness() const;
	};
}
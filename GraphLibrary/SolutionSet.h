#pragma once

#include <memory>
#include <vector>
#include <queue>
#include "Solution.h"

using std::vector;
using std::shared_ptr;

/*********************************
* This class represents a population
* of solutions.
**********************************/

namespace msonlab
{
	class chrComparator
	{
	public:
		bool operator() (const shared_ptr<Solution>& a, const shared_ptr<Solution>& b) const
		{
			return a->getFitness() > b->getFitness();
		}
	};

	using msonlab::Solution;

	class SolutionSet
	{
		/// this contains the size of the graph's levels
		vector<size_t> levelSize;
		vector<shared_ptr<Solution>> solution;
		//typedef shared_ptr<Solution> cPtr;
		std::priority_queue<Solution::sPtr, vector<Solution::sPtr>, chrComparator> newGeneration;
		const size_t POPMAXSIZE;
		const size_t KEEP;
		const size_t KEEPBEST;
		
	public:
		typedef std::unique_ptr<SolutionSet> setPtr;
		SolutionSet(const vector<Solution::sPtr>& sol, size_t keepSize, size_t popMaxSize, size_t keepBest);

		const vector<Solution::sPtr>& getSolutionSet() const;

		/// Gets a random solution from the solution.
		Solution::sPtr getParent() const;
		/// Adds a new offspring to the population.
		void addOffspring(Solution::sPtr offspring);
		/// Adds one year to the age of all the solution
		void ageSolutions();
		/// Limits the number of solutions to the initial.
		void limit();
		/// Gets one solution with the best fitness in the population.
		Solution::sPtr best() const;

		/// Sets the levels' size's.
		void setLevelSize(const vector<size_t>& sizes);
		/// Gets the number of levels.
		// TODO: return a pointer to the vector
		const vector<unsigned>& getLevels() const { return this->levelSize; }

		SolutionSet & operator = (SolutionSet &);

		unsigned avarageFittness() const;
	};
}
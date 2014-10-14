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

namespace msonlab {
	namespace scheduling {

		class chrComparator
		{
		public:
			bool operator() (const SolutionPtr& a, const SolutionPtr& b) const
			{
				return a->getFitness() > b->getFitness();
			}
		};

		class SolutionSet
		{
			/// this contains the size of the graph's levels
			vector<size_t> levelSize;
			vector<SolutionPtr> solution;
			//typedef shared_ptr<Solution> cPtr;
			std::priority_queue<SolutionPtr, vector<SolutionPtr>, chrComparator> newGeneration;
			const size_t POPMAXSIZE;
			const size_t KEEP;
			const size_t KEEPBEST;
		public:
			typedef std::shared_ptr<SolutionSet> setPtr;
			SolutionSet(const vector<SolutionPtr>& sol, size_t keepSize, size_t popMaxSize, size_t keepBest);
			SolutionSet(const SolutionSet&& set);

			const vector<SolutionPtr>& getSolutionSet() const;

			/// Gets a random solution from the solution.
			SolutionPtr getParent() const;
			/// Adds a new offspring to the population.
			void addOffspring(SolutionPtr offspring);
			/// Adds one year to the age of all the solution
			void ageSolutions();
			/// Limits the number of solutions to the initial.
			void limit();
			/// Gets one solution with the best fitness in the population.
			SolutionPtr best() const;

			/// Sets the levels' size's.
			void setLevelSize(const vector<size_t>& sizes);
			/// Gets the number of levels.
			// TODO: return a pointer to the vector
			const vector<unsigned>& getLevels() const { return this->levelSize; }

			unsigned avarageFittness() const;
		};
	}
}
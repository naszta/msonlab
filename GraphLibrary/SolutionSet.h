#pragma once

#include <memory>
#include <vector>
#include <queue>
#include "SchedulingResult.h"


using std::vector;
using std::shared_ptr;

/*********************************
* This class represents a population
* of solutions.
**********************************/

namespace msonlab {
	namespace scheduling {

		// designed to compare results by their fitness
		class SchedulingResultComparator
		{
		public:
			bool operator() (const SchedulingResultPtr<const lw::lwnode*>& a, const SchedulingResultPtr<const lw::lwnode*> &b) const
			{
				return a->fitness() > b->fitness();
			}
		};

		class SolutionSet
		{
		public:
			SolutionSet(const size_t keepSize, const size_t popMaxSize, const size_t keepBest);
			SolutionSet(const vector<SchedulingResultPtr<const lw::lwnode*>>& sol, const size_t keepSize, const size_t popMaxSize, const size_t keepBest);
			SolutionSet(const SolutionSet&& set);

			//const vector<SchedulingResultLWPtr>& getSolutionSet() const;

			// Gets a random solution from the population.
			SchedulingResultPtr<const lw::lwnode*> getParent() const;
			// Adds a new offspring to the population
			void addOffspring(const SchedulingResultPtr<const lw::lwnode*> offspring);
			// Adds one year to the age of all the solution
			void ageSolutions();
			// Limits the number of solutions to the initial, keeping the ones with the highest fitness
			void limit();
			// Gets one solution with the best fitness in the population.
			SchedulingResultPtr<const lw::lwnode*> best() const;

			// Sets the levels' size's.
			void setLevelSize(const vector<size_t>& sizes);
			// Gets the number of levels.
			const vector<unsigned>& getLevels() const { return this->levelSize; }

			// Calculates the average fitness of the solutions
			unsigned averageFittness() const;
		private:
			// Contains the size of the graph's levels
			vector<size_t> levelSize;
			// The vector of the solutions
			vector<SchedulingResultPtr<const lw::lwnode*>> solution;
			// Orders the enqueued solutions by the fitnesses
			std::priority_queue<SchedulingResultPtr<const lw::lwnode*>, vector<SchedulingResultPtr<const lw::lwnode*>>, SchedulingResultComparator> newGeneration;
			const size_t POPMAXSIZE;
			const size_t KEEP;
			const size_t KEEPBEST;
		};

		typedef std::shared_ptr<SolutionSet> SolutionSetPtr;
	}
}
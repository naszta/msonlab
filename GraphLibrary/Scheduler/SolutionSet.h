#ifndef SOLUTION_SET_H
#define SOLUTION_SET_H

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

namespace msonlab { namespace scheduling {

	// designed to compare results by their fitness
	class SchedulingResultComparator
	{
	public:
		bool operator() (const SchedulingResultPtr<const lite::litenode*>& a, const SchedulingResultPtr<const lite::litenode*> &b) const
		{
			return a->fitness() > b->fitness();
		}
	};

	class SolutionSet
	{
	public:
		SolutionSet(const size_t keepSize, const size_t popMaxSize, const size_t keepBest);
		SolutionSet(const vector<SchedulingResultPtr<const lite::litenode*>>& sol, const size_t keepSize, const size_t popMaxSize, const size_t keepBest);
		SolutionSet(const SolutionSet& set) = delete;
		SolutionSet(const SolutionSet&& set);

		SolutionSet& operator=(const SolutionSet&) = delete;

		// Gets a random solution from the population.
		SchedulingResultPtr<const lite::litenode*> getParent() const;
		// Adds a new offspring to the population
		void addOffspring(const SchedulingResultPtr<const lite::litenode*> offspring);
		// Adds one year to the age of all the solution
		void ageSolutions();
		// Limits the number of solutions to the initial, keeping the ones with the highest fitness
		void limit();
		// Gets one solution with the best fitness in the population.
		SchedulingResultPtr<const lite::litenode*> best() const { return _best; };
		SchedulingResultPtr<const lite::litenode*> ultimate() const { return _ultimate; };
		size_t size() const { return _size; }
		unsigned last_improvement() const { return _last_improvement; }
		void randomized() { _last_improvement = 0; }

		// Sets the levels' size's.
		void setLevelSize(const vector<size_t>& sizes);
		// Gets the number of levels.
		const vector<unsigned>& getLevels() const { return this->levelSize; }

		// Calculates the average fitness of the solutions
		unsigned averageFittness() const;
	private:
		SchedulingResultPtr<const lite::litenode*> _best;
		SchedulingResultPtr<const lite::litenode*> _ultimate;
		unsigned _size;
		unsigned _last_improvement;
		// Contains the size of the graph's levels
		vector<size_t> levelSize;
		// The vector of the solutions
		vector<SchedulingResultPtr<const lite::litenode*>> solution;
		// Orders the enqueued solutions by the fitnesses
		std::priority_queue<SchedulingResultPtr<const lite::litenode*>, vector<SchedulingResultPtr<const lite::litenode*>>, SchedulingResultComparator> newGeneration;
		const size_t POPMAXSIZE;
		const size_t KEEP;
		const size_t KEEPBEST;
	};

	typedef std::shared_ptr<SolutionSet> SolutionSetPtr;
}}
#endif
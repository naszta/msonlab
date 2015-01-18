#include "SolutionSet.h"
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <utility>
#include "Global.h"

namespace msonlab {
	namespace scheduling {
		bool solutionCompare(SchedulingResultPtr<const lw::lwnode*> a, SchedulingResultPtr<const lw::lwnode*> b)
		{
			return a->fitness() > b->fitness();
		}

		SolutionSet::SolutionSet(size_t keepSize, size_t popMaxSize, size_t keepBest) : solution(vector<SchedulingResultPtr<const lw::lwnode*>>(KEEP)), POPMAXSIZE(popMaxSize), KEEP(keepSize), KEEPBEST(keepBest)
		{
		}

		SolutionSet::SolutionSet(const vector<SchedulingResultPtr<const lw::lwnode*>>& sol, size_t keepSize, size_t popMaxSize, size_t keepBest) : solution(sol), POPMAXSIZE(popMaxSize), KEEP(keepSize), KEEPBEST(keepBest)
		{
			this->solution.resize(KEEP);
		}

		// move constructor
		SolutionSet::SolutionSet(const SolutionSet&& set)
			: levelSize(std::move(set.levelSize)), solution(std::move(set.solution)),
			newGeneration(std::move(set.newGeneration)),
			POPMAXSIZE(set.POPMAXSIZE), KEEP(set.KEEP), KEEPBEST(set.KEEPBEST)
		{
		}

		//const vector<SchedulingResultLWPtr>& SolutionSet::getSolutionSet() const
		//{
		//	return this->solution;
		//}

		// adds a new offspring to the population
		void SolutionSet::addOffspring(SchedulingResultPtr<const lw::lwnode*> offspring)
		{
			this->newGeneration.push(offspring);
		}

		// Increase age of the solutions.
		void SolutionSet::ageSolutions()
		{
			throw Exceptions::NotImplementedException("This feature is not implemented yet");
		}

		// Limits the population to KEEP size.
		// It keeps the solutions with the best fitness.
		void SolutionSet::limit()
		{
			for (size_t i = 0; i < KEEP; ++i)
			{
				solution[i] = newGeneration.top();
				newGeneration.pop();
			}

			while (!this->newGeneration.empty())
				this->newGeneration.pop();

			for (int i = 0; i < 5; ++i)
			{
				newGeneration.push(solution[i]);
			}
		}

		// Gets a random solution from the set
		SchedulingResultPtr<const lw::lwnode*> SolutionSet::getParent() const
		{
			return this->solution[rand() % solution.size()];
		}

		// Gets the best solution from the population
		SchedulingResultPtr<const lw::lwnode*> SolutionSet::best() const
		{
			return solution[0];
		}

		// Calculates the avarage fitness of the population.
		unsigned SolutionSet::averageFittness() const
		{
			unsigned sum = 0;
			for (size_t i = 0; i < solution.size(); ++i)
			{
				sum += solution[i]->fitness();
			}

			return sum / solution.size();
		}

		// Sets the levels' size's.
		void SolutionSet::setLevelSize(const vector<size_t>& sizes)
		{
			if (this->levelSize.size() != 0)
				throw Exceptions::GeneralErrorException("Level size can be set only once.");
			for (vector<size_t>::const_iterator it = sizes.begin(); it != sizes.end(); ++it)
			{
				this->levelSize.push_back(*it);
			}
		}
	}
}
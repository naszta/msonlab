#include "SolutionSet.h"
#include <algorithm>
#include <queue>
#include "GeneticAlgorithm.h"
#include <cstdlib>
#include <ctime>

namespace msonlab {
	namespace scheduling {
		bool solutionCompare(shared_ptr<Solution> a, shared_ptr<Solution> b)
		{
			return a->getFitness() > b->getFitness();
		}

		SolutionSet::SolutionSet(const vector<SolutionPtr>& sol, size_t keepSize, size_t popMaxSize, size_t keepBest) : solution(sol), POPMAXSIZE(popMaxSize), KEEP(keepSize), KEEPBEST(keepBest)
		{
			this->solution.resize(KEEP);
		}

		const vector<shared_ptr<Solution>>& SolutionSet::getSolutionSet() const
		{
			return this->solution;
		}

		///
		/// adds a new offspring to the population
		///
		/// @param offspring the solution to add.
		void SolutionSet::addOffspring(SolutionPtr offspring)
		{
			this->newGeneration.push(offspring);
		}

		///
		/// Increase age of the solutions.
		///
		void SolutionSet::ageSolutions()
		{
			throw Exceptions::NotImplementedException("This feature is not implemented yet");
		}

		///
		/// Limits the population to KEEP size.
		/// 
		/// It keeps the solutions with the best fitness.
		///
		void SolutionSet::limit()
		{
			//std::priority_queue<cPtr, vector<cPtr>, chrComparator> best(solution.begin(), solution.end());
			//solution.resize(KEEP);

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

		///
		/// gets a potential parent from the population randomly
		///
		/// @return A solution from the offspring will be created.
		shared_ptr<Solution> SolutionSet::getParent() const
		{
			return this->solution[rand() % solution.size()];
		}

		///
		/// Gets the best solution from the population
		///
		/// @return A solution with the best fitness value.
		shared_ptr<Solution> SolutionSet::best() const
		{
			return solution[0];
		}

		///
		/// Calculates the avarage fitness of the population.
		///
		/// @return The avarage fitness of the population.
		unsigned SolutionSet::avarageFittness() const
		{
			unsigned sum = 0;
			for (size_t i = 0; i < solution.size(); ++i)
			{
				sum += solution[i]->getFitness();
			}

			return sum / solution.size();
		}

		///
		/// Sets the levels' size's.
		///
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
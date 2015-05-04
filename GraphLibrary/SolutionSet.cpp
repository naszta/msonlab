#include "SolutionSet.h"
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <stdexcept>
#include "Global.h"

namespace msonlab { namespace scheduling {
	bool solutionCompare(SchedulingResultPtr<const lite::litenode*> a, SchedulingResultPtr<const lite::litenode*> b)
	{
		return a->fitness() > b->fitness();
	}

	SolutionSet::SolutionSet(size_t keepSize, size_t popMaxSize, size_t keepBest) :
		solution(vector<SchedulingResultPtr<const lite::litenode*>>(keepSize)), POPMAXSIZE(popMaxSize), KEEP(keepSize), KEEPBEST(keepBest), _size(0)
	{
	}

	SolutionSet::SolutionSet(const vector<SchedulingResultPtr<const lite::litenode*>>& sol, size_t keepSize, size_t popMaxSize, size_t keepBest) : solution(sol), POPMAXSIZE(popMaxSize), KEEP(keepSize), KEEPBEST(keepBest), _size(0)
	{
		this->solution.resize(KEEP);
	}

	// move constructor
	SolutionSet::SolutionSet(const SolutionSet&& set)
		: levelSize(std::move(set.levelSize)), solution(std::move(set.solution)),
		newGeneration(std::move(set.newGeneration)),
		POPMAXSIZE(set.POPMAXSIZE), KEEP(set.KEEP), KEEPBEST(set.KEEPBEST), _size(0)
	{
	}

	//const vector<SchedulingResultLWPtr>& SolutionSet::getSolutionSet() const
	//{
	//	return this->solution;
	//}

	// adds a new offspring to the population
	void SolutionSet::addOffspring(SchedulingResultPtr<const lite::litenode*> offspring)
	{
		this->newGeneration.push(offspring);
		++this->_size;
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

		this->_size = 0;
		this->_best = this->solution[0];
		if (this->_ultimate == nullptr || this->_best->fitness() < this->_ultimate->fitness()) {
			this->_ultimate = this->_best;
		}
	}

	// Gets a random solution from the set
	SchedulingResultPtr<const lite::litenode*> SolutionSet::getParent() const
	{
		return this->solution[rand() % solution.size()];
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
			throw std::exception("Level size can be set only once.");
		for (vector<size_t>::const_iterator it = sizes.begin(); it != sizes.end(); ++it)
		{
			this->levelSize.push_back(*it);
		}
	}
}}
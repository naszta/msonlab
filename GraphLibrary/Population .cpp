#include "Population.h"
#include <algorithm>
#include <queue>
#include "GeneticAlgorithm.h"
#include <cstdlib>
#include <ctime>

namespace msonlab
{
	bool chromosomeCompare(shared_ptr<Chromosome> a, shared_ptr<Chromosome> b)
	{
		return a->getFitness() > b->getFitness();
	}

	Population::Population(vector<cPtr> sol, size_t keepSize, size_t popMaxSize, size_t keepBest) : solution(sol), POPMAXSIZE(popMaxSize), KEEP(keepSize), KEEPBEST(keepBest)
	{
		this->solution.resize(KEEP);
	}

	vector<shared_ptr<Chromosome>> Population::getPopulation()
	{
		return this->solution;
	}

	///
	// adds a new offspring to the population
	///
	void Population::addOffspring(cPtr offspring)
	{
		this->newGeneration.push(offspring);
	}

	///
	// increase age of the chromosomes
	///
	void Population::ageChromosomes()
	{
		throw Exceptions::NotImplementedException("This feature is not implemented yet");
	}

	///
	// limits the population to KEEP size
	// it keeps the chromosomes with the best fitness
	///
	void Population::limit()
	{
		//std::priority_queue<cPtr, vector<cPtr>, chrComparator> best(solution.begin(), solution.end());
		//solution.resize(KEEP);

		for (size_t i = 0; i < KEEP; ++i)
		{
			solution[i] = newGeneration.top();
			newGeneration.pop();
		}

		while(!this->newGeneration.empty())
			this->newGeneration.pop();

		for(int i = 0; i < 5; ++i)
		{
			newGeneration.push(solution[i]);
		}
	}

	///
	// gets a potential parent from the population randomly
	///
	shared_ptr<Chromosome> Population::getParent()
	{
		return this->solution[rand() % solution.size()];
	}

	///
	// gets the best chromosome from the population
	///
	shared_ptr<Chromosome> Population::best()
	{
		return solution[0];
	}

	unsigned Population::avarageFittness()
	{
		unsigned sum = 0;
		for (size_t i = 0; i < solution.size(); ++i)
		{
			sum += solution[i]->getFitness();
		}

		return sum / solution.size();
	}
}
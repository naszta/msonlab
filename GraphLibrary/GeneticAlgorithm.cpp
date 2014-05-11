#include "GeneticAlgorithm.h"
#include "GraphAlgorithms.h"
#include <cstdlib>
#include <ctime>
#include <set>
#include <queue>
#include <map>

namespace msonlab {

	GeneticAlgorithm::GeneticAlgorithm(Options::oPtr options, FitnessStrategy::fsPtr strategy) 
		: options(options), fsstrategy(strategy)
	{
	}

	///
	/// Constructs a chromosome using greedy scheduling.
	///
	/// @param The input graph.
	/// @return The constructed chromosome.
	Chromosome::cPtr GeneticAlgorithm::greedyChromosome(Graph::gPtr graph) const
	{
		unsigned timeCounter = 0;
		unsigned taskCounter = 0;
		std::map< IProcessable::nPtr, int> count;
		IProcessable::nVect inputNodes = graph->getInputNodes();
		std::queue < IProcessable::nPtr> free;
		for (size_t i = 0; i < inputNodes.size(); ++i)
		{
			free.push(inputNodes[i]);
		}

		shared_ptr<Chromosome> c(new Chromosome(graph->numberOfNodes(), options->getNumberOfPus()));
		while (taskCounter < graph->numberOfNodes())
		{
			vector< IProcessable::nPtr > out;
			int limit = c->pus;
			while (!free.empty() && limit > 0)
			{
				IProcessable::nPtr node = free.front();
				c->mapping[taskCounter] = limit - 1;
				c->scheduling[taskCounter] = node;

				out.push_back(node);
				free.pop();
				--limit;
				taskCounter++;
			}
			++timeCounter;

			if (taskCounter == graph->numberOfNodes())
			{
				break;
			}

			for (size_t i = 0; i < out.size(); ++i)
			{
				IProcessable::eVect successors = out[i]->getSuccessors();
				for (size_t i = 0; i < successors.size(); ++i)
				{
					count[successors[i]->getTo()]++;
					if (successors[i]->getTo()->getPredecessorsSize() == count[successors[i]->getTo()])
					{
						free.push(successors[i]->getTo());
					}
				}
			}
		}

		fitness(c);
		return c;
	}

	///
	/// Schedules the given graph.
	///
	/// For the schedule the Options of this GeneticAlgorithm is used.
	/// @param graph The graph to schedule.
	/// @return the best solution the GA finds.
	Chromosome::cPtr GeneticAlgorithm::schedule(Graph::gPtr graph, Options::oPtr options) const {
		auto population = this->generateInitialSolution(graph);
		population->limit();

		for (size_t i = 0; i < options->getNumberOfYears(); ++i)
		{
			simulateMating(population, options->getPopMaxSize());
			population->limit();
		}

		return population->best();
	}

	///
	/// Generating a random initial solution.
	///
	/// The mapping part is generated randomly, each task is randomly mapped to a PU.
	/// The scheduling part is generated using the leveling created by the GraphAlgorithms.
	/// 
	/// @param graph The input graph.
	/// @return The generated population.
	shared_ptr<Population> GeneticAlgorithm::generateInitialSolution(Graph::gPtr graph) const
	{
		cVect solution;
		vector<IProcessable::nVect> levels = algorithms.createLeveling(graph);
		size_t numLevels = levels.size();
		vector<unsigned> levelingLimits;
		DEBUG("levels: " << numLevels);
		unsigned limits = 0;
		for (size_t i = numLevels; i > 0; --i)
		{
			limits += levels[i - 1].size();
#if PRINT == 1
			std::cout << "Level " << i << " ---" << std::endl;
			for (size_t j = 0; j < levels[i - 1].size(); ++j)
			{
				std::cout << levels[i - 1][j]->getId() << " ";
			}

			std::cout << "\n---\n";
#endif
		}

		pPtr population = pPtr(new Population(solution, options->getKeepSize(), options->getPopMaxSize(), options->getKeepBest()));

		cPtr chr = this->greedyChromosome(graph);

		cPtr cc(new Chromosome(graph->numberOfNodes(), options->getNumberOfPus()));
		size_t currentPos = 0;
		unsigned counter = 0;
		for (size_t i = numLevels; i > 0; --i)
		{
			for (size_t j = 0; j < levels[i - 1].size(); ++j)
			{
				cc->mapping[counter] = j % cc->pus;
				++counter;
			}
			std::random_shuffle(levels[i - 1].begin(), levels[i - 1].end());
			std::copy(levels[i - 1].begin(), levels[i - 1].end(), cc->scheduling.begin() + currentPos);
			currentPos += levels[i - 1].size();
		}

		fitness(cc);
		DEBUG("First fitness: " << cc->fitness);
		population->addOffspring(cc);

		counter = options->getPopMaxSize() - 1;
		for (; counter > 0; --counter)
		{
			shared_ptr<Chromosome> c(new Chromosome(graph->numberOfNodes(), options->getNumberOfPus()));
			for (unsigned int i = 0; i < c->mapping.size(); ++i)
			{
				c->mapping[i] = rand() % c->pus;
			}

			size_t currentPos = 0;
			for (size_t i = numLevels; i > 0; --i)
			{
				if (counter == 1)
				{
					levelingLimits.push_back(currentPos);
				}

				std::random_shuffle(levels[i - 1].begin(), levels[i - 1].end());
				std::copy(levels[i - 1].begin(), levels[i - 1].end(), c->scheduling.begin() + currentPos);
				currentPos += levels[i - 1].size();
			}

			population->setLevelSize(levelingLimits);
			fitness(c);
			population->addOffspring(c);
		}

		return population;
	}

	///
	/// calculate fitness for a given chromosem
	///
	/// @param chromosome Which's fitness is calculated.
	/// @return The fitness of the chromosome.
	unsigned int GeneticAlgorithm::fitness(cPtr chromosome) const
	{
		if (chromosome->fitness > 0)
		{
			return chromosome->fitness;
		}

		chromosome->fitness = this->fsstrategy->fitness(chromosome, options);
		return chromosome->fitness;
	}

	///
	/// Crossover operation between the mapping part of chromosome.
	///
	/// @param father One parent of the offspring.
	/// @param mother Another parent of the offspring.
	/// @return the offspring.
	Chromosome::cPtr GeneticAlgorithm::crossoverMap(cPtr father, cPtr mother) const
	{
		cPtr offspring(new Chromosome(*father));
		uint crossoverPoint = rand() % father->mapping.size();
		std::copy(mother->mapping.begin() + crossoverPoint, mother->mapping.end(), offspring->mapping.begin() + crossoverPoint);

		return offspring;
	}

	///
	/// Crossover operation between the scheduling part of chromosome.
	///
	/// @param father One parent of the offspring.
	/// @param mother Another parent of the offspring.
	/// @return the offspring.
	Chromosome::cPtr GeneticAlgorithm::crossoverOrder(cPtr father, cPtr mother, const vector<unsigned>& levelingLimits) const
	{
		cPtr offsrping(new Chromosome(*father));
		uint crossoverPoint = rand() % levelingLimits.size();

		std::copy(mother->scheduling.begin() + levelingLimits[crossoverPoint], mother->scheduling.end(), offsrping->scheduling.begin() + levelingLimits[crossoverPoint]);

		return offsrping;
	}

	///
	/// Mutates the mapping part.
	///
	/// Changes the mapping of a task to another PU.
	/// The mutation happens with [MutationPercentage] probablity
	/// and the number of mutation points are [MutationRate].
	///
	/// @param offspring chromosome to mutate
	void GeneticAlgorithm::mutateMapping(cPtr offspring) const
	{
		for (uint i = 0; i < options->getMutationRate(); ++i)
		{
			unsigned rate = rand() % 100;
			if (rate < options->getMutationPercentage())
			{
				int position = rand() % offspring->scheduling.size();
				int mutation = rand() % (offspring->pus - 1) + 1;
				offspring->mapping[position] += mutation;
				if (offspring->mapping[position] >= offspring->pus)
				{
					offspring->mapping[position] -= offspring->pus;
				}
			}
		}
	}

	/// Mutates the scheduling part.
	///
	/// The mutation happens with [MutationPercentage] probability.
	/// In case of a mutation a level is choosen and shuffled.
	///
	/// @param offspring The chromosome to mutate.
	void GeneticAlgorithm::mutateSheduling(cPtr offspring, const vector<unsigned>& levelingLimits) const
	{
		unsigned rate = rand() % 100;
		if (rate < options->getMutationPercentage())
		{
			int position = rand() % levelingLimits.size();
			IProcessable::nVect::iterator begin = offspring->scheduling.begin() + levelingLimits[position];
			IProcessable::nVect::iterator end = position == levelingLimits.size() - 1 ? offspring->scheduling.end() : offspring->scheduling.begin() + levelingLimits[position + 1];
			std::random_shuffle(begin, end);
		}
	}

	///
	/// Generates offsprings.
	/// 
	/// Randomly chooses parents from the population, uses a crossover operation on them and
	/// mutates the offspring. The offspring is added to the population. Totally [offsprings]
	/// number of offsprings are added to the populationn.
	///
	/// @param population A set of chromosomes to choose the parents from.
	/// @param offsprings The number of offsprings to generate.
	void GeneticAlgorithm::simulateMating(shared_ptr<Population> population, int offsprings) const
	{
		//population->addOffspring(population->best());
		for (int i = 0; i < offsprings; i++)
		{
			cPtr father = population->getParent();
			cPtr mother = population->getParent();
			int crossoverType = rand() % 2;
			cPtr offspring;
			if (crossoverType == 0)
			{
				offspring = crossoverMap(father, mother);
			}
			else
			{
				offspring = crossoverOrder(father, mother, population->getLevels());
			}

			mutateMapping(offspring);
			mutateSheduling(offspring, population->getLevels());
			fitness(offspring);
			population->addOffspring(offspring);
		}
	}

	///
	/// Transform the mapping and scheduling into one vector.
	///
	/// Assumes that the id's of the tasks are from 0 to n-1, where n is the number
	/// nodes in the graph.
	///
	/// @param c the chromosome to transform
	/// @param result the result vector.
	void GeneticAlgorithm::transfromResult(Chromosome::cPtr c, vector<unsigned>& result) const {
		auto mapping = c->getMapping();
		auto scheduling = c->getScheduling();
		size_t tasks = scheduling.size();
		if (result.size() != tasks) {
			result.resize(tasks);
		}

		for (unsigned i = 0; i < tasks; ++i) {
			result[scheduling[i]->getId()] = mapping[i];
		}
	}
}
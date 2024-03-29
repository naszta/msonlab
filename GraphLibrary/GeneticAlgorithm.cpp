#include "GeneticAlgorithm.h"
#include "GraphAlgorithms.h"
#include "HusSchedulingAlgorithm.h"
#include <cstdlib>
#include <ctime>
#include <set>
#include <queue>
#include <map>
#include <memory>

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
	Chromosome::cPtr GeneticAlgorithm::greedyChromosome(Graph::gPtr& graph) const
	{
		unsigned timeCounter = 0;
		unsigned taskCounter = 0;
		std::map< IProcessable::nPtr, unsigned> count;
		IProcessable::nVect inputNodes = graph->getInputNodes();
		std::queue < IProcessable::nPtr> free;
		for (size_t i = 0; i < inputNodes.size(); ++i)
		{
			free.push(inputNodes[i]);
		}

		auto c = std::make_shared<Chromosome>(graph->numberOfNodes(), options->getNumberOfPus());
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
	Chromosome::cPtr GeneticAlgorithm::schedule(Graph::gPtr& graph, Options::oPtr options) const {
		Population::pPtr population = this->generateInitialSolution(graph, options);
		population->limit();
		
		bool doOrderCrossover = options->getFitnessStrategy().compare("reschedule") != 0;
		for (size_t i = 0; i < options->getNumberOfYears(); ++i)
		{
			std::cout << "Round " << i + 1 << std::endl;
			simulateMating(population, options->getPopMaxSize(), doOrderCrossover);
			population->limit();
		}

		return population->best();
	}

	Population::pPtr GeneticAlgorithm::generateInitialSolution(Graph::gPtr& graph, Options::oPtr options) const
	{
		Population::pPtr p;
		if (options->getInitialSolution().compare("cp") == 0) {
			vector<IProcessable::nVect> levels = algorithms.partialTopologicalSort(graph);
			size_t numLevels = levels.size();
			vector<unsigned> levelingLimits;
			unsigned limits = 0;
			for (size_t i = numLevels; i > 0; --i)
			{
				DEBUG("Level: " << i);
				for (auto it = levels[i - 1].begin(); it != levels[i - 1].end(); ++it){
					DEBUG(" " << (*it)->getId());
				}

				DEBUGLN("");
				limits += levels[i - 1].size();
				levelingLimits.push_back(limits);
			}
			p = this->generateCPSolution(graph, options);
			p->setLevelSize(levelingLimits);
		}
		else {
			p = this->generateRndSolution(graph, options);
		}

		
		return p;
	}

	///
	/// Generating a random initial solution.
	///
	/// The mapping part is generated randomly, each task is randomly mapped to a PU.
	/// The scheduling part is generated using the leveling created by the GraphAlgorithms.
	/// 
	/// @param graph The input graph.
	/// @return The generated population.
	Population::pPtr GeneticAlgorithm::generateRndSolution(Graph::gPtr& graph, Options::oPtr options) const
	{
		cVect solution;
		vector<IProcessable::nVect> levels = algorithms.partialTopologicalSort(graph);
		size_t numLevels = levels.size();
		vector<unsigned> levelingLimits;
		unsigned limits = 0;
		for (size_t i = numLevels; i > 0; --i)
		{
			DEBUG("Level: " << i);
			for (auto it = levels[i - 1].begin(); it != levels[i - 1].end(); ++it){
				DEBUG(" " << (*it)->getId());
			}

			DEBUGLN("");
			limits += levels[i - 1].size();
		}

		Population::pPtr population = std::make_unique<Population>(solution, options->getKeepSize(), options->getPopMaxSize(), options->getKeepBest());

		Chromosome::cPtr chr = this->greedyChromosome(graph);

		Chromosome::cPtr cc = std::make_shared<Chromosome>(graph->numberOfNodes(), options->getNumberOfPus());
		size_t currentPos = 0;
		unsigned counter = 0;
		for (size_t i = numLevels; i > 0; --i)
		{
			// set mapping for the level
			for (size_t j = 0; j < levels[i - 1].size(); ++j)
			{
				cc->mapping[counter] = j % cc->pus;
				++counter;
			}

			// random shuffle the level
			std::random_shuffle(levels[i - 1].begin(), levels[i - 1].end());
			std::copy(levels[i - 1].begin(), levels[i - 1].end(), cc->scheduling.begin() + currentPos);
			levelingLimits.push_back(currentPos);
			currentPos += levels[i - 1].size();
		}

		auto initialFitness = fitness(cc);
		DEBUGLN("Population created. Initial fitness : " << initialFitness);
		population->addOffspring(cc);
		population->setLevelSize(levelingLimits);

		// the number of solutions to generate
		counter = options->getPopMaxSize() - 1;
		for (; counter > 0; --counter)
		{
			auto c = std::make_shared<Chromosome>(graph->numberOfNodes(), options->getNumberOfPus());
			for (unsigned int i = 0; i < c->mapping.size(); ++i)
			{
				c->mapping[i] = rand() % c->pus;
			}

			size_t currentPos = 0;
			for (size_t i = numLevels; i > 0; --i)
			{
				std::random_shuffle(levels[i - 1].begin(), levels[i - 1].end());
				std::copy(levels[i - 1].begin(), levels[i - 1].end(), c->scheduling.begin() + currentPos);
				currentPos += levels[i - 1].size();
			}

			fitness(c);
			population->addOffspring(c);
		}

		return population;
	}

	// Generate the chromosome using the scheduling created by the CP scheduler
	Population::pPtr GeneticAlgorithm::generateCPSolution(Graph::gPtr& graph, Options::oPtr options) const {
		cVect solution;
		Population::pPtr population = std::make_unique<Population>(solution, options->getKeepSize(), options->getPopMaxSize(), options->getKeepBest());
		HusSchedulingAlgorithm cpAlg;
		auto greedy = greedyChromosome(graph);
		population->addOffspring(greedy);
		auto cpC = cpAlg.schedule(graph, options);
		auto initialFitness = fitness(cpC);
		population->addOffspring(cpC);
		unsigned counter = options->getPopMaxSize() - 2; // CP and greedy
		for (; counter > 0; --counter)
		{
			auto c = std::make_shared<Chromosome>(graph->numberOfNodes(), options->getNumberOfPus());
			for (unsigned int i = 0; i < c->mapping.size(); ++i)
			{
				c->mapping[i] = rand() % c->pus;
			}

			std::copy(cpC->scheduling.begin(), cpC->scheduling.end(), c->scheduling.begin());
			fitness(c);
			population->addOffspring(c);
		}

		DEBUGLN("Population created. Initial fitness : " << initialFitness);
		return population;
	}

	///
	/// calculate fitness for a given chromosem
	///
	/// @param chromosome Which's fitness is calculated.
	/// @return The fitness of the chromosome.
	unsigned int GeneticAlgorithm::fitness(Chromosome::cPtr chromosome) const
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
	Chromosome::cPtr GeneticAlgorithm::crossoverMap(Chromosome::cPtr father, Chromosome::cPtr mother) const
	{
		Chromosome::cPtr offspring = std::make_shared<Chromosome>(*father);
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
	Chromosome::cPtr GeneticAlgorithm::crossoverOrder(Chromosome::cPtr father, Chromosome::cPtr mother, const vector<unsigned>& levelingLimits) const
	{
		Chromosome::cPtr offspring = std::make_shared<Chromosome>(*father);
		uint crossoverPoint = rand() % levelingLimits.size();

		std::copy(mother->scheduling.begin() + levelingLimits[crossoverPoint], mother->scheduling.end(), offspring->scheduling.begin() + levelingLimits[crossoverPoint]);

		return offspring;
	}

	///
	/// Mutates the mapping part.
	///
	/// Changes the mapping of a task to another PU.
	/// The mutation happens with [MutationPercentage] probablity
	/// and the number of mutation points are [MutationRate].
	///
	/// @param offspring chromosome to mutate
	void GeneticAlgorithm::mutateMapping(Chromosome::cPtr& offspring) const
	{
		unsigned rate = rand() % 100;
		if (rate < this->options->getMapMutationRate())
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

	/// Mutates the scheduling part.
	///
	/// The mutation happens with [MutationPercentage] probability.
	/// In case of a mutation a level is choosen and shuffled.
	///
	/// @param offspring The chromosome to mutate.
	void GeneticAlgorithm::mutateSheduling(Chromosome::cPtr offspring, const vector<unsigned>& levelingLimits) const
	{
		//unsigned rate = rand() % 100;
		//if (rate < options->getScheduleMutationRate())
		//{
			int position = rand() % levelingLimits.size();
			IProcessable::nVect::iterator begin = offspring->scheduling.begin() + levelingLimits[position];
			IProcessable::nVect::iterator end = position == levelingLimits.size() - 1 ? offspring->scheduling.end() : offspring->scheduling.begin() + levelingLimits[position + 1];
			std::random_shuffle(begin, end);
		//}
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
	void GeneticAlgorithm::simulateMating(Population::pPtr& population, int offsprings, bool doOrderCrossover) const
	{
		int faultyGens = 0;
		int mutations = 0;
		for (; offsprings > 0;)
		{
			Chromosome::cPtr father = population->getParent();
			Chromosome::cPtr mother = population->getParent();
			int crossoverType = rand() % 2;
			Chromosome::cPtr offspring;
			// always mapping crossover
			if (crossoverType == 0 || !doOrderCrossover)
			{
				offspring = crossoverMap(father, mother);
			}
			else
			{
				offspring = crossoverOrder(father, mother, population->getLevels());
			}

			mutateMapping(offspring);
			unsigned rate = rand() % 100;
			if (rate < options->getScheduleMutationRate()) {
				mutateSheduling(offspring, population->getLevels());
				++mutations;
			}

			unsigned cost = fitness(offspring);
			if (cost < UINT32_MAX)
			{
				population->addOffspring(offspring);
				--offsprings;
			}
			else ++faultyGens;
		}

		DEBUGLN("Mutations happened   " << mutations);
		DEBUGLN("Faulty gens created  " << faultyGens);
		DEBUGLN("Successful mutations " << mutations - faultyGens);
	}

	///
	/// Transform the mapping and scheduling into one vector.
	///
	/// Assumes that the id's of the tasks are from 0 to n-1, where n is the number
	/// nodes in the graph.
	///
	/// @param c the chromosome to transform
	/// @param result the result vector.
	void GeneticAlgorithm::transfromResult(Chromosome::cPtr c, vector<unsigned>& result) {
		auto mapping = c->getMapping();
		auto scheduling = c->getScheduling();
		size_t tasks = scheduling.size();
		if (result.size() != 17) {
			result.resize(17);
		}

		for (unsigned i = 0; i < tasks; ++i) {
			result[scheduling[i]->getId()] = mapping[i];
			Node::nPtr node = scheduling[i];
			for (auto it = node->getPredecessorBegin(); it != node->getPredecessorEnd(); ++it)
			{
				result[(*it)->getId()] = mapping[i];
			}
		}
	}

	// EXPERIMENTAL

	void GeneticAlgorithm::swapMutateScheduling(Chromosome::cPtr offspring) const
	{
		unsigned rate = rand() % 100;
		if (rate < options->getScheduleMutationRate())
		{
			unsigned toswap = rand() % offspring->scheduling.size() - 2;
			std::iter_swap(offspring->scheduling.begin() + toswap,
				offspring->scheduling.begin() + toswap + 1);

		}
	}
}
#include "GeneticAlgorithm.h"
#include "GraphAlgorithms.h"
#include <cstdlib>
#include <ctime>
#include <set>
#include <queue>
#include <map>

namespace msonlab {
	unsigned int max(unsigned int a, unsigned int b)
	{
		return a > b ? a : b;
	}

	GeneticAlgorithm::GeneticAlgorithm()
	{
	}

	GeneticAlgorithm::GeneticAlgorithm(shared_ptr<GAOptions> options) : gaoptions(options)
	{
	}

	shared_ptr<Chromosome> GeneticAlgorithm::greedyChromosome(Graph::gPtr graph)
	{
		unsigned timeCounter = 0;
		unsigned taskCounter = 0;
		std::map< IProcessable::nPtr , int> count;
		IProcessable::nVect inputNodes = graph->getInputNodes();
		std::queue < IProcessable::nPtr> free;
		for (size_t i = 0; i < inputNodes.size(); ++i)
		{
			free.push(inputNodes[i]);
		}

		shared_ptr<Chromosome> c(new Chromosome(graph->numberOfNodes()));
		c->pus = gaoptions->getNumberOfPus();
		while (taskCounter < graph->numberOfNodes())
		{
			vector< IProcessable::nPtr > out;
			int limit = c->pus;
			while ( !free.empty() && limit > 0 )
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
					if ( successors[i]->getTo()->getPredecessorsSize() == count[successors[i]->getTo()])
					{
						free.push(successors[i]->getTo());
					}
				}	
			}
		}

		std::cout << "Greedy time: " << timeCounter << std::endl;
		fitness(c);
		std::cout << "Greedy fitness: " << c->fitness << std::endl;
		return c;
	}

	///
	// generating a random initial solution
	///
	shared_ptr<Population> GeneticAlgorithm::generateInitialSolution(Graph::gPtr graph)
	{
		vector<shared_ptr<Chromosome>> solution;
		vector<IProcessable::nVect> levels = algorithms.createLeveling(graph);
		size_t numLevels = levels.size();
		std::cout << "levels: " << numLevels << std::endl;
		unsigned limits = 0;
		for (size_t i = numLevels; i > 0; --i)
		{
			limits += levels[i-1].size();
			std::cout << "Level " << i << " ---" << std::endl;
			for (size_t j = 0; j < levels[i-1].size(); ++j)
			{
				std::cout << levels[i-1][j]->getId() << " ";
			}

			std::cout << "\n---\n";
		}

		shared_ptr<Population> population = shared_ptr<Population>(new Population(solution, gaoptions->getKeepSize(), gaoptions->getPopMaxSize(), gaoptions->getKeepBest()));
		
		shared_ptr<Chromosome> chr = this->greedyChromosome(graph);

		shared_ptr<Chromosome> cc(new Chromosome(graph->numberOfNodes()));
		cc->pus = gaoptions->getNumberOfPus();
		size_t currentPos = 0;
		unsigned counter = 0;
		for (size_t i = numLevels; i > 0; --i)
		{
			for (size_t j = 0; j < levels[i-1].size(); ++j)
			{
				cc->mapping[counter] = j % cc->pus;
				++counter;
			}
			std::random_shuffle(levels[i-1].begin(), levels[i-1].end());
			std::copy(levels[i-1].begin(), levels[i-1].end(), cc->scheduling.begin() + currentPos);
			currentPos += levels[i-1].size();
		}

		fitness(cc);
		std::cout << "First fitness: " << cc->fitness << std::endl;
		population->addOffspring(cc);

		counter = gaoptions->getPopMaxSize()-1;
		for (; counter > 0; --counter)
		{
			shared_ptr<Chromosome> c(new Chromosome(graph->numberOfNodes()));
			c->pus = gaoptions->getNumberOfPus();
			for (unsigned int i = 0; i < c->mapping.size(); ++i)
			{
				c->mapping[i] = rand() % c->pus;
			}

			size_t currentPos = 0;
			for (size_t i = numLevels; i > 0; --i)
			{
				if (counter == 1 )
				{
					levlingLimits.push_back(currentPos);
				}

				std::random_shuffle(levels[i-1].begin(), levels[i-1].end());
				std::copy(levels[i-1].begin(), levels[i-1].end(), c->scheduling.begin() + currentPos);
				currentPos += levels[i-1].size();
			}

			fitness(c);
			population->addOffspring(c);
		}

		return population;
	}

	///
	// calculate fitness for a given chromosem
	///
	unsigned int GeneticAlgorithm::fitness(cPtr chromosome)
	{
		//if (chromosome->fitness > 0)
		//{
		//	return chromosome->fitness;
		//}
		typedef unsigned int uint;
		uint tasks = chromosome->scheduling.size();
		vector<uint> RT(chromosome->pus); // ready time of the PUs
		vector<uint> ST(tasks); // start time of the tasks
		vector<uint> FT(tasks); // finish time of the tasks
		vector<uint> DAT(tasks); // Data Arrival Time
		vector<uint> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j] // is this necessary?

		uint actId = chromosome->scheduling[0]->getId();
		ST[actId] = 0;
		FT[actId] = ST[actId] + gaoptions->getTaskLength(); // it takes
		DAT[actId] = 0;
		idPuMapping[actId] = chromosome->mapping[0];
		RT[chromosome->mapping[0]] = FT[actId];

		// skipping first task
		for (uint i = 1; i < tasks; ++i)
		{
			IProcessable::nPtr actNode = chromosome->scheduling[i];
			uint actId = actNode->getId();
			uint actPU = chromosome->mapping[i];
			idPuMapping[actId] = actPU;

			// calculating data arrival time
			//IProcessable::eVect::iterator it = actNode->getPredecessorBegin();
			//IProcessable::eVect::iterator end = actNode->getPredecessorEnd();
			//for (; it != end; ++it)
			//uint id = (*it)->getFromId();
			size_t predecessorSize = actNode->getPredecessorsSize();
			for (uint j = 0; j < predecessorSize; ++j)
			{
				uint id = actNode->getPredecessor(j)->getFromId();
				uint comm = actPU == idPuMapping[id] ? 0 : gaoptions->getCommOverhead();
				if (actPU / gaoptions->getPuGroupSize() != idPuMapping[id] / gaoptions->getPuGroupSize())
				{
					comm *= 2;
				}
				if ( DAT[actId] < FT[id] + comm)
				{
					DAT[actId] = FT[id] + comm;
				}
				if (FT[id] == 0)
				{
					DAT[actId] = 10000;
					std::cout<< "Watch out..." << std::endl;
					chromosome->printChromosome(std::cout);
					std::cin.get();
				}
			}

			ST[actId] = max(RT[actPU], DAT[actId]);
			FT[actId] = ST[actId] + gaoptions->getTaskLength();
			RT[actPU] = FT[actId];
		}
		uint length = 0;
		for (uint i = 0; i < tasks; ++i)
		{
			if (FT[i] > length)
			{
				length = FT[i];
			}
		}

		chromosome->fitness = length;
		return length;
	}

	///
	// crossover operation between the mapping part of chromosome
	// creates a brand new chromosome
	///
	shared_ptr<Chromosome> GeneticAlgorithm::crossover_map(cPtr father, cPtr mother)
	{
		cPtr offspring(new Chromosome(*father)); 
		uint crossoverPoint = rand() % father->mapping.size();
		std::copy(mother->mapping.begin() + crossoverPoint, mother->mapping.end(), offspring->mapping.begin() + crossoverPoint);

		return offspring;
	}

	///
	// crossover operation between the scheduling part of chromosome
	///
	shared_ptr<Chromosome> GeneticAlgorithm::crossover_order(cPtr father, cPtr mother)
	{
		
		cPtr offsrping(new Chromosome(*father));
		uint crossoverPoint = rand() % this->levlingLimits.size();

		std::copy(mother->scheduling.begin() + this->levlingLimits[crossoverPoint], mother->scheduling.end(), offsrping->scheduling.begin() + this->levlingLimits[crossoverPoint]);

		return offsrping;
	}

	///
	// randomly changes one mapping to another pu
	///
	void GeneticAlgorithm::mutate(cPtr offspring)
	{
		for (uint i = 0; i < gaoptions->getMutationRate(); ++i)
		{
			unsigned rate = rand() % 100;
			if (rate < gaoptions->getMutationPercentage())
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

	void GeneticAlgorithm::mutateSheduling(cPtr offspring)
	{
		//for (uint i = 0; i < gaoptions->getMutationRate(); ++i)
		{
			unsigned rate = rand() % 100;
			if (rate < gaoptions->getMutationPercentage())
			{

				int position = rand() % levlingLimits.size();
				IProcessable::nVect::iterator begin = offspring->scheduling.begin() + levlingLimits[position];
				IProcessable::nVect::iterator end = position == levlingLimits.size() - 1 ? offspring->scheduling.end() : offspring->scheduling.begin() + levlingLimits[position+1];
				std::random_shuffle(begin, end);
			}
		}
	}

	///
	// generates offsprings
	///
	void GeneticAlgorithm::simulateMating(shared_ptr<Population> population, int offsprings)
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
				offspring = crossover_map(father, mother);
			}
			else
			{
				offspring = crossover_order(father, mother);
			}

			mutate(offspring);
			mutateSheduling(offspring);
			fitness(offspring);
			population->addOffspring(offspring);
		}
	}
}
#pragma once	

#include "Options.h"
#include "Graph.h"
#include <ostream>
#include <vector>
#include <memory>

/// This class represents a solution
/// a solution is described with
/// a mapping and a scheduling part

namespace msonlab
{
	class Chromosome : public std::enable_shared_from_this<Chromosome>
	{
	private:
		// the scheduling part, escribes the order of the tasks
		IProcessable::nVect scheduling;

		// the mapping part, maps the task to PUs.
		vector<unsigned int> mapping;

		// the number of PUs
		unsigned int pus;

		// the score of this solution, the less is better
		unsigned int fitness;

		// the length of the result
		unsigned int length; 

		friend class chrComparator;
		friend class GraphAlgorithms;
		friend class GeneticAlgorithm;
		friend class GreedySchedulingAlgorithm;
		friend class HusSchedulingAlgorithm;

		void calcStartTime(Options::oPtr options);
	public:
		friend std::ostream& operator<<(std::ostream& os, const Chromosome& chromosome);
		typedef std::shared_ptr<const Chromosome> cPtr;
		typedef vector< cPtr > cVect;

		Chromosome(unsigned pus);
		Chromosome(size_t size, unsigned pus);
		Chromosome(const Chromosome& chromosome);

		Chromosome& operator=(const Chromosome &chromosome);

		unsigned int getFitness() const { return fitness; }
		const vector<unsigned int>& getMapping() const { return mapping; }
		const IProcessable::nVect& getScheduling() const { return scheduling; }

		void printChromosome(std::ostream& o) const;
		void printTable(std::ostream& o, unsigned commOverhead) const;
	};
}
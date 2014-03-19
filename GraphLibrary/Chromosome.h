#pragma once	

#include "IProcessable.h"
#include "GraphAlgorithms.h"
#include "Global.h"
#include <ostream>

/// This class represents a solution
/// a solution is described with
/// a mapping and a scheduling part

namespace msonlab
{
	class Chromosome
	{
	private:
		/// the scheduling part
		/// this part describes the order of the tasks
		IProcessable::nVect scheduling;

		/// the mapping part
		/// this part maps the task to PUs.
		vector<unsigned int> mapping;

		/// the number of PUs
		unsigned int pus;

		/// the score of this solution
		/// the less is better
		unsigned int fitness;

		friend class chrComparator;
		friend class GeneticAlgorithm;
	public:
		typedef std::shared_ptr<Chromosome> cPtr;
		typedef vector< cPtr > cVect;

		Chromosome();
		Chromosome(size_t size);
		Chromosome(Chromosome& chromosome);

		Chromosome& operator=(const Chromosome &chromosome);

		unsigned int getFitness() { return fitness; }
		unsigned int getPUs() { return pus; }
		vector<unsigned int> getMapping() { return mapping; }
		IProcessable::nVect getScheduling() { return scheduling; }

		void printChromosome(std::ostream& o);
	};
}
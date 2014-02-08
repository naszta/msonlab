#pragma once	

#include "IProcessable.h"
#include "GraphAlgorithms.h"
#include "Global.h"
#include <ostream>

/*********************
 * This class represents a genome
 * 
 **********************/

namespace msonlab
{
	class Chromosome
	{
	private:
		IProcessable::nVect scheduling;
		vector<unsigned int> mapping;
		unsigned int pus;
		unsigned int fitness;

		friend class chrComparator;
		friend class GeneticAlgorithm;
	public:
		typedef boost::shared_ptr<Chromosome> cPtr;
		typedef vector< cPtr > cVect;

		Chromosome();
		Chromosome(size_t size);

		unsigned int getFitness() { return fitness; }
		unsigned int getPUs() { return pus; }
		vector<unsigned int> getMapping() { return mapping; }
		IProcessable::nVect getScheduling() { return scheduling; }

		void printChromosome(std::ostream& o);
	};
}
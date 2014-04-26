#pragma once	

#include "IProcessable.h"
#include "GraphAlgorithms.h"
#include "Global.h"
#include "GAOptions.h"
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

		void calcStartTime(GAOptions::gaPtr options);
	public:
		friend std::ostream& operator<<(std::ostream& os, const Chromosome& chromosome);
		typedef std::shared_ptr<Chromosome> cPtr;
		typedef vector< cPtr > cVect;

		Chromosome();
		Chromosome(size_t size);
		Chromosome(const Chromosome& chromosome);

		Chromosome& operator=(const Chromosome &chromosome);

		unsigned int getFitness() const { return fitness; }
		unsigned int getPUs() const { return pus; }
		const vector<unsigned int>& getMapping() const { return mapping; }
		const IProcessable::nVect& getScheduling() const { return scheduling; }

		void printChromosome(std::ostream& o) const;
		void printTable(std::ostream& o, unsigned taskLength, unsigned commOverhead) const;
	};
}
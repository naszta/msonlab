#include "Chromosome.h"

namespace msonlab
{

	Chromosome::Chromosome(size_t size) : fitness(0)
	{
		scheduling.resize(size);
		mapping.resize(size);
	}

	void Chromosome::printChromosome(std::ostream& o)
	{
		for (size_t i = 0; i < mapping.size(); ++i)
		{
			o << mapping[i] << " ";
		}

		o << " | ";

		for (size_t i = 0; i < scheduling.size(); ++i)
		{
			o << scheduling[i]->getId() << " ";
		}

		o << "| length = " << fitness << std::endl;
	}
}
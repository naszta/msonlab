#include "Chromosome.h"

namespace msonlab
{
	/// Initialises a new instance of the Chromosome class.
	/// The size is the number of tasks.
	Chromosome::Chromosome(size_t size) : fitness(0)
	{
		scheduling.resize(size);
		mapping.resize(size);
	}

	/// prints this Chromosome's instance to the
	/// given output stream.
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
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

	/// Copy constructor.
	/// Sets the fitness to zero.
	Chromosome::Chromosome(Chromosome& chromosome)
	{
		*this = chromosome;
		this->fitness = 0;
	}

	/// operator =
	/// It DOES copy the fitness.
	Chromosome& Chromosome::operator=(const Chromosome &chromosome)
	{
		if (this != &chromosome)
		{
			this->mapping = chromosome.mapping;
			this->scheduling = chromosome.scheduling;
			this->fitness = chromosome.fitness;
			this->pus = chromosome.pus;
		}

		return *this;
	}

	/// prints this Chromosome's instance to the
	/// given output stream
	void Chromosome::printChromosome(std::ostream& o) const
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

	std::ostream& operator<<(std::ostream& os, const Chromosome& chromosome)
	{
		chromosome.printChromosome(os);
		return os;
	}
}
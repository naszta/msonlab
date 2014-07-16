#include "Chromosome.h"
#include "GraphAlgorithms.h"

namespace msonlab
{
	/// Initialises a new instance of the Chromosome class.
	/// The size is the number of tasks.
	/*Chromosome::Chromosome(unsigned pus, unsigned edges) : fitness(0), pus(pus), edges(edges)
	{
	}*/

	/// Initialises a new instance of the Chromosome class.
	/// The size is the number of tasks.
	Chromosome::Chromosome(size_t size, unsigned pus, unsigned edges) : fitness(0), pus(pus), edges(edges)
	{
		scheduling.resize(size);
		mapping.resize(size);
	}

	/// Copy constructor.
	/// Sets the fitness to zero.
	Chromosome::Chromosome(const Chromosome& chromosome)
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
			this->edges = chromosome.edges;
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

	void Chromosome::printTable(std::ostream& os, Options::oPtr options) const
	{
		auto tasks = scheduling.size();

		vector<unsigned> ST(tasks);
		unsigned length = GraphAlgorithms::computeLengthAndST(shared_from_this(), options, ST);

		// initialize table
		//unsigned length = *std::max_element(FT.begin(), FT.end());
		vector<vector<int>> table(length);
		for (unsigned i = 0; i < table.size(); ++i)
		{
			table[i].resize(this->pus, -1);

		}
		for (unsigned i = 0; i < this->scheduling.size(); ++i)
		{
			unsigned pu = this->mapping[i];
			unsigned task = this->scheduling[i]->getId();
			unsigned start = ST[task];
			for (unsigned j = 0; j < this->scheduling[i]->getComputationTime(); ++j)
			{
				table[start + j][pu] = task;
			}
		}

		//os.width(3);
		os << "Print table length: " << length << std::endl;
		for (unsigned i = 0; i < table.size(); ++i)
		{
			os.width(2);
			os << i << ": ";
			for (unsigned j = 0; j < table[i].size(); ++j)
			{
				if (table[i][j] >= 0) {
					os.width(3);
					os << table[i][j];
				}
				else
					os << "   ";
				os << " | ";
			}

			os << std::endl;
		}
	}
}
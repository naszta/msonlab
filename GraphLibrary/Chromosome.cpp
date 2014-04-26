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
		chromosome.printTable(os, 3, 1);
		return os;
	}

	void Chromosome::printTable(std::ostream& os, unsigned taskLength, unsigned commOverhead) const
	{
		unsigned taskLength = 3;// options->getTaskLength();
		unsigned commOverhead = 1;// options->getCommOverhead();
		unsigned puGroupSize = 4;
		unsigned tasks = scheduling.size();
		vector<unsigned> ST(tasks);
		vector<unsigned> RT(this->getPUs()); // ready time of the PUs
		vector<unsigned> FT(tasks); // finish time of the tasks
		vector<unsigned> DAT(tasks); // Data Arrival Time
		vector<unsigned> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j] // is this necessary?

		unsigned actId = scheduling[0]->getId();
		ST[actId] = 0;
		FT[actId] = ST[actId] + taskLength; // task length, TODO: create a distribution
		DAT[actId] = 0;
		idPuMapping[actId] = mapping[0];
		RT[mapping[0]] = FT[actId];

		// skipping first task
		for (unsigned i = 1; i < tasks; ++i)
		{
			IProcessable::nPtr actNode = scheduling[i];
			unsigned actId = actNode->getId();
			unsigned actPU = mapping[i];
			idPuMapping[actId] = actPU;

			// calculating data arrival time
			size_t predecessorSize = actNode->getPredecessorsSize();
			for (unsigned j = 0; j < predecessorSize; ++j)
			{
				unsigned id = actNode->getPredecessor(j)->getFromId();
				unsigned comm = actPU == idPuMapping[id] ? 0 : commOverhead;
				if (actPU / puGroupSize != idPuMapping[id] / puGroupSize)
				{
					comm *= 2;
				}

				if (DAT[actId] < FT[id] + comm)
				{
					DAT[actId] = FT[id] + comm;
				}
			}

			ST[actId] = RT[actPU] > DAT[actId] ? RT[actPU] : DAT[actId];
			FT[actId] = ST[actId] + taskLength;
			RT[actPU] = FT[actId];

			std::cout << "ST[" << actId << "] = " << ST[actId] << std::endl;
			std::cout << "FT[" << actId << "] = " << FT[actId] << std::endl;
			std::cout << "DAT[" << actId << "] = " << DAT[actId] << std::endl;
			std::cout << "RT[" << actPU << "] = " << RT[actPU] << std::endl;
		}

		// initialize table
		vector<vector<unsigned>> table(this->fitness);
		for (unsigned i = 0; i < table.size(); ++i)
		{
			table[i].resize(this->getPUs());
		}

		for (unsigned i = 0; i < this->scheduling.size(); ++i)
		{
			unsigned pu = this->mapping[i];
			unsigned task = this->scheduling[i]->getId();
			unsigned start = ST[task];
			for (unsigned j = 0; j < taskLength; ++j)
			{
				table[start + j][pu] = i;
			}
		}

		//os.width(3);
		for (unsigned i = 0; i < table.size(); ++i)
		{
			for (unsigned j = 0; j < table[i].size(); ++j)
			{
				if (table[i][j] > 0) {
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
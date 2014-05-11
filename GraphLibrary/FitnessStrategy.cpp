#include "FitnessStrategy.h"
#include <algorithm>

namespace msonlab
{

	LengthFitnessStartegy::LengthFitnessStartegy() : punishCommunication(false) {}

	LengthFitnessStartegy::LengthFitnessStartegy(bool punishCommunication) : punishCommunication(punishCommunication) {}

	unsigned int LengthFitnessStartegy::fitness(Chromosome::cPtr chromosome, Options::oPtr options)
	{
		typedef unsigned int uint;
		unsigned communication = 0;
		//unsigned taskLength = options->getTaskLength();
		unsigned commOverhead = options->getCommOverhead();
		unsigned puGroupSize = options->getPuGroupSize();
		auto mapping = chromosome->getMapping();
		auto scheduling = chromosome->getScheduling();
		auto tasks = scheduling.size();
		vector<uint> RT(chromosome->getPUs()); // ready time of the PUs
		vector<uint> ST(tasks); // start time of the tasks
		vector<uint> FT(tasks); // finish time of the tasks
		vector<uint> DAT(tasks); // Data Arrival Time
		vector<uint> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j]

		uint actId = scheduling[0]->getId();
		ST[actId] = 0;
		FT[actId] = ST[actId] + scheduling[0]->getComputationTime(); // task length, TODO: create a distribution
		DAT[actId] = 0;
		idPuMapping[actId] = mapping[0];
		RT[mapping[0]] = FT[actId];

		// skipping first task
		for (uint i = 1; i < tasks; ++i)
		{
			IProcessable::nPtr actNode = scheduling[i];
			uint actId = actNode->getId();
			uint actPU = mapping[i];
			idPuMapping[actId] = actPU;

			// calculating data arrival time
			size_t predecessorSize = actNode->getPredecessorsSize();
			for (uint j = 0; j < predecessorSize; ++j)
			{
				uint id = actNode->getPredecessor(j)->getFromId();
				uint comm = 0;
				if (actPU != idPuMapping[id]) {
					comm = commOverhead;
					++communication;
				}

				if (actPU / puGroupSize != idPuMapping[id] / puGroupSize)
				{
					comm *= 2;
				}
				if (DAT[actId] < FT[id] + comm)
				{
					DAT[actId] = FT[id] + comm;
				}

				// check whether a flaw is present in this solution
				if (FT[id] == 0)
				{
					// TODO: remove, when ensured, cannot happen
					std::cout << "Flawed chromosome" << std::endl;
					chromosome->printChromosome(std::cout);
					std::cin.get();
					return UINT32_MAX;
				}
			}

			ST[actId] = max(RT[actPU], DAT[actId]);
			FT[actId] = ST[actId] + actNode->getComputationTime();
			RT[actPU] = FT[actId];
		}

		uint length = *std::max_element(FT.begin(), FT.end());
		if (this->punishCommunication) {
			length += communication*commOverhead;
		}

		return length;
	}

	unsigned int LeastCutFitnessStrategy::fitness(Chromosome::cPtr chromosome, Options::oPtr options)
	{
		unsigned length = LengthFitnessStartegy::fitness(chromosome, options);
		// count cuts
		return 0;
	}

	unsigned int OpenEdgesFitnessStrategy::fitness(Chromosome::cPtr chromosome, Options::oPtr options)
	{
		return 0;
	}

}
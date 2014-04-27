#include "FitnessStrategy.h"

namespace msonlab
{
	unsigned int LengthFitnessStartegy::fitness(Chromosome::cPtr chromosome, GAOptions::gaPtr options)
	{
		typedef unsigned int uint;
		unsigned taskLength = options->getTaskLength();
		unsigned commOverhead = options->getCommOverhead();
		unsigned puGroupSize = 4;
		const vector<unsigned int>& mapping = chromosome->getMapping();
		const IProcessable::nVect& scheduling = chromosome->getScheduling();
		uint tasks = scheduling.size();
		vector<uint> RT(chromosome->getPUs()); // ready time of the PUs
		vector<uint> ST(tasks); // start time of the tasks
		vector<uint> FT(tasks); // finish time of the tasks
		vector<uint> DAT(tasks); // Data Arrival Time
		vector<uint> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j] // is this necessary?

		uint actId = scheduling[0]->getId();
		ST[actId] = 0;
		FT[actId] = ST[actId] + taskLength; // task length, TODO: create a distribution
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
				uint comm = actPU == idPuMapping[id] ? 0 : commOverhead;
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
					DAT[actId] = 10000;
					std::cout << "Flawed chromosome" << std::endl;
					chromosome->printChromosome(std::cout);
					std::cin.get();
					return UINT32_MAX;
				}
			}

			ST[actId] = max(RT[actPU], DAT[actId]);
			FT[actId] = ST[actId] + taskLength;
			RT[actPU] = FT[actId];
		}

		uint length = 0;
		for (uint i = 0; i < tasks; ++i)
		{
			if (FT[i] > length)
			{
				length = FT[i];
			}
		}

		return length;
	}

	void LengthFitnessStartegy::communication(unsigned nodeFromId, unsigned nodeToId) {}
	void LengthFitnessStartegy::startTimeSet(unsigned nodeId, unsigned time){}

	unsigned int LeastCutFitnessStrategy::fitness(Chromosome::cPtr chromosome, GAOptions::gaPtr options)
	{
		unsigned length = LengthFitnessStartegy::fitness(chromosome, options);
		// count cuts
		return 0;
	}

	void LeastCutFitnessStrategy::communication(unsigned nodeFromId, unsigned nodeToId) {}
	void LeastCutFitnessStrategy::startTimeSet(unsigned nodeId, unsigned time){}

	unsigned int OpenEdgesFitnessStrategy::fitness(Chromosome::cPtr chromosome, GAOptions::gaPtr options)
	{
		return 0;
	}

	void OpenEdgesFitnessStrategy::communication(unsigned nodeFromId, unsigned nodeToId) {}
	void OpenEdgesFitnessStrategy::startTimeSet(unsigned nodeId, unsigned time){}
}
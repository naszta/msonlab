#pragma once

#include <fstream>
#include "ConfigFile.h"

namespace msonlab
{
	class GAOptions
	{
		typedef unsigned int uint;
		// algorithm parameters
		uint mutationPercentage;
		uint mutationRate;
		uint popMaxSize;
		uint keepSize;
		uint keepBest;
		uint numberOfYears;

		uint taskLength;
		uint commOverhead;
		uint numberOfPus;
		uint puGroupSize;

		uint graphSize;
		uint graphEdgeProb;
		uint graphWidening;
		
	public:

		GAOptions();
		GAOptions(char * configFilePath);

		unsigned getMutationPercentage() { return mutationPercentage; }
		unsigned getMutationRate() { return mutationRate; }
		unsigned getPopMaxSize() { return popMaxSize; }
		unsigned getKeepSize() { return keepSize; }
		unsigned getKeepBest() { return keepBest; }
		unsigned getNumberOfYears() { return numberOfYears; }

		unsigned getCommOverhead() { return commOverhead; }
		unsigned getTaskLength() { return taskLength; }
		unsigned getNumberOfPus() { return numberOfPus; }
		unsigned getPuGroupSize() { return puGroupSize; }

		unsigned getGraphSize() { return graphSize; }
		unsigned getGraphEdgeProb() { return graphEdgeProb; }
		unsigned getGraphWidening() { return graphWidening; }
	};
}
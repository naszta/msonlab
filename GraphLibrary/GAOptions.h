#pragma once

#include <fstream>
#include <memory>
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
		typedef std::shared_ptr<GAOptions> gaPtr;

		GAOptions(char * configFilePath);

		unsigned getMutationPercentage() const { return mutationPercentage; }
		unsigned getMutationRate() const { return mutationRate; }
		unsigned getPopMaxSize() const { return popMaxSize; }
		unsigned getKeepSize() const { return keepSize; }
		unsigned getKeepBest() const { return keepBest; }
		unsigned getNumberOfYears() const { return numberOfYears; }

		unsigned getCommOverhead() const { return commOverhead; }
		unsigned getTaskLength() const { return taskLength; }
		unsigned getNumberOfPus() const { return numberOfPus; }
		unsigned getPuGroupSize() const { return puGroupSize; }

		unsigned getGraphSize() const { return graphSize; }
		unsigned getGraphEdgeProb() const { return graphEdgeProb; }
		unsigned getGraphWidening() const { return graphWidening; }
	};
}
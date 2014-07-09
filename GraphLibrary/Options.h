#pragma once

#include <fstream>
#include <memory>
#include <string>
#include "ConfigFile.h"

namespace msonlab
{
	using std::string;

	class Options
	{
		typedef unsigned int uint;
		// algorithm parameters
		uint scheduleMutationRate;
		uint mapMutationRate; // ?
		uint popMaxSize; // the max size of the population
		uint keepSize; // the size of population to be kept
		uint keepBest; //
		uint numberOfYears; // number of rounds in genetic alg.
		double mc; // rate of mutation

		uint taskLength; // length of tasks
		uint commOverhead;
		uint numberOfPus;
		uint puGroupSize;

		uint graphSize;
		uint graphEdgeProb;
		uint graphWidening;

		string algorithm;
		string fitnessStrategy;
	public:
		typedef std::shared_ptr<const Options> oPtr;

		Options(const char * configFilePath);

		unsigned getScheduleMutationRate() const { return scheduleMutationRate; }
		unsigned getMapMutationRate() const { return mapMutationRate; }
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

		string getAlgorithm() const { return algorithm; }
		string getFitnessStrategy() const { return fitnessStrategy; }
	};
}
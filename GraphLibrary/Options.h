#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>

namespace msonlab { namespace scheduling {
	using std::string;

	class Options {
	public:
		Options(const char * configFilePath);

		// for testing purposes.
		Options(const Options& opt, unsigned value);

		unsigned getScheduleMutationRate() const { return scheduleMutationRate; }
		unsigned getMapMutationRate() const { return mapMutationRate; }
		unsigned getPopMaxSize() const { return popMaxSize; }
		unsigned getKeepSize() const { return keepSize; }
		unsigned getKeepBest() const { return keepBest; }
		unsigned getNumberOfYears() const { return numberOfYears; }
		unsigned maxRoundsWithoutImprovement() const { return _max_rounds_wo_improvement; }
		bool isParallel() const { return parallel; }

		unsigned getCommOverhead() const { return commOverhead; }
		unsigned getTaskLength() const { return taskLength; }
		unsigned getNumberOfPus() const { return numberOfPus; }
		unsigned getPuGroupSize() const { return puGroupSize; }

		unsigned getGraphSize() const { return graphSize; }
		unsigned getGraphEdgeProb() const { return graphEdgeProb; }
		unsigned getGraphWidening() const { return graphWidening; }

		string getAlgorithm() const { return algorithm; }
		string getFitnessStrategy() const { return fitnessStrategy; }
		string getInitialSolution() const { return initialSolution; }
	private:
		typedef unsigned int uint;
		// algorithm parameters
		uint scheduleMutationRate;
		uint mapMutationRate; // ?
		uint popMaxSize; // the max size of the population
		uint keepSize; // the size of population to be kept
		uint keepBest; //
		uint numberOfYears; // number of rounds in genetic alg.
		uint _max_rounds_wo_improvement; // add random solutions after X rounds without improvement
		double mc; // rate of mutation
		bool parallel;

		uint taskLength; // length of tasks
		uint commOverhead;
		uint numberOfPus;
		uint puGroupSize;

		uint graphSize;
		uint graphEdgeProb;
		uint graphWidening;

		string initialSolution;
		string algorithm;
		string fitnessStrategy;
	};
}}
#endif
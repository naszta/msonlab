#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>

namespace msonlab { namespace scheduling {
	using std::string;

	class Options {
	public:
		Options(const char * configFilePath);

		// for testing purposes.
		Options(const Options& opt);

		unsigned scheduleMutationRate()	const { return _scheduleMutationRate; }
		void scheduleMutationRate(unsigned scheduleMutationRate_) { _scheduleMutationRate = scheduleMutationRate_; }
		unsigned mapMutationRate()	const { return _mapMutationRate; }
		void mapMutationRate(unsigned mapMutationRate_) { _mapMutationRate = mapMutationRate_; }
		unsigned popMaxSize() const { return _popMaxSize; }
		void popMaxSize(unsigned popMaxSize_) { _popMaxSize = popMaxSize_; }
		unsigned keepSize()	const { return _keepSize; }
		void keepSize(unsigned keepSize_) { _keepSize = keepSize_; }
		unsigned keepBest()	const { return _keepBest; }
		void keepBest(unsigned keepBest_) { _keepBest = keepBest_; }
		unsigned numberOfYears() const { return _numberOfYears; }
		void numberOfYears(unsigned numberOfYears_) { _numberOfYears = numberOfYears_; }
		unsigned maxRoundsWithoutImprovement()	const { return _max_rounds_wo_improvement; }
		void maxRoundsWithoutImprovement(unsigned mrwi_) { _max_rounds_wo_improvement = mrwi_; }
		bool isParallel() const { return _parallel; }
		void isParallel(bool parallel_)	{ _parallel = parallel_; }

		unsigned commOverhead() const { return _commOverhead; }
		void commOverhead(unsigned commOverhead_) { _commOverhead = commOverhead_; }
		unsigned numberOfPus() const { return _numberOfPus; }
		void numberOfPus(unsigned numberOfPus_) { _numberOfPus = numberOfPus_; }
		unsigned puGroupSize() const { return _puGroupSize; }
		void puGroupSize(unsigned puGroupSize_) { _puGroupSize = puGroupSize_; }

		unsigned graphSize() const { return _graphSize; }
		void graphSize(unsigned graphSize_) { _graphSize = graphSize_; }
		unsigned graphEdgeProb() const { return _graphEdgeProb; }
		void graphEdgeProb(unsigned graphEdgeProb_) { _graphEdgeProb = graphEdgeProb_; }
		unsigned graphWidening() const { return _graphWidening; }
		void graphWidening(unsigned graphWidening_) { _graphWidening = graphWidening_; }

		const string& algorithm() const { return _algorithm; }
		string& algorithm() { return _algorithm; }
		void algorithm(string algorithm_) { _algorithm = algorithm_; }

		const string& fitnessStrategy() const { return _fitnessStrategy; }
		string& fitnessStrategy() { return _fitnessStrategy; }
		void fitnessStrategy(string fitnessStrategy_) { _fitnessStrategy = fitnessStrategy_; }
		
		const string& initialSolution() const { return _initialSolution; }
		string& initialSolution() { return _initialSolution; }
		void initialSolution(string initialSolution) { _initialSolution = initialSolution; }
	private:
		typedef unsigned int uint;
		// algorithm parameters
		uint _scheduleMutationRate;
		uint _mapMutationRate; // ?
		uint _popMaxSize; // the max size of the population
		uint _keepSize; // the size of population to be kept
		uint _keepBest; //
		uint _numberOfYears; // number of rounds in genetic alg.
		uint _max_rounds_wo_improvement; // add random solutions after X rounds without improvement
		double _mc; // rate of mutation
		bool _parallel;

		uint _commOverhead;
		uint _numberOfPus;
		uint _puGroupSize;

		uint _graphSize;
		uint _graphEdgeProb;
		uint _graphWidening;

		string _initialSolution;
		string _algorithm;
		string _fitnessStrategy;
	};
}}
#endif
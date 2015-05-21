#include "Options.h"
#include "ConfigFile.h"

#include <fstream>

namespace msonlab { namespace scheduling {
	Options::Options(const char * configFilePath) {
		ConfigFile config = ConfigFile(configFilePath);
		_scheduleMutationRate		= config.getValueOfKey<unsigned>("scheduleMutationRate", 20);
		_mapMutationRate			= config.getValueOfKey<unsigned>("mapMutationRate", 80);
		_commOverhead				= config.getValueOfKey<unsigned>("commOverhead", 3);
		_popMaxSize					= config.getValueOfKey<unsigned>("popMaxSize", 100);
		_keepSize					= config.getValueOfKey<unsigned>("keepSize", 200);
		_keepBest					= config.getValueOfKey<unsigned>("keepBest", 0);
		_numberOfPus				= config.getValueOfKey<unsigned>("numberOfPus", 2);
		_puGroupSize				= config.getValueOfKey<unsigned>("puGroupSize", 4);
		_graphSize					= config.getValueOfKey<unsigned>("graphSize", 100);
		_graphEdgeProb				= config.getValueOfKey<unsigned>("graphEdgeProb", 25);
		_graphWidening				= config.getValueOfKey<unsigned>("graphWidening", 20);
		_numberOfYears				= config.getValueOfKey<unsigned>("numberOfYears", 100);
		_max_rounds_wo_improvement	= config.getValueOfKey<unsigned>("maxRoundsWithoutImprovement", _numberOfYears / 10);
		_parallel					= config.getValueOfKey<bool>("parallel", false);
		_algorithm					= config.getValueOfKey<string>("algorithm", "greedy");
		_fitnessStrategy			= config.getValueOfKey<string>("fitnessStrategy", "length");
		_initialSolution			= config.getValueOfKey<string>("initialSolution", "rnd");
	}

	Options::Options(const Options& opt)
	{
		_scheduleMutationRate      = opt._scheduleMutationRate;
		_mapMutationRate           = opt._mapMutationRate;
		_commOverhead              = opt._commOverhead;
		_popMaxSize                = opt._popMaxSize;
		_keepSize                  = opt._keepSize;
		_keepBest                  = opt._keepBest;
		_numberOfPus               = opt._numberOfPus;
		_puGroupSize               = opt._puGroupSize;
		_graphSize                 = opt._graphSize;
		_graphEdgeProb             = opt._graphEdgeProb;
		_graphWidening             = opt._graphWidening;
		_numberOfYears             = opt._numberOfYears;
		_algorithm                 = opt._algorithm;
		_fitnessStrategy           = opt._fitnessStrategy;
		_max_rounds_wo_improvement = opt._max_rounds_wo_improvement;
		_scheduleMutationRate      = opt._scheduleMutationRate;
	}
}}
#include "Options.h"

namespace msonlab
{
	Options::Options(char * configFilePath)
	{
		ConfigFile config = ConfigFile(configFilePath);
		scheduleMutationRate = config.getValueOfKey<unsigned>("scheduleMutationRate");
		mapMutationRate = config.getValueOfKey<unsigned>("mapMutationRate");
		commOverhead = config.getValueOfKey<unsigned>("commOverhead");
		taskLength = config.getValueOfKey<unsigned>("taskLength");
		popMaxSize = config.getValueOfKey<unsigned>("popMaxSize");
		keepSize = config.getValueOfKey<unsigned>("keepSize");
		keepBest = config.getValueOfKey<unsigned>("keepBest");
		numberOfPus = config.getValueOfKey<unsigned>("numberOfPus");
		puGroupSize = config.getValueOfKey<unsigned>("puGroupSize");
		graphSize = config.getValueOfKey<unsigned>("graphSize");
		graphEdgeProb = config.getValueOfKey<unsigned>("graphEdgeProb");
		graphWidening = config.getValueOfKey<unsigned>("graphWidening");
		numberOfYears = config.getValueOfKey<unsigned>("numberOfYears");
		algorithm = config.getValueOfKey<string>("algorithm");
		if (algorithm.length() == 0) {
			algorithm = "greedy";
		}

		fitnessStrategy = config.getValueOfKey<string>("fitnessStrategy");
		if (fitnessStrategy.length() == 0) {
			fitnessStrategy = "length";
		}
	}
}
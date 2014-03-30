#include "GAOptions.h"

namespace msonlab
{
	GAOptions::GAOptions(char * configFilePath)
	{
		ConfigFile config = ConfigFile(configFilePath);
		mutationPercentage = config.getValueOfKey<unsigned>("mutationPercentage");
		mutationRate = config.getValueOfKey<unsigned>("mutationRate");
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
	}
}
#include "Processor.h"

namespace msonlab
{

	//NodeVect Processor::process(GraphPtr graph)
	//{
	//	NodeVect readyToProcess;
	//	NodeVect processed;

	//	// get input nodes
	//	auto inodes = graph->getInputNodes();
	//	auto it = inodes.cbegin();
	//	while (it != inodes.end())
	//	{
	//		readyToProcess.insert(readyToProcess.end(),(*it));
	//		++it;
	//	}

	//	// start processing
	//	auto i = readyToProcess.begin();
	//	while (i != readyToProcess.end())
	//	{
	//		auto newReadyToProcess = (*(*i)).process();
	//		readyToProcess.erase(i);
	//		for (auto j = newReadyToProcess.begin(); j != newReadyToProcess.end(); ++j)
	//		{
	//			readyToProcess.insert(readyToProcess.end(), (*j));
	//		}

	//		i = readyToProcess.begin();
	//	}

	//	return graph->getOutputNodes();
	//}
}
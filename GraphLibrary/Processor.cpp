#pragma once
#include "Processor.h"

namespace msonlab
{

	IProcessable::nVect Processor::process(Graph::gPtr graph)
	{
		IProcessable::pVect readyToProcess;
		IProcessable::pVect processed;

		// get input nodes
		IProcessable::nVect inodes = graph->getInputNodes();
		IProcessable::nVect::const_iterator it = inodes.begin();
		while (it != inodes.end())
		{
			readyToProcess.insert(readyToProcess.end(),(*it));
			++it;
		}

		// start processing
		IProcessable::pVect::iterator i = readyToProcess.begin();
		while (i != readyToProcess.end())
		{
			IProcessable::pVect newReadyToProcess = (*(*i)).process();
			readyToProcess.erase(i);
			for (IProcessable::pVect::iterator j = newReadyToProcess.begin(); j != newReadyToProcess.end(); ++j)
			{
				readyToProcess.insert(readyToProcess.end(), (*j));
			}

			i = readyToProcess.begin();
		}

		return graph->getOutputNodes();;
	}
}
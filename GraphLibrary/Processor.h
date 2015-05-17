#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "Graph/Graph.h"

namespace msonlab
{

	class Processor
	{
	private:

		IProcessableVect readyToProcess;
		IProcessableVect processed;

	public:
		virtual NodeVect process(GraphPtr toProcess);
		
	};
}
#endif
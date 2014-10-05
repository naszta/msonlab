#pragma once
#include "Graph.h"

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
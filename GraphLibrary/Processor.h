#pragma once
#include "Graph.h"

namespace msonlab
{

	class Processor
	{
	private:

		IProcessable::pVect readyToProcess;
		IProcessable::pVect processed;

	public:
		virtual IProcessable::nVect process(Graph::gPtr toProcess);
		
	};


}
#pragma once
#include "Graph.h"

namespace msonlab
{

	class Processor
	{
	private:
		Graph::gPtr graph;

		IProcessable::pVect readyToProcess;
		IProcessable::pVect processed;

	public:
		Processor(Graph::gPtr toProcess);

		virtual IProcessable::nVect process();
		
	};


}
#pragma once
#include "Processor.h"

namespace msonlab
{

	Processor::Processor(Graph::gPtr toProcess)
	{
		graph = toProcess;
	}

	bool Processor::process()
	{




		throw Exceptions::NotImplementedException("Processor::process function");
	}

	IProcessable::nVect Processor::getOutput() const
	{
		return (*graph).getOutputNodes();
	}



}
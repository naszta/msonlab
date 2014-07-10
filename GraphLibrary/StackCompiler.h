#pragma once

#include "Graph.h"

namespace msonlab
{
	class StackCompiler
	{
	public:
		static msonlab::StackRunner::srPtr getStackProgram(Graph::gPtr& graph);
	};

}
#pragma once

#include "Graph.h"

namespace msonlab
{
	class StackCompiler
	{
	private:
		msonlab::Graph::gPtr graph;


	public:

		StackCompiler(msonlab::Graph::gPtr toCompile);

		msonlab::StackRunner::srPtr getStackProgram();

	};

}
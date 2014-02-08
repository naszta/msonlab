#include "StackCompiler.h"
#include "Node.h"


msonlab::StackRunner::srPtr msonlab::StackCompiler::getStackProgram()
{
	msonlab::StackRunner::srPtr ret (new msonlab::StackRunner());

	auto outputs = graph->getOutputNodes();

	// only 1 output is allowed
	if (outputs.size() == 1)
	{
		msonlab::Node::nPtr startNode = outputs[0];

		(*startNode).compile(ret);
	}

	return ret;
}



msonlab::StackCompiler::StackCompiler(msonlab::Graph::gPtr toCompile)
{
	graph = toCompile;
}



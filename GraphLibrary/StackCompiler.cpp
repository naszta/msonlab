#include "StackCompiler.h"
#include "Node.h"


msonlab::StackRunner::srPtr msonlab::StackCompiler::getStackProgram(Graph::gPtr& graph)
{
	msonlab::StackRunner::srPtr ret = std::make_shared<StackRunner>();

	auto outputs = graph->getOutputNodes();

	// only 1 output is allowed
	if (outputs.size() == 1)
	{
		msonlab::Node::nPtr startNode = outputs[0];

		(*startNode).compile(ret);
	}

	return ret;
}

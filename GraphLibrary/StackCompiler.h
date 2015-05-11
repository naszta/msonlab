#ifndef STACK_COMPILER_H
#define STACK_COMPILER_H

#include "Graph.h"

namespace msonlab
{
	class StackCompiler
	{
	private:

		static void mark_node_with_iteration_number(NodePtr to_mark, int iteration_number);
		static void mark_edge_with_iteration_number(EdgePtr to_mark, int iteration_number);


	public:

		static StackRunner::srPtr getStackProgram(const Graph& graph, unsigned int number_of_threads, StackRunner::scheduleOrder schedule);

	};
}
#endif
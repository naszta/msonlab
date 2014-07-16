#pragma once

#include "Graph.h"

namespace msonlab
{
	class StackCompiler
	{
	private:

		static void mark_node_with_iteration_number(Node::nPtr to_mark, int iteration_number);
		static void mark_edge_with_iteration_number(Edge::ePtr to_mark, int iteration_number);


	public:

		static StackRunner::srPtr getStackProgram(msonlab::Graph::gPtr& graph, unsigned int number_of_threads, StackRunner::scheduleOrder schedule);

	};

}
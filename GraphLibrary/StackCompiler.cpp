#include "StackCompiler.h"
#include "Node.h"
#include "StackValue.h"
#include "SimpleStackValue.h"
#include "FutureStackValue.h"
#include "BFSIterator.h"


namespace msonlab
{
	using std::vector;
	void StackCompiler::mark_node_with_iteration_number(NodePtr to_mark, int iteration_number)
	{
		if (to_mark->compile_iteration == -1)
		{
			// first processed in this iteration
			to_mark->compile_iteration = iteration_number;

			for (auto pred : to_mark->getPredecessors())
			{
				if (pred->compile_iteration == -1)
				{
					// first processed in this iteration
					mark_edge_with_iteration_number(pred, iteration_number);
				}
				else
				{
					if (pred->compile_iteration < iteration_number)
					{
						// already processed, just insert "WAIT"
					}
				}
			}
		}

	}

	void StackCompiler::mark_edge_with_iteration_number(EdgePtr to_mark, int iteration_number)
	{
		if (to_mark->compile_iteration == -1)
		{
			// first processed in this iteration
			to_mark->compile_iteration = iteration_number;

			auto pred = to_mark->getFrom();
			if (pred->compile_iteration == -1)
			{
				// first processed in this iteration
				mark_node_with_iteration_number(pred, iteration_number);
			}
			else
			{
				if (pred->compile_iteration < iteration_number)
				{
					// already processed, just insert "WAIT"
				}
			}
		}
	}



	StackRunner::srPtr StackCompiler::getStackProgram(const Graph& graph, unsigned int number_of_threads, StackRunner::scheduleOrder schedule)
	{
		msonlab::StackRunner::srPtr runner(new msonlab::StackRunner());


		// clear sync markers on edges and node
		for (auto curr_node : graph.getNodes())
		{
			curr_node->clear_synced();
			curr_node->clear_sync_marker();
			curr_node->compile_iteration = -1;
		}
		for (auto curr_edge : graph.getEdges())
		{
			curr_edge->clear_synced();
			curr_edge->clear_sync_marker();
			curr_edge->compile_iteration = -1;
		}

		// create threads, programs and stack
		vector<std::thread> threads;
		vector<StackRunner::program> programs;
		vector<StackRunner::dPtrStack> stacks;
		for (size_t i = 0; i < number_of_threads; ++i)
		{
			threads.push_back(std::thread());
			programs.push_back(StackRunner::program());
			stacks.push_back(StackRunner::dPtrStack());
		}

		// create structure for holding futures and promises
		std::set<unsigned int> promises;
		std::set<unsigned int> futures;

		// sync cross-threaded inputs and constants
		auto inputs = graph.getInputNodes();

		for (auto curr_input : inputs)
		{
			unsigned int thread_id = schedule.at(curr_input->getId());

			bool need_to_sync = false;
			for (auto pred : curr_input->getSuccessors())
			{
				if (schedule.at(pred->getId()) != thread_id)
				{
					// input is cross-threaded, needed to be synced
					need_to_sync = true;
					break;
				}
			}

			if (need_to_sync)
			{
				StackRunner::program* prog = &(programs.at(thread_id));

				StackRunner::addToken(prog, StackRunner::PUSH, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(StackValue::stackvaluePtr(new SimpleStackValue(curr_input->getValue())), -1)));
				StackRunner::addToken(prog, StackRunner::SYNC, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, curr_input->getId())));

				curr_input->set_synced();
				curr_input->set_sync_marker();

				promises.insert(curr_input->getId());
				futures.insert(curr_input->getId());
			}
		}


		// create output order - TODO: create order optimalization
	auto outputs = graph.getOutputNodes();
		vector<unsigned int> output_order;
		for (unsigned int i = 0; i < outputs.size(); ++i)
		{
			output_order.push_back(i);
		}

		int iteration = 1;
		for (auto curr_output_i : output_order)
		{
			//mark nodes with the iteration number
			auto curr_output = outputs.at(curr_output_i);

			mark_node_with_iteration_number(curr_output, iteration++);
		}


		// create promises and futures for outputs and save output ids
		vector<unsigned int> output_ids;
		for (auto curr_output : outputs)
		{
			unsigned int curr_id = curr_output->getId();

			promises.insert(curr_id);
			futures.insert(curr_id);

			curr_output->set_sync_marker();

			output_ids.push_back(curr_id);
		}


		// mark nodes and edges that are needed to be synced
		for (auto curr_node : graph.getNodes())
		{
			// mark cross-threaded nodes
			bool cross_threaded = false;
			bool used_for_multiple_outputs = false;
			unsigned int curr_thread = schedule.at(curr_node->getId());

			for (auto succ : curr_node->getSuccessors())
			{
				if (schedule.at(succ->getId()) != curr_thread)
				{
					cross_threaded = true;
					break;
				}
			}
			if (cross_threaded && !curr_node->synced)
			{
				curr_node->set_sync_marker();

				unsigned int curr_id = curr_node->getId();


				promises.insert(curr_id);
				futures.insert(curr_id);
			}

			// mark nodes for sync due to multiple outputs
			for (auto succ : curr_node->getSuccessors())
			{
				if (succ->compile_iteration > curr_node->compile_iteration)
				{
					used_for_multiple_outputs = true;
					break;
				}
			}
			if (used_for_multiple_outputs && !curr_node->synced)
			{
				curr_node->set_sync_marker();

				unsigned int curr_id = curr_node->getId();

				promises.insert(curr_id);
				futures.insert(curr_id);
			}

		}
		for (auto curr_edge : graph.getEdges())
		{
			unsigned int curr_thread = schedule.at(curr_edge->getId());

			if (schedule.at(curr_edge->getToId()) != curr_thread)
	{
				curr_edge->set_sync_marker();

				unsigned int curr_id = curr_edge->getId();

				promises.insert(curr_id);
				futures.insert(curr_id);
	}
			if (curr_edge->getTo()->compile_iteration > curr_edge->compile_iteration)
			{
				curr_edge->set_sync_marker();

				unsigned int curr_id = curr_edge->getId();


				promises.insert(curr_id);
				futures.insert(curr_id);
			}
}




		// compile
		for (auto curr_output_i : output_order)
		{
			auto curr_output = outputs.at(curr_output_i);
			curr_output->compile(-1, &programs, schedule);
		}



		// assign futures and promises to threads




		// build stack runner return object
		runner->programs = programs;
		runner->stacks = stacks;
		runner->promise_candidates = promises;
		runner->future_candidates = futures;
		runner->output_ids = output_ids;

		runner->number_of_threads = number_of_threads;

		return runner;
	}



}

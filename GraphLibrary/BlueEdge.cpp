#pragma once
#include "BlueEdge.h"
#include "Node.h"
#include "GraphExchanger.h"

namespace msonlab
{
	BlueEdge::BlueEdge(unsigned int _id, types::LabelType _label, types::DataPtr _value, IProcessable::nPtr _from, IProcessable::nPtr _to)
		: Edge(_id, _label, _value, _from, _to)
	{
	}

	Edge::EdgeTypeEnum BlueEdge::getEdgeType() const
	{
		return blue_edge;
	}

	// compile

	void BlueEdge::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
	{
		unsigned int thread_id = schedule.at(getId());
		StackRunner::program* prog = &(programs->at(thread_id));

		// if already synced, just push the associated future object to the stack
		if (synced)
		{
			StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, getId())));
			return;
		}

		if (from->compile_iteration < compile_iteration)
		{
			// already has value
			StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, from->getId())));
		}
		else
		{
			// going deeper and calculate predecessor
			from->compile(thread_id, programs, schedule);

			// if predecessor is placed on another thread, add a wait operation
			if (schedule.at(from->getId()) != thread_id)
			{
				// predecessor is on another thread, need to create a FutureStackValue
				StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, from->getId())));
			}
		}

		// manage sync-s
		if (!synced && extra_sync_marker)
		{
			// if need to sync but called from the same thread, DUP is needed
			if (caller_thread != -1)
			{
				if (caller_thread == thread_id)
				{
					StackRunner::addToken(prog, StackRunner::DUP, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, -1)));
				}
			}

			StackRunner::addToken(prog, StackRunner::SYNC, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, getId())));
			set_synced();
		}
	}

	// exchange
	std::string BlueEdge::getTypeString() const
	{
		return GraphExchanger::getSupportedEdgeTypeName(GraphExchanger::supportedEdgeType::BLUE);
	}

	std::string BlueEdge::get_color() const
	{
		return "#0000FF";
	}

}
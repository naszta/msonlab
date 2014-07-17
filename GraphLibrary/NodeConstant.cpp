#pragma once
#include "NodeConstant.h"
#include "Edge.h"
#include "GraphExchanger.h"
#include "SimpleStackValue.h"

namespace msonlab
{
	NodeConstant::NodeConstant(unsigned int _id, types::LabelType _label, types::DataPtr _value)
		: Node(_id, _label, _value)
	{
	}

	IProcessable::pVect NodeConstant::process()
	{
		IProcessable::pVect ret;

		if (isReadyForProcess())
		{
			if (setProcessed(value))
			{
				for (IProcessable::eVect::iterator it = successors.begin(); it != successors.end(); ++it)
				{
					if ((*(*it)).registerParameter())
					{
						ret.insert(ret.begin(), (*it));
					}
				}
				return ret;
			}
			else
				throw msonlab::Exceptions::GeneralErrorException("Error while setting the result of processing on this processable element!");
		}
		else
		{
			throw msonlab::Exceptions::StillNotReadyForProcessException("This processable element is not yet ready for processing!");
		}
	}

	// compile

	void NodeConstant::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
	{
		unsigned int thread_id = schedule.at(getId());
		StackRunner::program* prog = &(programs->at(thread_id));

		// if already synced, just push the associated future object to the stack
		if (synced)
		{
			if (caller_thread == thread_id)
			{
				StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, getId())));
			}
		}
		else
		{
			// add PUSH operation
			StackRunner::addToken(prog, StackRunner::PUSH, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(StackValue::stackvaluePtr(new SimpleStackValue(getValue())), -1)));

			// manage sync-s
			if (extra_sync_marker)
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
	}

	// exchange
	std::string NodeConstant::getTypeString() const
	{
		return GraphExchanger::getSupportedNodeTypeName(GraphExchanger::supportedNodeType::CONSTANT);
	}

	std::string NodeConstant::get_shape() const
	{
		return "ellipse";
	}

	std::string NodeConstant::get_color() const
	{
		return "#CC99FF";
	}

	std::string NodeConstant::get_custom_data() const
	{
		return std::to_string(*value);
	}
}
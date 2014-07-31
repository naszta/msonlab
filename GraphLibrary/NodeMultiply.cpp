#pragma once
#include "NodeMultiply.h"
#include "Edge.h"
#include "GraphExchanger.h"

namespace msonlab
{
	NodeMultiply::NodeMultiply(unsigned int _id, types::LabelType _label, types::DataPtr _value)
		: Node(_id, _label, _value, 5)
	{
		this->type_string = GraphExchanger::getSupportedNodeTypeName(GraphExchanger::supportedNodeType::MULTIPLY);
	}

	NodeMultiply::NodeMultiply(const NodeMultiply& other) : Node(other)
	{
		if (this != &other)
		{
			*this = other;
		}
	}

	// does NOT copy connections
	NodeMultiply& NodeMultiply::operator=(const NodeMultiply& other)
	{
		Node::operator=(other);
		return *this;
	}

	Node::nPtr NodeMultiply::clone() {
		return std::make_shared<NodeMultiply>(*this);
	}

	IProcessable::pVect NodeMultiply::process()
	{
		IProcessable::pVect ret;

		if (isReadyForProcess())
		{
			types::DataPtr newVal = std::make_shared<types::DataType>(1.0);

			for (IProcessable::eVect::iterator it = predecessors.begin(); it != predecessors.end(); ++it)
			{
				*newVal *= *(*(*it)).getResultValue();
			}

			if (setProcessed(newVal))
			{
				for (IProcessable::eVect::iterator it = successors.begin(); it != successors.end(); ++it)
				{
					if((*(*it)).registerParameter())
					{
						ret.insert(ret.begin(),(*it));
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

		return ret;
	}

	// compile
	void NodeMultiply::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
	{
		unsigned int thread_id = schedule.at(getId());
		StackRunner::program* prog = &(programs->at(thread_id));

		// if already synced, just push the associated future object to the stack
		if (synced)
		{
			StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, getId())));
			return;
		}


		// going deeper and calculate predecessors
		for (IProcessable::pPtr pred : predecessors)
		{
			if (pred->compile_iteration < compile_iteration)
			{
				// already has value
				StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, pred->getId())));
			}
			else
			{
				pred->compile(thread_id, programs, schedule);

				// if predecessor is placed on another thread, add a wait operation
				if (schedule.at(pred->getId()) != thread_id)
				{
					// predecessor is on another thread, need to create a FutureStackValue
					StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, pred->getId())));
				}
			}
		}

		// add MUL operations
		for (unsigned int i = 0; i < predecessors.size() - 1; ++i)
		{
			StackRunner::addToken(prog, StackRunner::MUL, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, -1)));
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
	/*std::string NodeMultiply::getTypeString() const
		{
		return GraphExchanger::getSupportedNodeTypeName(GraphExchanger::supportedNodeType::MULTIPLY);
		}*/

	std::string NodeMultiply::get_color() const
	{
		return "#FFFF00";
	}
}
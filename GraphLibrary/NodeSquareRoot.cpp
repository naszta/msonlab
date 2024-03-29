#pragma once
#include "NodeSquareRoot.h"
#include "Edge.h"
#include <math.h>
#include "GraphExchanger.h"
#include "SimpleStackValue.h"

namespace msonlab
{
	NodeSquareRoot::NodeSquareRoot(unsigned int _id, Types::LabelType _label, Types::DataPtr _value)
		: Node(_id, _label, _value)
	{
	}

	IProcessable::pVect NodeSquareRoot::process()
	{
		IProcessable::pVect ret;

		Types::DataPtr newVal = std::make_shared<msonlab::Types::DataType>(0.0);


		if (isReadyForProcess())
		{
			if (predecessors.size() != 1)
				throw Exceptions::NodeSquareRootPredecessorsAreNotValidException("NodeSquareRoot::process function");

			*newVal = sqrt(*(predecessors.at(0)->getResultValue()));


			if (setProcessed(newVal))
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

		return ret;
	}

	// compile
	void NodeSquareRoot::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
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
		if (predecessors.size() != 1) throw new Exceptions::GeneralErrorException("Square root node is needed to have one predecessor");

		IProcessable::pPtr pred = predecessors.at(0);

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

		// add SQRT operation
		Types::DataPtr data = std::make_shared<Types::DataType>(0.5);
		StackValue::stackvaluePtr stackValue = std::make_shared<SimpleStackValue>(data);
		StackRunner::addToken(prog, StackRunner::PUSH, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(stackValue, -1)));
		StackRunner::addToken(prog, StackRunner::POW, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, -1)));


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
	std::string NodeSquareRoot::getTypeString() const
	{
		return GraphExchanger::getSupportedNodeTypeName(GraphExchanger::supportedNodeType::SQUAREROOT);
	}

	std::string NodeSquareRoot::get_color() const
	{
		return "#00CCFF";
	}

}
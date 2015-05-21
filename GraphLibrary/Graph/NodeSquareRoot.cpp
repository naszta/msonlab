#include "NodeSquareRoot.h"

#include <math.h>

#include "Edge.h"
#include "../Executor/SimpleStackValue.h"

namespace msonlab
{
	NodeSquareRoot::NodeSquareRoot(unsigned int _id, types::LabelType _label, types::DataPtr _value)
		: Node(_id, _label, _value, NodeDescriptor("SQUAREROOT", "#00CCFF"), 7)
	{
	}

	NodeSquareRoot::NodeSquareRoot(const NodeSquareRoot& other) : Node(other)
	{
		if (this != &other)
		{
			*this = other;
		}
	}

	// does NOT copy connections
	NodeSquareRoot& NodeSquareRoot::operator=(const NodeSquareRoot& other)
	{
		Node::operator=(other);
		return *this;
	}

	NodePtr NodeSquareRoot::clone() const
	{
		return std::make_shared<NodeSquareRoot>(*this);
	}

	IProcessableVect NodeSquareRoot::process()
	{
		IProcessableVect ret;

		types::DataPtr newVal = std::make_shared<msonlab::types::DataType>(0.0);


		if (isReadyForProcess())
		{
			if (getPredecessorsSize() != 1)
				throw Exceptions::NodeSquareRootPredecessorsAreNotValidException("NodeSquareRoot::process function");

			*newVal = sqrt(*(getPredecessor(0)->getResultValue()));


			if (setProcessed(newVal))
			{
				for (auto edge : getSuccessors())
				{
					if (edge->registerParameter())
					{
						ret.insert(ret.begin(), edge);
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
	void NodeSquareRoot::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
	{
		unsigned int thread_id = schedule.at(id());
		StackRunner::program* prog = &(programs->at(thread_id));

		// if already synced, just push the associated future object to the stack
		if (synced)
		{
			StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, id())));
			return;
		}


		// going deeper and calculate predecessors
		if (getPredecessorsSize() != 1) throw new Exceptions::GeneralErrorException("Square root node is needed to have one predecessor");

		auto pred = getPredecessor(0);

		if (pred->compile_iteration < compile_iteration)
		{
			// already has value
			StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, pred->id())));
		}
		else
		{
			pred->compile(thread_id, programs, schedule);

			// if predecessor is placed on another thread, add a wait operation
			if (schedule.at(pred->id()) != thread_id)
			{
				// predecessor is on another thread, need to create a FutureStackValue
				StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, pred->id())));
			}
		}

		// add SQRT operation
		types::DataPtr data = std::make_shared<types::DataType>(0.5);
		StackValue::stackvaluePtr stackValue = std::make_shared<SimpleStackValue>(data);
		StackRunner::addToken(prog, StackRunner::PUSH, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(stackValue, -1)));
		StackRunner::addToken(prog, StackRunner::POW, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, -1)));


		// manage sync-s
		if (!synced && extra_sync_marker)
		{
			// if need to sync but called from the same thread, DUP is needed
			if (caller_thread != -1)
			{
				if (static_cast<unsigned>(caller_thread) == thread_id)
				{
					StackRunner::addToken(prog, StackRunner::DUP, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, -1)));
				}
			}

			StackRunner::addToken(prog, StackRunner::SYNC, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, id())));
			set_synced();
		}

	}
}
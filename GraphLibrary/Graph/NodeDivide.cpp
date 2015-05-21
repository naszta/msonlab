#include "NodeDivide.h"
#include "Edge.h"
#include "PersistenceUtils.h"

namespace msonlab
{
	NodeDivide::NodeDivide(unsigned int _id, types::LabelType _label, types::DataPtr _value)
		: Node(_id, _label, _value, NodeDescriptor("DIVIDE", "#FF9900"), 5)
	{
	}

	NodeDivide::NodeDivide(const NodeDivide& other) : Node(other)
	{
		if (this != &other)
		{
			*this = other;
		}
	}

	// does NOT copy connections
	NodeDivide& NodeDivide::operator=(const NodeDivide& other)
	{
		Node::operator=(other);
		return *this;
	}

	NodePtr NodeDivide::clone() const {
		return std::make_shared<NodeDivide>(*this);
	}

	IProcessableVect NodeDivide::process()
	{
		IProcessableVect ret;

		if (isReadyForProcess())
		{
			types::DataPtr newVal = std::make_shared<types::DataType>(1.0);

			int i = 1;

			for (auto edge : getPredecessors())
			{
				if (i == 1)
					*newVal = *(edge->getResultValue());
				else
					*newVal /= *(edge->getResultValue());

				i++;
			}

			if (setProcessed(newVal))
			{
				for (auto edge : getSuccessors())
				{
					if(edge->registerParameter())
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

	void NodeDivide::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
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
		int i = 0;
		for (auto pred : getPredecessors())
		{
			if (i == 2) break;

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
			++i;
		}

		// add DIV operation
		StackRunner::addToken(prog, StackRunner::DIV, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, -1)));


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
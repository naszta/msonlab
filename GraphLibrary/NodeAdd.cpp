#include "NodeAdd.h"
#include "Edge.h"
#include "GraphExchanger.h"
#include <memory>

namespace msonlab
{
	NodeAdd::NodeAdd(unsigned int _id, types::LabelType _label, types::DataPtr _value)
		: Node(_id, _label, _value, GraphExchanger::getSupportedNodeTypeName(GraphExchanger::supportedNodeType::ADD), 5)
	{
	}

	NodeAdd::NodeAdd(const NodeAdd& other) : Node(other)
	{
		if (this != &other)
		{
			*this = other;
		}
	}

	// does NOT copy connections
	NodeAdd& NodeAdd::operator=(const NodeAdd& other)
	{
		Node::operator=(other);
		return *this;
	}

	NodePtr NodeAdd::clone() {
		return std::make_shared<NodeAdd>(*this);
	}

	IProcessableVect NodeAdd::process()
	{
		IProcessableVect ret;

		if (isReadyForProcess()) {
			types::DataPtr newVal = std::make_shared<types::DataType>(0.0);

			for (auto edge : getPredecessors()) {
				*newVal += *(edge->getResultValue());
			}

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
			else {
				throw msonlab::Exceptions::GeneralErrorException("Error while setting the result of processing on this processable element!");
			}
		}
		else {
			throw msonlab::Exceptions::StillNotReadyForProcessException("This processable element is not yet ready for processing!");
		}
	}

	// compile

	void NodeAdd::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
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
		for (IProcessablePtr pred : getPredecessors())
		{
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
		}

		// add ADD operations
		size_t count = this->getPredecessorsSize();
		for (unsigned int i = 0; i < count - 1; ++i)
		{
			StackRunner::addToken(prog, StackRunner::ADD, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, -1)));
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

			StackRunner::addToken(prog, StackRunner::SYNC, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, id())));
			set_synced();
		}
	
	}

	std::string NodeAdd::get_color() const
	{
		return "#00FF00";
	}
}
#include "Edge.h"
#include "Node.h"
#include "PersistenceUtils.h"

namespace msonlab
{
	Edge::Edge(unsigned int id_, types::LabelType label_, types::DataPtr value_, NodePtr from_, NodePtr to_,
		EdgeDescriptor descriptor_)
		: IProcessable(id_, label_, value_), from(from_), to(to_), paramReady(false), _descriptor(descriptor_)
	{
	}

	Edge::Edge(const Edge& other) : IProcessable(other), _descriptor(other._descriptor)
	{
		if (this != &other)
		{
			*this = other;
		}
	}

	Edge& Edge::operator=(const Edge& other) 
	{
		IProcessable::operator=(other);

		this->paramReady = other.paramReady;
		this->from = other.from;
		this->to = other.to;
		this->_descriptor = other._descriptor;

		return *this;
	}

	bool Edge::registerParameter()
	{
		paramReady = true;

		return true;
	}

	IProcessableVect Edge::process()
	{
		if (isReadyForProcess())
		{
			if (setProcessed((*lock_from()).getResultValue()))
			{
				IProcessableVect retVal;
				
				auto to = lock_to();
				if((*to).registerParameter())
					retVal.insert(retVal.begin(), to);

				return retVal;
			}
			else
				throw msonlab::Exceptions::GeneralErrorException("Error while setting the result of processing on this processable element!");
		}
		else
		{
			throw msonlab::Exceptions::StillNotReadyForProcessException("This processable element is not yet ready for processing!");
		}
	}

	bool Edge::isReadyForProcess() const
	{
		return paramReady;
	}

	bool Edge::resetProcessingState()
	{
		IProcessable::resetProcessingState();
		paramReady = false;
		return true;
	}

	NodePtr Edge::opposite(NodePtr x)
	{
		auto to = lock_to();
		auto from = lock_from();
		if (to == x)
			return from;
		else if (from  == x)
			return to;
		else
			throw Exceptions::NotPartOfEdgeException("The node is not part of the current edge!");
	}

	NodePtr Edge::getFrom() const
	{
		return lock_from();
	}

	NodePtr Edge::getTo() const
	{
		return lock_to();
	}

	unsigned Edge::getFromId() const
	{
		return lock_from()->id();
	}

	unsigned Edge::getToId() const
	{
		return lock_to()->id();
	}

	std::string Edge::getIdString() const
	{
		std::string ret("e");
		ret = ret + std::to_string(id());
		return ret;
	}

	Edge::EdgeTypeEnum Edge::getEdgeType() const
	{
		return default_edge;
	}
	// compile

	void Edge::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
	{
		unsigned int thread_id = schedule.at(id());
		StackRunner::program* prog = &(programs->at(thread_id));

		// if already synced, just push the associated future object to the stack
		if (synced)
		{
			StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, id())));
			return;
		}

		auto from = lock_from();
		if (from->compile_iteration < compile_iteration)
		{
			// already has value
			StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, from->id())));
		}
		else
		{
			// going deeper and calculate predecessor
			from->compile(thread_id, programs, schedule);

			// if predecessor is placed on another thread, add a wait operation
			if (schedule.at(from->id()) != thread_id)
			{
				// predecessor is on another thread, need to create a FutureStackValue
				StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, from->id())));
			}
		}

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
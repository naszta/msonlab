#include "MultiplierEdge.h"
#include "Node.h"
#include "PersistenceUtils.h"
#include "..\Executor\SimpleStackValue.h"

namespace msonlab
{
	MultiplierEdge::MultiplierEdge(unsigned int _id, types::LabelType _label, types::DataPtr _value, NodePtr _from, NodePtr _to, types::DataType _multiplier)
		: Edge(_id, _label, _value, _from, _to)
	{
		multiplier = _multiplier;
	}

	IProcessableVect MultiplierEdge::process()
	{
		IProcessableVect ret;

		if (isReadyForProcess())
		{
			// TODO: static class for operands
			if (setProcessed((*getFrom()).getResultValue()))
			{
				if ((*getTo()).registerParameter())
				{
					ret.insert(ret.begin(), (getTo()));
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

	void MultiplierEdge::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
	{
		unsigned int thread_id = schedule.at(id());
		StackRunner::program* prog = &(programs->at(thread_id));

		// if already synced, just push the associated future object to the stack
		if (synced)
		{
			StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, id())));
			return;
		}

		auto from = getFrom();
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

		// add MUL operation
		types::DataPtr data = std::make_shared<types::DataType>(multiplier);
		StackValue::stackvaluePtr stackVal = std::make_shared<SimpleStackValue>(data);
		StackRunner::addToken(prog, StackRunner::PUSH, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(stackVal, -1)));
		StackRunner::addToken(prog, StackRunner::MUL, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, -1)));


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

	// exchange
	std::string MultiplierEdge::getTypeString() const
	{
		return persistence::edgeTypeToString(persistence::SupportedEdgeType::MULTIPLIER);
	}

	std::string MultiplierEdge::get_target_arrow_style() const
	{
		// plain, standard
		return "plain";
	}

	std::string MultiplierEdge::get_custom_data() const
	{
		return std::to_string(multiplier);
	}

}
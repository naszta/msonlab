#pragma once
#include "Node.h"

namespace msonlab
{
	Node::Node(unsigned int _id, wchar_t _label, Types::DataType _value)
		: IProcessable(_id, _label, _value), paramCount(0)
	{
	}

	Node::Node(const Node& other)
	{
		throw Exceptions::NotImplementedException("Node copy constructor");
	}

	bool Node::registerParameter()
	{
		++paramCount;

		return isReadyForProcess();
	}

	IProcessable::pVect Node::process()
	{
		throw Exceptions::NotImplementedException("Node::process function");
	}

	bool Node::isReadyForProcess() const
	{
		if (predecessors.size() == paramCount)
			return true;
		else
			return false;
	}

	bool Node::resetProcessingState()
	{
		IProcessable::resetProcessingState();
		paramCount = 0;
		return true;
	}

	IProcessable::pVect Node::getPredecessors() const
	{
		return predecessors;
	}

	IProcessable::pVect Node::getSuccessors() const
	{
		return successors;
	}

	bool Node::registerPredecessor(IProcessable::pPtr _newPredecessor)
	{
		msonlab::IProcessable::pVect::iterator it;
		it = std::find(predecessors.begin(),predecessors.end(),_newPredecessor);

		if (it != predecessors.end())
			return false;

		it = predecessors.insert(predecessors.end(),_newPredecessor);

		if (*it == _newPredecessor)
			return true;
		else
		{
			throw Exceptions::FailedToAddEdgeException("Failed to register new predecessor to the node!");
			return false;
		}

	}

	bool Node::unregisterPredecessor(IProcessable::pPtr _newPredecessor)
	{
		throw Exceptions::NotImplementedException("Node::unregisterPredecessor function");
	}

	bool Node::registerSuccessor(IProcessable::pPtr _newSuccessor)
	{
		msonlab::IProcessable::pVect::iterator it;
		it = std::find(successors.begin(),successors.end(),_newSuccessor);

		if (it != successors.end())
			return false;

		it = successors.insert(successors.end(),_newSuccessor);

		if (*it == _newSuccessor)
			return true;
		else
		{
			throw Exceptions::FailedToAddEdgeException("Failed to register new successor to the node!");
			return false;
		}
	}

	bool Node::unregisterSuccessor(IProcessable::pPtr _newSuccessor)
	{
		throw Exceptions::NotImplementedException("Node::unregisterSuccessor function");
	}


	IProcessable::PlaceEnum Node::getPlace() const
	{
		if (predecessors.size() > 0 && successors.size() > 0)
			return IProcessable::Inside;
		else if (predecessors.size() > 0)
			return IProcessable::Output;
		else
			return IProcessable::Input;
	}
}
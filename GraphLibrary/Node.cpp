#pragma once
#include "Node.h"

namespace msonlab
{
	Node::Node(unsigned int _id, Types::LabelType _label, Types::DataType _value)
		: IProcessable(_id, _label, _value), paramCount(0), compTime(1)
	{
	}

	Node::Node(unsigned int _id, Types::LabelType _label, Types::DataType _value, unsigned compTime)
		: IProcessable(_id, _label, _value), paramCount(0), compTime(compTime)
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

	const IProcessable::eVect& Node::getPredecessors() const
	{
		return predecessors;
	}

	size_t Node::getPredecessorsSize() const
	{
		return predecessors.size();
	}

	IProcessable::ePtr Node::getPredecessor(size_t index) const
	{
		return predecessors[index];
	}

	IProcessable::eVect::iterator Node:: getPredecessorBegin()
	{
		return predecessors.begin();
	}

	IProcessable::eVect::iterator Node:: getPredecessorEnd()
	{
		return predecessors.end();
	}

	const IProcessable::eVect& Node::getSuccessors() const
	{
		return successors;
	}

	size_t Node::getSuccessorsSize() const
	{
		return successors.size();
	}

	bool Node::registerPredecessor(IProcessable::ePtr _newPredecessor)
	{
		msonlab::IProcessable::eVect::iterator it;
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

	bool Node::unregisterPredecessor(IProcessable::ePtr _newPredecessor)
	{
		throw Exceptions::NotImplementedException("Node::unregisterPredecessor function");
	}

	bool Node::registerSuccessor(IProcessable::ePtr _newSuccessor)
	{
		msonlab::IProcessable::eVect::iterator it;
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

	bool Node::unregisterSuccessor(IProcessable::ePtr _newSuccessor)
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

	// compile
	void Node::compile(msonlab::StackRunner::srPtr stackProgram)
	{

	}
}
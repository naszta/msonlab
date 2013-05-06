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

		return true;
	}

	bool Node::process()
	{
		throw Exceptions::NotImplementedException("Node::process function");
	}

	bool Node::isReady() const
	{
		if (predecessors.size() == paramCount)
			return true;
		else
			return false;
	}

	IProcessable::pVect Node::getPredecessors() const
	{
		return predecessors;
	}

	IProcessable::pVect Node::getSuccessors() const
	{
		return successors;
	}

	Node::PlaceEnum Node::getPlace() const
	{
		if (predecessors.size() > 0 && successors.size() > 0)
			return Node::Inside;
		else if (predecessors.size() > 0)
			return Node::Output;
		else
			return Node::Input;
	}
}
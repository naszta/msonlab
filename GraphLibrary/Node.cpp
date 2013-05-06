#pragma once
#include "Node.h"

namespace msonlab
{
	Node::Node(unsigned int _id, wchar_t _label, Types::DataType _value)
	{

	}

	Node::Node(const Node& other)
	{

	}

	bool Node::process()
	{
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
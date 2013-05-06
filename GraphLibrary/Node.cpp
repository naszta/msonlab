#pragma once
#include "Node.h"

namespace msonlab
{
	Node::Node(unsigned int _id, wchar_t _label, Types::DataType _value)
	{
		throw Exceptions::NotImplementedException("Node constructor");
	}

	Node::Node(const Node& other)
	{
		throw Exceptions::NotImplementedException("Node copy constructor");
	}

	bool Node::process()
	{
		throw Exceptions::NotImplementedException("Node::process function");
	}

	IProcessable::pVect Node::getPredecessors() const
	{
		throw Exceptions::NotImplementedException("Node::getPredecessors function");
	}

	IProcessable::pVect Node::getSuccessors() const
	{
		throw Exceptions::NotImplementedException("Node::getSuccessors function");
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
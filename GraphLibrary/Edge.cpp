#pragma once
#include "Edge.h"

namespace msonlab
{
	Edge::Edge(unsigned int _id, wchar_t _label, Types::DataType _value, Node::nPtr _from, Node::nPtr _to)
		: IProcessable(_id, _label, _value), from(_from), to(_to), paramReady(false)
	{
	}

	Edge::Edge(const Edge& other)
	{
		throw Exceptions::NotImplementedException("Edge copy constructor");
	}

	bool Edge::registerParameter()
	{
		paramReady = true;

		return true;
	}

	bool Edge::process()
	{
		throw Exceptions::NotImplementedException("Edge::process function");
	}

	bool Edge::isReady() const
	{
		return paramReady;
	}

	Node::nPtr Edge::opposite(Node::nPtr x)
	{
		if (to == x)
			return from;
		else if (from  == x)
			return to;
		else
			throw Exceptions::NotPartOfEdgeException("The node is not part of the current edge!");
	}

	Node::nPtr Edge::getFrom() const
	{
		return from;
	}

	Node::nPtr Edge::getTo() const
	{
		return to;
	}

}
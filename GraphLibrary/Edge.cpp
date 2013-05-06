#pragma once
#include "Edge.h"

namespace msonlab
{
	Edge::Edge(unsigned int _id, wchar_t _label, Types::DataType _value, Node::nPtr _from, Node::nPtr _to)
	{
		throw Exceptions::NotImplementedException("Edge contructor");
	}

	Edge::Edge(const Edge& other)
	{
		throw Exceptions::NotImplementedException("Edge copy constructor");
	}

	bool Edge::process()
	{
		throw Exceptions::NotImplementedException("Edge::process function");
	}

	Node::nPtr Edge::opposite(Node::nPtr x)
	{
		throw Exceptions::NotImplementedException("Edge::opposite function");
	}

	Node::nPtr Edge::getFrom() const
	{
		throw Exceptions::NotImplementedException("Edge::getFrom function");
	}

	Node::nPtr Edge::getTo() const
	{
		throw Exceptions::NotImplementedException("Edge::getTo function");
	}

}
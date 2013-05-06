#pragma once
#include "Edge.h"

namespace msonlab
{
	Edge::Edge(unsigned int _id, wchar_t _label, Types::DataType _value, Node::nPtr _from, Node::nPtr _to)
	{

	}

	Edge::Edge(const Edge& other)
	{

	}

	bool Edge::process()
	{
		return false;
	}

	Node::nPtr Edge::opposite(Node::nPtr x)
	{
		return from;
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
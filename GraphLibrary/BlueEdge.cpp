#pragma once
#include "BlueEdge.h"
#include "Node.h"

namespace msonlab
{
	BlueEdge::BlueEdge(unsigned int _id, Types::LabelType _label, Types::DataType _value, IProcessable::nPtr _from, IProcessable::nPtr _to)
		: Edge(_id, _label, _value, _from, _to)
	{
	}

	Edge::EdgeTypeEnum BlueEdge::getEdgeType() const
	{
		return blue_edge;
	}

}
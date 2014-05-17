#pragma once
#include "Edge.h"


namespace msonlab
{
	class BlueEdge : public Edge
	{
	public:
		BlueEdge(unsigned int _id, Types::LabelType _label, Types::DataType _value, IProcessable::nPtr _from, IProcessable::nPtr _to);

		virtual EdgeTypeEnum getEdgeType() const;
	};

}
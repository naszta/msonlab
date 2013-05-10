#pragma once
#include "Edge.h"

namespace msonlab
{
	class MultiplierEdge : public Edge
	{
	private:
		double multiplier;
	public:
		MultiplierEdge(unsigned int _id, wchar_t _label, Types::DataType _value, Node::nPtr _from, Node::nPtr _to, double _multiplier);

		virtual IProcessable::pVect process();
	};
}
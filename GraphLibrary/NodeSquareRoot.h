#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeSquareRoot : public Node
	{
	public:
		NodeSquareRoot(unsigned int _id, Types::LabelType _label, Types::DataPtr _value);

		virtual IProcessable::pVect process();

		unsigned getComputationTime() { return 7; }
	};
}
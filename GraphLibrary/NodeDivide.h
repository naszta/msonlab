#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeDivide : public Node
	{
	public:
		NodeDivide(unsigned int _id, Types::LabelType _label, Types::DataPtr _value);

		virtual IProcessable::pVect process();

		// compile
		virtual void compile(msonlab::StackRunner::srPtr stackProgram);

		unsigned getComputationTime() { return 5; }
	};
}
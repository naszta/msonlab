#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeAdd : public Node
	{
	public:
		NodeAdd(unsigned int _id, Types::LabelType _label, Types::DataType _value);

		virtual IProcessable::pVect process();

		// compile
		virtual void compile(msonlab::StackRunner::srPtr stackProgram);

		unsigned getComputationTime() { return 5; }
	};
}
#pragma once
#include "Node.h"

namespace msonlab
{
	class NodePower : public Node
	{
	public:
		NodePower(unsigned int _id, wchar_t _label, Types::DataType _value);

		virtual IProcessable::pVect process();

		// compile
		virtual void compile(msonlab::StackRunner::srPtr stackProgram);
	};



}
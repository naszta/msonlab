#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeMultiply : public Node
	{
	public:
		NodeMultiply(unsigned int _id, wchar_t _label, Types::DataType _value);

		virtual IProcessable::pVect process();
	};



}
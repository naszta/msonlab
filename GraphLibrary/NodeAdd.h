#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeAdd : public Node
	{
	public:
		NodeAdd(unsigned int _id, wchar_t _label, Types::DataType _value);

		virtual IProcessable::pVect process();
	};



}
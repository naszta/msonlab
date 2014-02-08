#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeDivide : public Node
	{
	public:
		NodeDivide(unsigned int _id, wchar_t _label, Types::DataType _value);

		virtual IProcessable::pVect process();
	};



}
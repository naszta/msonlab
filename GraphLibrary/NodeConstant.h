#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeConstant : public Node
	{
	public:
		NodeConstant(unsigned int _id, wchar_t _label, Types::DataType _value);

		virtual IProcessable::pVect process();
	};



}
#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeSquareRoot : public Node
	{
	public:
		NodeSquareRoot(unsigned int _id, wchar_t _label, Types::DataType _value);

		virtual IProcessable::pVect process();
	};



}
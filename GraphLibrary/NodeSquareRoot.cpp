#pragma once
#include "NodeSquareRoot.h"

namespace msonlab
{
	NodeSquareRoot::NodeSquareRoot(unsigned int _id, wchar_t _label, Types::DataType _value)
		: Node(_id, _label, _value)
	{
	}

	IProcessable::pVect NodeSquareRoot::process()
	{
		throw Exceptions::NotImplementedException("NodeSquareRoot::process function");
	}
}
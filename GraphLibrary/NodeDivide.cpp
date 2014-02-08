#pragma once
#include "NodeDivide.h"

namespace msonlab
{
	NodeDivide::NodeDivide(unsigned int _id, wchar_t _label, Types::DataType _value)
		: Node(_id, _label, _value)
	{
	}

	IProcessable::pVect NodeDivide::process()
	{
		throw Exceptions::NotImplementedException("NodeDivide::process function");
	}
}
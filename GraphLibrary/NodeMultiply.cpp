#pragma once
#include "NodeMultiply.h"

namespace msonlab
{
	NodeMultiply::NodeMultiply(unsigned int _id, wchar_t _label, Types::DataType _value)
		: Node(_id, _label, _value)
	{
	}

	IProcessable::pVect NodeMultiply::process()
	{
		throw Exceptions::NotImplementedException("NodeMultiply::process function");
	}
}
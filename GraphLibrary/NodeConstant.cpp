#pragma once
#include "NodeConstant.h"

namespace msonlab
{
	NodeConstant::NodeConstant(unsigned int _id, wchar_t _label, Types::DataType _value)
		: Node(_id, _label, _value)
	{
	}

	IProcessable::pVect NodeConstant::process()
	{
		throw Exceptions::NotImplementedException("NodeConstant::process function");
	}
}
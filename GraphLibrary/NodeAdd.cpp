#pragma once
#include "NodeAdd.h"

namespace msonlab
{
	NodeAdd::NodeAdd(unsigned int _id, wchar_t _label, Types::DataType _value)
		: Node(_id, _label, _value)
	{
	}

	IProcessable::pVect NodeAdd::process()
	{
		throw Exceptions::NotImplementedException("NodeAdd::process function");
	}
}
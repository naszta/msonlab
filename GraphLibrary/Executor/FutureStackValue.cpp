#include "FutureStackValue.h"
#include "../Global.h"

namespace msonlab
{
	FutureStackValue::FutureStackValue(types::FutureDataType val)
	{
		value = val;
	}

	types::DataPtr FutureStackValue::getValue()
	{
		return (*value).get();
	}

	FutureStackValue::~FutureStackValue()
	{

	}
}

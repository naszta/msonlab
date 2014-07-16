#pragma once
#include "FutureStackValue.h"
#include "Global.h"


namespace msonlab
{
	FutureStackValue::FutureStackValue(Types::FutureDataType val)
	{
		value = val;
	}


	Types::DataPtr FutureStackValue::getValue()
	{
		return (*value).get();
	}

	FutureStackValue::~FutureStackValue()
	{

	}

}


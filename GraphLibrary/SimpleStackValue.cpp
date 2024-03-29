#pragma once
#include "SimpleStackValue.h"
#include "Global.h"

#include <string>
#include <sstream>
#include <iomanip>


namespace msonlab
{

	SimpleStackValue::SimpleStackValue(Types::DataPtr val)
	{
		value = val;
	}

	Types::DataPtr msonlab::SimpleStackValue::getValue()
	{
		return value;
	}

	std::string SimpleStackValue::to_string()
	{
		std::stringstream ss;
		ss << std::setprecision(2)<< *value;
		return ss.str();
	}
}

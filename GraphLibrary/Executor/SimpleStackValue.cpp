#include "SimpleStackValue.h"

#include <string>
#include <sstream>
#include <iomanip>

#include "../Global.h"

namespace msonlab
{
	SimpleStackValue::SimpleStackValue(types::DataPtr val)
	{
		value = val;
	}

	types::DataPtr msonlab::SimpleStackValue::getValue()
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

#include "StackValue.h"
#include "../Global.h"

namespace msonlab
{
	types::DataPtr StackValue::getValue()
	{
		throw new Exceptions::NotImplementedException("StackValue can't be used. Use one inherited class.");
	}

	std::string StackValue::to_string()
	{
		return "";
	}
}
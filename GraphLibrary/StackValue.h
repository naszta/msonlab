#pragma once
#include "Global.h"

namespace msonlab
{
	class StackValue 
	{
	public:
		typedef std::shared_ptr<StackValue> stackvaluePtr;

		virtual Types::DataType getValue();

		virtual std::string to_string();
	};

}
#pragma once
#include "Global.h"

namespace msonlab
{
	class StackValue 
	{
	public:
		typedef std::shared_ptr<StackValue> stackvaluePtr;

		virtual Types::DataPtr getValue();

		virtual std::string to_string();
	};

}
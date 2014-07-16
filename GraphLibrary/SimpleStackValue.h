#pragma once
#include "StackValue.h"

namespace msonlab
{
	class SimpleStackValue :
		public StackValue
	{

	private:
		Types::DataType value;

	public:
		SimpleStackValue(Types::DataType val);

		Types::DataType getValue();

		std::string to_string();
	};

}
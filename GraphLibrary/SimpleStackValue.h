#pragma once
#include "StackValue.h"

namespace msonlab
{
	class SimpleStackValue :
		public StackValue
	{

	private:
		Types::DataPtr value;

	public:
		SimpleStackValue(Types::DataPtr val);

		Types::DataPtr getValue();

		std::string to_string();
	};

}
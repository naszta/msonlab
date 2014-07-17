#pragma once
#include "StackValue.h"

namespace msonlab
{
	class SimpleStackValue :
		public StackValue
	{

	private:
		types::DataPtr value;

	public:
		SimpleStackValue(types::DataPtr val);

		types::DataPtr getValue();

		std::string to_string();
	};

}
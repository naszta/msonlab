#ifndef SIMPLE_STACK_VALUE_H
#define SIMPLE_STACK_VALUE_H
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
#endif
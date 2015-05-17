#ifndef FUTURE_STACK_VALUE_H
#define FUTURE_STACK_VALUE_H
#include "StackValue.h"

namespace msonlab
{
	class FutureStackValue : public StackValue
	{
	private:
		types::FutureDataType value;

	public:
		FutureStackValue(types::FutureDataType val);

		types::DataPtr getValue();

		~FutureStackValue();
	};
}
#endif
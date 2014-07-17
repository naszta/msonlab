#pragma once
#include "StackValue.h"

namespace msonlab
{
	class FutureStackValue :
		public StackValue
	{
	private:
		types::FutureDataType value;

	public:
		FutureStackValue(types::FutureDataType val);

		types::DataPtr getValue();

		~FutureStackValue();
	};

}
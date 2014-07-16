#pragma once
#include "StackValue.h"

namespace msonlab
{
	class FutureStackValue :
		public StackValue
	{
	private:
		Types::FutureDataType value;

	public:
		FutureStackValue(Types::FutureDataType val);

		Types::DataPtr getValue();

		~FutureStackValue();
	};

}
#pragma once
#include <vector>
#include <boost\shared_ptr.hpp>
#include <boost\variant\variant.hpp>
#include <iostream>


using std::vector;

namespace msonlab
{

	namespace Types
	{
		typedef boost::variant<int> DataType;

	}

	namespace Exceptions
	{
		struct NotImplementedException
		{
			const char* functionName;
			NotImplementedException(const char* _functionName) : functionName(_functionName){}
		};


	}
}
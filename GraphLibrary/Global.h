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



	}
}
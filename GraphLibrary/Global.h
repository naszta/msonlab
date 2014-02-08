#pragma once
#include <vector>
#include <set>
#include <boost\shared_ptr.hpp>
#include <boost\variant\variant.hpp>
#include <iostream>
#include <algorithm>



using std::vector;

namespace msonlab
{

	namespace Types
	{
		typedef boost::variant<int,unsigned,wchar_t> DataType;

	}

	namespace Exceptions
	{
		struct GeneralErrorException
		{
			const char* msg;
			GeneralErrorException(const char* _msg) 
				: msg(_msg){}
		};

		struct NotImplementedException
		{
			const char* msg;
			NotImplementedException(const char* _msg) 
				: msg(_msg){}
		};

		struct FailedToAddNodeException
		{
			const char* msg;
			FailedToAddNodeException(const char* _msg) 
				: msg(_msg){}
		};

		struct NotPartOfEdgeException
		{
			const char* msg;
			NotPartOfEdgeException(const char* _msg) 
				: msg(_msg){}
		};

		struct FailedToAddEdgeException
		{
			const char* msg;
			FailedToAddEdgeException(const char* _msg) 
				: msg(_msg){}
		};

		struct ResultStillNotReadyException
		{
			const char* msg;
			ResultStillNotReadyException(const char* _msg) 
				: msg(_msg){}
		};

		struct StillNotReadyForProcessException
		{
			const char* msg;
			StillNotReadyForProcessException(const char* _msg) 
				: msg(_msg){}
		};


	}
}
#pragma once
#include <vector>
#include <set>
#include <memory>
#include <iostream>
#include <algorithm>
#include <string>

using std::vector;

namespace msonlab
{

	namespace Types
	{
		typedef std::shared_ptr<double> DataType;
		typedef std::wstring LabelType;
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
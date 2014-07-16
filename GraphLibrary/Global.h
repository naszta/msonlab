#pragma once
#include <vector>
#include <set>
#include <memory>
#include <iostream>
#include <algorithm>
#include <string>

using std::vector;

#define PRINT 1
#if PRINT == 1
#define DEBUG(str) std::cout << str
#define DEBUGLN(str) std::cout << str << std::endl
#else
#define DEBUG(str) 
#define DEBUGLN(str)
#endif

namespace msonlab
{

	namespace Types
	{
		typedef double DataType;
		typedef std::shared_ptr<DataType> DataPtr;
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
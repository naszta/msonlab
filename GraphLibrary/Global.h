#pragma once
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <iostream>
#include <algorithm>
#include <memory>
#include <string>
#include <thread>
#include <future>

#include <xercesc\util\PlatformUtils.hpp>
#include <xercesc\dom\DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc\parsers\XercesDOMParser.hpp>
#include <xercesc\sax\HandlerBase.hpp>
XERCES_CPP_NAMESPACE_USE

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

	namespace types
	{
		typedef double DataType;
		typedef std::shared_ptr<DataType> DataPtr;
		typedef std::wstring LabelType;
		typedef std::shared_ptr<std::shared_future<DataPtr>> FutureDataType;
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

		struct NodeSquareRootPredecessorsAreNotValidException
		{
			const char* msg;
			NodeSquareRootPredecessorsAreNotValidException(const char* _msg)
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

		struct NodeTypeCanNotBeSerializedException
		{
			const char* msg;
			NodeTypeCanNotBeSerializedException(const char* _msg)
				: msg(_msg){}
		};

		struct UnknownNodeTypeException
		{
			const char* msg;
			UnknownNodeTypeException(const char* _msg)
				: msg(_msg){}
		};

		struct UnknownEdgeTypeException
		{
			const char* msg;
			UnknownEdgeTypeException(const char* _msg)
				: msg(_msg){}
		};


	}
}
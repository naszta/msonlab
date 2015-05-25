#ifndef GRAPHLIB_GRAPH_FUNCTIONBUILDER
#define GRAPHLIB_GRAPH_FUNCTIONBUILDER

#include <memory>
#include <string>
#include <map>

#include "Function.h"

namespace msonlab { namespace graph { namespace function {

	using FunctionPtr = std::unique_ptr < Function > ;

	class FunctionBuilder {
	public:
		FunctionBuilder(std::string type_);
		FunctionBuilder(const FunctionBuilder&) = delete;
		FunctionBuilder& operator=(const FunctionBuilder&) = delete;
		static void registerFunction(std::string, const Function*);
		FunctionPtr build(functiondata value_ = 0.0);
	private:
		static std::map < std::string, const Function* >& registered();
		const std::string _type;
	};

}}}

#endif
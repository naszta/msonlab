#include "FunctionBuilder.h"

#include <utility>

namespace msonlab { namespace graph { namespace function {

	FunctionBuilder::FunctionBuilder(std::string type_) : _type(type_) {
	}

	std::map<std::string, const Function*>& FunctionBuilder::registered() {
		static std::map<std::string, const Function*> functions_map;
		return functions_map;
	}

	void FunctionBuilder::registerFunction(std::string _type, const Function* _func) {
		FunctionBuilder::registered()[_type] = _func;
	}

	std::unique_ptr<Function> FunctionBuilder::build(functiondata value_) {
		const auto it = FunctionBuilder::registered().find(_type);
		return it == FunctionBuilder::registered().end() ? nullptr : (*it).second->clone(value_);
	}

}}}
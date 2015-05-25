#ifndef GRAPHLIB_GRAPH_FUNCTION
#define GRAPHLIB_GRAPH_FUNCTION

#include <vector>
#include <memory>

namespace msonlab { namespace graph { namespace function {

	using std::vector;

	namespace {
		struct funcexample{};
	}

	using functiondata = double;
	using functionlist = vector < functiondata >;
	class Function;
	using functionptr = std::unique_ptr < Function > ;

	class Function {
	public:
		// evaluates the function
		virtual functiondata evaluate(const functionlist& input) const = 0;
		virtual functionptr clone(functiondata) const = 0;
		virtual void value(functiondata) {}
		virtual ~Function() {}
	};

	class FunctionAdd : public Function {
	public:
		FunctionAdd() = default;
		FunctionAdd(funcexample);
		FunctionAdd(const FunctionAdd&) = delete;
		FunctionAdd(const FunctionAdd&&) = delete;
		FunctionAdd& operator=(const FunctionAdd&) = delete;
		FunctionAdd& operator=(const FunctionAdd&&) = delete;
		virtual functiondata evaluate(const functionlist& input) const override;
		virtual functionptr clone(functiondata _value = 0.0) const override;
	private:
		static FunctionAdd example;
	};

	class FunctionConstant : public Function {
	public:
		FunctionConstant(functiondata value_);
		FunctionConstant(funcexample);
		FunctionConstant(const FunctionConstant&) = delete;
		FunctionConstant(const FunctionConstant&&) = delete;
		FunctionConstant& operator=(const FunctionConstant&) = delete;
		FunctionConstant& operator=(const FunctionConstant&&) = delete;
		virtual functiondata evaluate(const functionlist& input) const override;
		virtual functionptr clone(functiondata _value = 0.0) const override;
	private:
		static FunctionConstant example;
		const functiondata _value;
	};

	class FunctionDivide : public Function {
	public:
		FunctionDivide() = default;
		FunctionDivide(funcexample);
		FunctionDivide(const FunctionDivide&) = delete;
		FunctionDivide(const FunctionDivide&&) = delete;
		FunctionDivide& operator=(const FunctionDivide&) = delete;
		FunctionDivide& operator=(const FunctionDivide&&) = delete;
		virtual functiondata evaluate(const functionlist& input) const override;
		virtual functionptr clone(functiondata _value = 0.0) const override;
	private:
		static FunctionDivide example;
	};

	class FunctionInput : public Function {
	public:
		FunctionInput(functiondata value_ = 0);
		FunctionInput(funcexample);
		FunctionInput(const FunctionInput&) = delete;
		FunctionInput(const FunctionInput&&) = delete;
		FunctionInput& operator=(const FunctionInput&) = delete;
		FunctionInput& operator=(const FunctionInput&&) = delete;
		void value(functiondata value_) override;
		virtual functiondata evaluate(const functionlist& input) const override;
		virtual functionptr clone(functiondata _value = 0.0) const override;
	private:
		functiondata _value;
		static FunctionInput example;
	};

	class FunctionSubtract : public Function {
	public:
		FunctionSubtract() = default;
		FunctionSubtract(funcexample);
		FunctionSubtract(const FunctionSubtract&) = delete;
		FunctionSubtract(const FunctionSubtract&&) = delete;
		FunctionSubtract& operator=(const FunctionSubtract&) = delete;
		FunctionSubtract& operator=(const FunctionSubtract&&) = delete;
		virtual functiondata evaluate(const functionlist& input) const override;
		virtual functionptr clone(functiondata _value = 0.0) const override;
	private:
		static FunctionSubtract example;
	};

	class FunctionMultiply : public Function {
	public:
		FunctionMultiply() = default;
		FunctionMultiply(funcexample);
		FunctionMultiply(const FunctionMultiply&) = delete;
		FunctionMultiply(const FunctionMultiply&&) = delete;
		FunctionMultiply& operator=(const FunctionMultiply&) = delete;
		FunctionMultiply& operator=(const FunctionMultiply&&) = delete;
		virtual functiondata evaluate(const functionlist& input) const override;
		virtual functionptr clone(functiondata _value = 0.0) const override;
	private:
		static FunctionMultiply example;
	};
}}}

#endif
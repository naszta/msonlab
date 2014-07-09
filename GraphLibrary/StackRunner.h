#pragma once

#include <utility>
#include <stack>
#include <vector>

#include "Global.h"

namespace msonlab
{
	class StackRunner
	{
	public:

		// public typedefinitions
		typedef std::shared_ptr<StackRunner> srPtr;

		typedef std::stack<msonlab::Types::DataPtr> dPtrStack;

		typedef void(*fPtr)(dPtrStack*,msonlab::Types::DataPtr);
		typedef std::pair<fPtr,msonlab::Types::DataPtr> token;

		typedef std::vector<token*> program;


		enum functionTypeEnum
		{
			PUSH,
			MUL,
			POW,
			SUB,
			ADD,
			DIV
		};

	private:
		friend std::ostream& operator<<(std::ostream& os, const msonlab::StackRunner& sr);
		// private variables
		program currentProgram;
		dPtrStack dataStack;

	public:
		// public stack functions
		static void push(dPtrStack* stack, msonlab::Types::DataPtr data);

		static void mul(dPtrStack* stack, msonlab::Types::DataPtr data);
		static void power(dPtrStack* stack, msonlab::Types::DataPtr data);
		static void sub(dPtrStack* stack, msonlab::Types::DataPtr data);
		static void add(dPtrStack* stack, msonlab::Types::DataPtr data);
		static void div(dPtrStack* stack, msonlab::Types::DataPtr data);

		// public build functions
		void addToken(functionTypeEnum functionType, msonlab::Types::DataPtr data);

		// public running functions
		msonlab::Types::DataPtr run(int count);
	};
}
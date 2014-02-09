#pragma once

#include "boost\shared_ptr.hpp"

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
		// typedef boost::shared_ptr<double> dataType;

		typedef boost::shared_ptr<StackRunner> srPtr;

		typedef std::stack<msonlab::Types::DataType> dPtrStack;

		typedef void(*fPtr)(dPtrStack*,msonlab::Types::DataType);
		typedef std::pair<fPtr,msonlab::Types::DataType> token;

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

		// private variables
		program currentProgram;
		dPtrStack dataStack;


	public:

		// public stack functions
		static void push(dPtrStack* stack, msonlab::Types::DataType data);

		static void mul(dPtrStack* stack, msonlab::Types::DataType data);
		static void power(dPtrStack* stack, msonlab::Types::DataType data);
		static void sub(dPtrStack* stack, msonlab::Types::DataType data);
		static void add(dPtrStack* stack, msonlab::Types::DataType data);
		static void div(dPtrStack* stack, msonlab::Types::DataType data);


		// public build functions
		void addToken(functionTypeEnum functionType, msonlab::Types::DataType data);


		// public running functions
		msonlab::Types::DataType run(int count);




	};

}
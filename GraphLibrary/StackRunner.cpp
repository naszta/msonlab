#pragma once

#include "StackRunner.h"
#include <math.h>

#include "Global.h"


// public stack functions
namespace msonlab
{
	using std::make_shared;

	void msonlab::StackRunner::push(dPtrStack* stack, msonlab::Types::DataPtr data)
	{
		stack->push(data);
	}

	void msonlab::StackRunner::mul(dPtrStack* stack, msonlab::Types::DataPtr data)
	{
		msonlab::Types::DataPtr t1 = stack->top();
		stack->pop();
		msonlab::Types::DataPtr t2 = stack->top();
		stack->pop();

		stack->push(make_shared<Types::DataType>(*t1 * *t2));
	}

	void msonlab::StackRunner::power(dPtrStack* stack, msonlab::Types::DataPtr data)
	{
		msonlab::Types::DataPtr t1 = stack->top();
		stack->pop();
		msonlab::Types::DataPtr t2 = stack->top();
		stack->pop();

		stack->push(make_shared<Types::DataType>(pow(*t2, *t1)));
	}

	void msonlab::StackRunner::sub(dPtrStack* stack, msonlab::Types::DataPtr data)
	{
		msonlab::Types::DataPtr t1 = stack->top();
		stack->pop();
		msonlab::Types::DataPtr t2 = stack->top();
		stack->pop();

		stack->push(make_shared<Types::DataType>(*t2 - *t1));
	}

	void msonlab::StackRunner::add(dPtrStack* stack, msonlab::Types::DataPtr data)
	{
		msonlab::Types::DataPtr t1 = stack->top();
		stack->pop();
		msonlab::Types::DataPtr t2 = stack->top();
		stack->pop();

		stack->push(make_shared<Types::DataType>(*t1 + *t2));
	}

	void msonlab::StackRunner::div(dPtrStack* stack, msonlab::Types::DataPtr data)
	{
		msonlab::Types::DataPtr t1 = stack->top();
		stack->pop();
		msonlab::Types::DataPtr t2 = stack->top();
		stack->pop();

		stack->push(make_shared<Types::DataType>(*t2 / *t1));
	}


	// public build functions
	void msonlab::StackRunner::addToken(functionTypeEnum functionType, msonlab::Types::DataPtr data)
	{
		switch (functionType)
		{
		case StackRunner::PUSH:
			currentProgram.insert(currentProgram.end(), new token(&StackRunner::push, data));
			break;
		case StackRunner::MUL:
			currentProgram.insert(currentProgram.end(), new token(&StackRunner::mul, data));
			break;
		case StackRunner::POW:
			currentProgram.insert(currentProgram.end(), new token(&StackRunner::power, data));
			break;
		case StackRunner::SUB:
			currentProgram.insert(currentProgram.end(), new token(&StackRunner::sub, data));
			break;
		case StackRunner::ADD:
			currentProgram.insert(currentProgram.end(), new token(&StackRunner::add, data));
			break;
		case StackRunner::DIV:
			currentProgram.insert(currentProgram.end(), new token(&StackRunner::div, data));
			break;
		default:
			break;
		}
	}


	// public running functions
	msonlab::Types::DataPtr msonlab::StackRunner::run(int count)
	{
		// repeat specified times
		for (; count > 0; --count)
		{
			// clear data stack
			dataStack = dPtrStack();

			// run the program
			for (StackRunner::program::iterator it = currentProgram.begin(); it != currentProgram.end(); ++it)
			{
				(*it)->first(&dataStack, (*it)->second);
			}
		}

		// return last result
		return dataStack.top();
	}

	std::ostream& operator<<(std::ostream& os, const StackRunner& sr)
	{
		for (auto it = sr.currentProgram.begin(); it != sr.currentProgram.end(); ++it)
		{
			void* func = std::get<0>(**it);
			if (func == &StackRunner::push)
			{
				os << "PUSH";
			}
			else if (func == &StackRunner::add)
			{
				os << "ADD";
			}
			else if (func == &StackRunner::sub)
			{
				os << "SUB";
			}
			else if (func == &StackRunner::mul)
			{
				os << "MUL";
			}
			else if (func == &StackRunner::div)
			{
				os << "DIV";
			}
			else if (func == &StackRunner::power)
			{
				os << "POW";
			}
			if (std::get<1>(**it) != nullptr)
				os << " => " << *(std::get<1>(**it));
			else
				os << " => null";
			os << std::endl;
		}

		return os;
	}

}
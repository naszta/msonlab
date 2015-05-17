#ifndef STACK_RUNNER_H
#define STACK_RUNNER_H

#include <utility>
#include <stack>
#include <vector>
#include <memory>

#include "Global.h"
#include "StackValue.h"

namespace msonlab
{
	class StackRunner
	{
	public:

		// public typedefinitions
		typedef std::shared_ptr<StackRunner> srPtr;
		typedef std::stack<StackValue::stackvaluePtr> dPtrStack;
		typedef void(*fPtr)(dPtrStack*, StackValue::stackvaluePtr, srPtr runner, int);
		typedef std::shared_ptr<std::pair<StackValue::stackvaluePtr, int>> dataToken;
		typedef std::pair<fPtr, dataToken> token;
		typedef std::vector<token*> program;
		enum functionTypeEnum
		{
			PUSH,
			MUL,
			POW,
			SUB,
			ADD,
			DIV,
			SYNC,
			WAIT,
			DUP
		};

		typedef std::shared_ptr<std::promise<types::DataPtr>> promisePtr;
		typedef std::shared_ptr<std::shared_future<types::DataPtr>> futurePtr;
		typedef vector<unsigned int> scheduleOrder;

	private:
		friend std::ostream& operator<<(std::ostream& os, const msonlab::StackRunner& sr);
		// private variables
		

		// private running functions
		void graph_runner(std::shared_ptr<std::shared_future<void>> start, unsigned int thread_id, srPtr runner);

	public:
		vector<program> programs;
		vector<dPtrStack> stacks;

		std::set<unsigned int> promise_candidates;
		std::set<unsigned int> future_candidates;

		std::vector<std::pair<unsigned int, promisePtr>> promises;
		std::vector<std::pair<unsigned int, futurePtr>> futures;

		vector<unsigned int> output_ids;

		unsigned int number_of_threads;

		// public stack functions
		static void push(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id = -1);
		static void mul(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id = -1);
		static void power(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id = -1);
		static void sub(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id = -1);
		static void add(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id = -1);
		static void div(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id = -1);
		static void sync(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id = -1);
		static void wait(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id = -1);
		static void dup(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id = -1);


		// public build functions
		static void addToken(program* toModify, functionTypeEnum functionType, dataToken data);

		// public running functions
		void run(srPtr runner, int count);

		// public debug functions
		void print_out_programs();

		static std::string getFunctionName(fPtr fun)
		{
			if (fun == fPtr(push))
				return "PUSH";
			if (fun == fPtr(mul))
				return "MUL";
			if (fun == fPtr(power))
				return "POWER";
			if (fun == fPtr(sub))
				return "SUB";
			if (fun == fPtr(add))
				return "ADD";
			if (fun == fPtr(div))
				return "DIV";
			if (fun == fPtr(sync))
				return "SYNC";
			if (fun == fPtr(wait))
				return "WAIT";
			if (fun == fPtr(dup))
				return "DUP";

			return "UNKOWN";
		}

	};
}
#endif
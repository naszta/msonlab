#pragma once

#include "StackRunner.h"
#include <math.h>

#include "Global.h"
#include "SimpleStackValue.h"
#include "FutureStackValue.h"


// public stack functions
namespace msonlab
{
	using std::make_shared;
	using namespace types;


	void msonlab::StackRunner::push(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id)
	{
		stack->push(data);
	}

	void msonlab::StackRunner::mul(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id)
	{
		msonlab::types::DataPtr t1 = (stack->top())->getValue();
		stack->pop();
		msonlab::types::DataPtr t2 = (stack->top())->getValue();
		stack->pop();

		DataPtr ret = make_shared<types::DataType>(*t1 * *t2);
		StackValue::stackvaluePtr ptr = make_shared<SimpleStackValue>(ret);
		stack->push(ptr);
	}

	void msonlab::StackRunner::power(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id)
	{
		msonlab::types::DataPtr t1 = (stack->top())->getValue();
		stack->pop();
		msonlab::types::DataPtr t2 = (stack->top())->getValue();
		stack->pop();

		DataPtr ret = make_shared<types::DataType>(pow(*t2, *t1));
		StackValue::stackvaluePtr ptr = make_shared<SimpleStackValue>(ret);
		stack->push(ptr);
	}

	void msonlab::StackRunner::sub(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id)
	{
		msonlab::types::DataPtr t1 = (stack->top())->getValue();
		stack->pop();
		msonlab::types::DataPtr t2 = (stack->top())->getValue();
		stack->pop();

		DataPtr ret = make_shared<types::DataType>(*t2 - *t1);
		StackValue::stackvaluePtr ptr = make_shared<SimpleStackValue>(ret);
		stack->push(ptr);
	}

	void msonlab::StackRunner::add(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id)
	{
		msonlab::types::DataPtr t1 = (stack->top())->getValue();
		stack->pop();
		msonlab::types::DataPtr t2 = (stack->top())->getValue();
		stack->pop();

		DataPtr ret = make_shared<types::DataType>(*t2 + *t1);
		StackValue::stackvaluePtr ptr = make_shared<SimpleStackValue>(ret);
		stack->push(ptr);
	}

	void msonlab::StackRunner::div(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id)
	{
		msonlab::types::DataPtr t1 = (stack->top())->getValue();
		stack->pop();
		msonlab::types::DataPtr t2 = (stack->top())->getValue();
		stack->pop();

		DataPtr ret = make_shared<types::DataType>(*t2 / *t1);
		StackValue::stackvaluePtr ptr = make_shared<SimpleStackValue>(ret);
		stack->push(ptr);
	}

	void msonlab::StackRunner::sync(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id)
	{
		std::pair<unsigned int, promisePtr> prom;
		for (auto p : runner->promises)
		{
			if (p.first == sync_id)
			{
				prom = p;
				break;
			}
		}
		prom.second->set_value((stack->top())->getValue());

		stack->pop();
	}

	void msonlab::StackRunner::wait(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id)
	{
		std::pair<unsigned int, futurePtr> fut;
		for (auto f : runner->futures)
		{
			if (f.first == sync_id)
			{
				fut = f;
				break;
			}
		}

		stack->push(StackValue::stackvaluePtr(new FutureStackValue(types::FutureDataType(fut.second))));
	}

	void msonlab::StackRunner::dup(dPtrStack* stack, StackValue::stackvaluePtr data, srPtr runner, int sync_id)
	{
		stack->push(stack->top());
	}

	void msonlab::StackRunner::addToken(program* toModify, functionTypeEnum functionType, dataToken data)
	{
		switch (functionType)
		{
		case StackRunner::PUSH:
			toModify->insert(toModify->end(), new token(&StackRunner::push, data));
			break;
		case StackRunner::MUL:
			toModify->insert(toModify->end(), new token(&StackRunner::mul, data));
			break;
		case StackRunner::POW:
			toModify->insert(toModify->end(), new token(&StackRunner::power, data));
			break;
		case StackRunner::SUB:
			toModify->insert(toModify->end(), new token(&StackRunner::sub, data));
			break;
		case StackRunner::ADD:
			toModify->insert(toModify->end(), new token(&StackRunner::add, data));
			break;
		case StackRunner::DIV:
			toModify->insert(toModify->end(), new token(&StackRunner::div, data));
			break;
		case StackRunner::SYNC:
			toModify->insert(toModify->end(), new token(&StackRunner::sync, data));
			break;
		case StackRunner::WAIT:
			toModify->insert(toModify->end(), new token(&StackRunner::wait, data));
			break;
		case StackRunner::DUP:
			toModify->insert(toModify->end(), new token(&StackRunner::dup, data));
			break;
		default:
			break;
		}
	}


	void msonlab::StackRunner::graph_runner(std::shared_ptr<std::shared_future<void>> start, unsigned int thread_id, srPtr runner)
	{
		// wait for main thread to signal
		start->wait();

		// get the program for the specified thread
		program prog = programs.at(thread_id);
		dPtrStack stack = stacks.at(thread_id);

		std::cout << "t" << thread_id << " started, program size: " << prog.size() << std::endl;

		// run the program
		for (auto t : prog)
		{
			auto func = t->first;
			StackValue::stackvaluePtr data = t->second->first;
			int sync_id = t->second->second;

			func(&stack, data, runner, sync_id);
		}

		std::cout << "t" << thread_id << " ended" << std::endl;

	}


	// public running functions
	void msonlab::StackRunner::run(srPtr runner, int count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			// create promises and futures
			promises = std::vector<std::pair<unsigned int, promisePtr>>();
			futures = std::vector<std::pair<unsigned int, futurePtr>>();

			for (auto i_id : promise_candidates)
			{
				auto prom = std::make_shared<std::promise<types::DataPtr>>();
				promises.push_back(std::pair<unsigned int, StackRunner::promisePtr>(i_id, prom));
				futures.push_back(std::pair<unsigned int, StackRunner::futurePtr>(i_id, StackRunner::futurePtr(new std::shared_future<types::DataPtr>(prom->get_future()))));
			}


			// create start promise and future for synchronized start
			std::shared_ptr<std::promise<void>> start_promise(new std::promise<void>);
			std::shared_ptr<std::shared_future<void>> start_future(new std::shared_future<void>(start_promise->get_future()));

			// create threads
			vector<std::thread> threads;

			for (unsigned int i = 0; i < number_of_threads; ++i)
			{
				threads.push_back(std::thread(&msonlab::StackRunner::graph_runner, *this, start_future, i, runner));
			}

			start_promise->set_value();



			for (unsigned int i = 0; i < output_ids.size(); ++i)
			{
				std::pair<unsigned int, futurePtr> fut;
				for (auto f : runner->futures)
				{
					if (f.first == output_ids.at(i))
					{
						fut = f;
						break;
					}
				}

				std::cout << "result for output (" << output_ids.at(i) << "): " << *(fut.second->get()) << std::endl;
			}

			for (unsigned int i = 0; i < number_of_threads; ++i)
			{
				threads.at(i).join();
			}

			promises.clear();
			futures.clear();
		}
	}


	// public debug functions
	void msonlab::StackRunner::print_out_programs()
	{
		std::cout << "Compile started..." << std::endl << std::endl << std::endl;

		for (size_t i = 0; i < programs.size(); ++i)
		{
			program p = programs.at(i);

			std::cout << "Program for thread #" << i << ": " << std::endl;
			std::cout << std::endl;

			for (auto t : p)
			{
				std::string data;
				if ((t->second->first) != nullptr)
					data = (t->second->first)->to_string();
				else
					data = "null";

				std::cout << StackRunner::getFunctionName(t->first)
					<< "\t"
					<< data
					<< "\t\t"
					<< t->second->second
					<< std::endl;
			}

			std::cout << "-----------------------------------" << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;

			std::cin.get();
		}
	}


}
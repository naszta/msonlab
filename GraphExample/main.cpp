#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <malloc.h>
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <chrono>
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

#include "../GraphLibrary/Graph/Graph.h"
#include "../GraphLibrary/Graph/GraphCreator.h"
#include "../GraphLibrary/Graph/GraphExchanger.h"
#include "../GraphLibrary/Graph/Function.h"
#include "../GraphLibrary/Graph/FunctionBuilder.h"
#include "../GraphLibrary/Algorithms.h"
#include "../GraphLibrary/litegraph.h"
#include "../GraphLibrary/Scheduler/GreedySchedulingAlgorithm.h"
#include "../GraphLibrary/Scheduler/GeneticAlgorithm.h"
#include "../GraphLibrary/Scheduler/ParallelGeneticAlgorithm.h"
#include "../GraphLibrary/Scheduler/SchedulingUtils.h"
#include "../GraphLibrary/Scheduler/FitnessStrategy.h"
#include "../GraphLibrary/Scheduler/ListSchedulingAlgorithm.h"
#include "../GraphLibrary/Scheduler/CriticalPathSchedulingAlgorithm.h"
#include "../GraphLibrary/Scheduler/CoffmanGrahamSchedulingAlgorithm.h"
//
//
//
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

using namespace std;
using namespace msonlab;
using namespace msonlab::lite;
using namespace msonlab::graph::algorithms;
using namespace msonlab::graph::function;
using namespace msonlab::scheduling;

std::chrono::time_point<std::chrono::high_resolution_clock> startCHRONO, finishCHRONO;

void run(const SchedulingAlgorithm& alg, const Graph& graph, const Options& options)
{	
	startCHRONO = std::chrono::high_resolution_clock::now();
	auto result = alg.schedule(graph, options);
	finishCHRONO = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedCHRONO = finishCHRONO - startCHRONO;
	std::cout << "Elapsed time " << std::setprecision(10) << elapsedCHRONO.count() << std::endl;
	try {
		is_correct(*result);
		write_solution(*result, options, std::cout);
	}
	catch (std::logic_error& e) {
		std::cout << e.what() << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "Unknow exception occured." << std::endl;
	}
}

int run_algorithms(int argc, char *argv[]) {
#ifdef _DEBUG
	_CrtMemState s1, s2, s3, s4;
	_CrtMemCheckpoint(&s1);
#endif
	try
	{
		std::string config_file_path = "Options.cfg";
		if (argc > 1) {
			config_file_path = argv[1];
		}
		Options options{ config_file_path.c_str() };
		Graph graph = graph::creator::createRandom(options.graphSize(), options.graphEdgeProb(), options.graphWidening(), options.numberOfPus());
		//Graph graph = graph::creator::createQuadrant();
		_CrtMemCheckpoint(&s2);
		srand(static_cast<unsigned>(time(NULL)));
		{
			std::cout << "greedy" << std::endl;
			GreedySchedulingAlgorithm alg{};
			run(alg, graph, options);
		}
		{
			std::cout << "list" << std::endl;
			ListSchedulingAlgorithm alg{};
			run(alg, graph, options);
		}
		{
			std::cout << "criticalpath" << std::endl;
			CriticalPathSchedulingAlgorithm alg{};
			run(alg, graph, options);
		}
		{
			std::cout << "coffmangraham" << std::endl;
			CoffmanGrahamSchedulingAlgorithm alg{};
			run(alg, graph, options);
			std::cout << std::endl;
		}	
		{
			std::cout << "genetic" << std::endl;
			GeneticAlgorithm alg{ FitnessStrategy::find_fitness_strategy(options.fitnessStrategy()) };
			run(alg, graph, options);
		}
		{
			std::cout << "parallel genetic" << std::endl;
			ParallelGeneticAlgorithm alg{ FitnessStrategy::find_fitness_strategy(options.fitnessStrategy()) };
			run(alg, graph, options);
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "Unknow exception occured." << std::endl;
	}

#ifdef _DEBUG
	// Printed in the debug output
	_CrtMemCheckpoint(&s3);
	if (_CrtMemDifference(&s4, &s1, &s3)) {
		_CrtMemDumpStatistics(&s1);
		_CrtMemDumpStatistics(&s2);
		_CrtMemDumpStatistics(&s3);
	}
	_CrtMemDumpStatistics(&s4);
#endif
	return 0;
}

int main(int argc, char *argv[]) {
	FunctionBuilder fb("ADD");
	auto result = fb.build();
	cout << result->evaluate({ 1, 2, 3 }) << endl;
	return 0;
}
#define _CRTDBG_MAP_ALLOC
#include <malloc.h>
#include <stdlib.h>
#include <crtdbg.h>

#include <chrono>
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

#include "../GraphLibrary/Graph.h"
//#include "../GraphLibrary/BFSIterator.h"
//#include "../GraphLibrary/DFSIterator.h"
#include "../GraphLibrary/GraphCreator.h"
#include "../GraphLibrary/GraphExchanger.h"
#include "../GraphLibrary/Algorithms.h"
#include "../GraphLibrary/litegraph.h"
#include "../GraphLibrary/GreedySchedulingAlgorithm.h"
#include "../GraphLibrary/GeneticAlgorithm.h"
#include "../GraphLibrary/SchedulingUtils.h"
#include "../GraphLibrary/FitnessStrategy.h"
#include "../GraphLibrary/ListSchedulingAlgorithm.h"
#include "../GraphLibrary/CriticalPathSchedulingAlgorithm.h"
#include "../GraphLibrary/CoffmanGrahamSchedulingAlgorithm.h"
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
	
}
// FIX CIRCULAR DEPENDENCY!
// to try out features of the GraphLibrary
int main(/*int argc, char *argv[]*/) {
	_CrtMemState s1, s2, s3, s4;
	_CrtMemCheckpoint(&s1);
	{
		//Graph graph = graph::creator::createQuadrant();
		Options options{ "Options1.cfg" };
		Graph graph = graph::creator::createRandom(options.getGraphSize(), options.getGraphEdgeProb(), options.getGraphWidening(), options.getNumberOfPus());
		//Graph graph = graph::creator::createRandomLeveledDAG(100, 12, 8);
		//srand(161903);
		_CrtMemCheckpoint(&s2);
		
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
			GeneticAlgorithm alg{ FitnessStrategy::find_fitness_strategy(options.getFitnessStrategy()) };
			run(alg, graph, options);
		}
	}
	_CrtMemCheckpoint(&s3);
	if (_CrtMemDifference(&s4, &s1, &s3)) {
		_CrtMemDumpStatistics(&s1);
		_CrtMemDumpStatistics(&s2);
		_CrtMemDumpStatistics(&s3);
	}
	_CrtMemDumpStatistics(&s4);
	//_CrtDumpMemoryLeaks();
}
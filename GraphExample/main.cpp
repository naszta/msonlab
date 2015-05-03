#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

#include "../GraphLibrary/Graph.h"
#include "../GraphLibrary/Node.h"
#include "../GraphLibrary/NodeConstant.h"
//#include "../GraphLibrary/BFSIterator.h"
//#include "../GraphLibrary/DFSIterator.h"
#include "../GraphLibrary/GraphCreator.h"
#include "../GraphLibrary/GraphExchanger.h"
#include "../GraphLibrary/Algorithms.h"
#include "../GraphLibrary/litegraph.h"
//#include "../GraphLibrary/GreedySchedulingAlgorithm.h"
#include "../GraphLibrary/GeneticAlgorithm.h"
#include "../GraphLibrary/SchedulingUtils.h"
#include "../GraphLibrary/FitnessStrategy.h"
//#include "../GraphLibrary/ListSchedulingAlgorithm.h"
//#include "../GraphLibrary/CriticalPathSchedulingAlgorithm.h"
#include "../GraphLibrary/CoffmanGrahamSchedulingAlgorithm.h"
//
//
//
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

using namespace std;
using namespace msonlab;
using namespace msonlab::lite;
using namespace msonlab::graph::algorithms;
using namespace msonlab::scheduling;

void run(const SchedulingAlgorithm& alg, const Graph& graph, const Options& options)
{
	auto result = alg.schedule(graph, options);
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
	{
		//Graph graph = graph::creator::createQuadrant();
		Options options{ "Options.cfg" };
		Graph graph = graph::creator::createRandom(options.getGraphSize(), options.getGraphEdgeProb(), options.getGraphWidening(), options.getNumberOfPus());
		//Graph graph = graph::creator::createRandomLeveledDAG(100, 12, 8);
		//srand(161903);
		
		/*
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
		}*/
		{
		std::cout << "coffmangraham" << std::endl;
		CoffmanGrahamSchedulingAlgorithm alg{};
		run(alg, graph, options);
		}
		{
		std::cout << "genetic" << std::endl;
		GeneticAlgorithm alg{ std::make_unique<LengthFitnessStartegy>() };
		run(alg, graph, options);
		}
	}
	_CrtDumpMemoryLeaks();
}
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

#include "../GraphLibrary/Graph.h"
#include "../GraphLibrary/Node.h"
#include "../GraphLibrary/BFSIterator.h"
#include "../GraphLibrary/DFSIterator.h"
#include "../GraphLibrary/GraphCreator.h"
#include "../GraphLibrary/GraphExchanger.h"
#include "../GraphLibrary/Algorithms.h"
#include "../GraphLibrary/litegraph.h"
#include "../GraphLibrary/GreedySchedulingAlgorithm.h"
#include "../GraphLibrary/GeneticAlgorithm.h"
#include "../GraphLibrary/SchedulingHelper.h"
#include "../GraphLibrary/FitnessStrategy.h"
#include "../GraphLibrary/ListSchedulingAlgorithm.h"
#include "../GraphLibrary/CriticalPathSchedulingAlgorithm.h"
#include "../GraphLibrary/CoffmanGrahamSchedulingAlgorithm.h"

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

// to try out features of the GraphLibrary
int main(int argc, char *argv[]){
	Graph graph = graph::creator::createQuadrant();
	Options options{ "Options.cfg" };
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
	}
	{
		std::cout << "genetic" << std::endl;
		GeneticAlgorithm alg{ std::make_unique<LengthFitnessStartegy>() };
		run(alg, graph, options);
	}
}
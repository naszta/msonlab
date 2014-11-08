#include "Graph.h"
#include "GraphExchanger.h"
#include "GraphCreator.h"
#include "SchedulingAlgorithm.h"
#include "Options.h"
#include "FitnessStrategy.h"
#include "SchedulingHelper.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include <xercesc\util\PlatformUtils.hpp>
#include <xercesc\dom\DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc\parsers\XercesDOMParser.hpp>
#include <xercesc\sax\HandlerBase.hpp>
XERCES_CPP_NAMESPACE_USE

#define MEASURE 1
#define WAIT 1

using namespace msonlab;
using namespace msonlab::scheduling;
using namespace msonlab::graph::creator;
using namespace std;

Graph initGraph()
{
	return createQuadrant();
}

Graph initTestGraph()
{
	return createTest();
}

Graph initSampleGraph()
{
	return createSample();
}

Graph initRandomGraph(const Options &options)
{
	return createRandom(options.getGraphSize(), options.getGraphEdgeProb(), options.getGraphWidening(), options.getNumberOfPus());
}

#include "lwgraph.h"
#include "NodeTest.h"

int main(int argc, char *argv[])
{
	/* initialize random seed: */
	srand(161803);

	// loading GA configuration
	OptionsPtr options = std::make_shared<const Options>("Options.cfg");
	//Options options{ "Options.cfg" };

	// get graph
	Graph graph = initRandomGraph(*options);
	//Graph graph = initGraph();

	// choosing algorithm
	SchedulingAlgorithmPtr alg = SchedulingAlgorithm::find_sceduling_algorithm(options);
#if MEASURE != 0	
	std::chrono::time_point<std::chrono::high_resolution_clock> startCHRONO, finishCHRONO;
	startCHRONO = std::chrono::high_resolution_clock::now();
#endif
	// the function that is measured
	SchedulingResultPtr best;
	try {
		if (alg != nullptr)
			best = alg->schedule(graph, *options);
		else
			std::cout << "Algorithm not found." << std::endl;
	}
	catch (const std::exception& ex) {
		std::cout << "Std exception occured: " << ex.what() << std::endl;
	}
	catch (...) {
		std::cout << "Unkown exception occured." << std::endl;
	}
#if MEASURE != 0
	finishCHRONO = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedCHRONO = finishCHRONO - startCHRONO;
#endif
	// check correctness
	//auto fs = FitnessStrategy::find_fitness_strategy("length");
	//bool correct = is_correct(*best);
	//cout << "Correct " << correct << endl;
	if (best != nullptr)
		std::cout << "Best length: " << best->fitness() << std::endl;
	else
		std:cout << "No result." << std::endl;
	/*if (!correct) {
		best->printSolution(std::cout);
		fs->fitness(*best, options);
	}

	unsigned l = computeLengthAndReuseIdleTime(*best, *options);
	std::cout << "Rescheduled length: " << l << std::endl;
	l = fs->fitness(*best, options);
	std::cout << "Recalculated Length: " << l << std::endl;*/
	cout << "Correct " << is_correct(*best) << endl;
#if MEASURE != 0
	std::cout << "Elapsed time " << std::setprecision(10) << elapsedCHRONO.count() << std::endl;
#endif
#if WAIT == 1
	std::cout << "Press a key to continue...";
	std::cin.get();
#endif

	return 0;
}


#include "Graph.h"
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

Graph initRandomGraph(Options::oPtr options)
{
	return createRandom(options->getGraphSize(), options->getGraphEdgeProb(), options->getGraphWidening(), options->getNumberOfPus());
}

int main(int argc, char *argv[])
{
	/* initialize random seed: */
	srand(161803);

	// loading GA configuration
	Options::oPtr options = std::make_shared<const Options>("Options.cfg");

	// get graph
	//Graph graph = initRandomGraph(options);
	auto graph = initGraph();

	// choosing algorithm
	SchedulingAlgorithm::ptr alg = SchedulingAlgorithm::find_sceduling_algorithm(options);

#if MEASURE != 0	
	std::chrono::time_point<std::chrono::high_resolution_clock> startCHRONO, finishCHRONO;
	startCHRONO = std::chrono::high_resolution_clock::now();
#endif
	// the function that is measured
	auto best = alg->schedule(graph, options);
#if MEASURE != 0
	finishCHRONO = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedCHRONO = finishCHRONO - startCHRONO;
#endif
	// check correctness
	auto fs = FitnessStrategy::find_fitness_strategy("length");
	bool correct = SchedulingHelper::is_correct(best);
	cout << "Correct " << correct << endl;
	std::cout << "Best length: " << best->getFitness() << std::endl;
	if (!correct) {
		fs->fitness(best, options);
	}

	unsigned l = SchedulingHelper::computeLengthAndReuseIdleTime(best, options);
	std::cout << "Rescheduled length: " << l << std::endl;
	l = fs->fitness(best, options);
	std::cout << "Recalculated Length: " << l << std::endl;
	cout << "Correct " << SchedulingHelper::is_correct(best) << endl;
#if MEASURE != 0
	std::cout << "Elapsed time " << std::setprecision(10) << elapsedCHRONO.count() << std::endl;
#endif
#if WAIT == 1
	std::cout << "Press a key to continue...";
	std::cin.get();
#endif

	return 0;
}


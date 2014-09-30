#include "Graph.h"
#include "GraphExchanger.h"
#include "StackCompiler.h"
#include "GraphCreator.h"
#include "SchedulingAlgorithm.h"
#include "Options.h"
#include "FitnessStrategy.h"
#include "SchedulingHelper.h"
#include "StackCompiler.h"
#include "StackRunner.h"
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
using namespace std;

Graph initGraph()
{
	return msonlab::graph::creator::createQuadrant();
}

Graph initTestGraph()
{
	return msonlab::graph::creator::createTest();
}

Graph initSampleGraph()
{
	return msonlab::graph::creator::createSample();
}

Graph initRandomGraph(Options::oPtr Options)
{
	return msonlab::graph::creator::createRandom(Options->getGraphSize(), Options->getGraphEdgeProb(), Options->getGraphWidening(), Options->getNumberOfPus());
}

//void test_small_graph(Options::oPtr options)
//{
//	const Graph&& g = initSampleGraph();
//
//	CriticalPathSchedulingAlgorithm alg;
//	auto result = alg.schedule(g, options);
//
//	vector<unsigned int> testOrder;
//	GeneticAlgorithm::transfromResult(result, testOrder);
//
//	auto sp1 = StackCompiler::getStackProgram(g, 2, testOrder);
//	sp1->print_out_programs();
//	sp1->run(sp1, 2);
//}

Solution::sPtr schedule(const Graph& graph, const SchedulingAlgorithm& alg, Options::oPtr options)
{
	auto best = alg.schedule(graph, options);
	cout << "Correct " << SchedulingHelper::is_correct(best) << endl;
	std::cout << "Best length: " << best->getFitness() << std::endl;
	//best->printTable(std::cout, options->getCommOverhead());
	unsigned l = SchedulingHelper::computeLengthAndReuseIdleTime(best, options);
	std::cout << "Rescheduled length: " << l << std::endl;
	auto fs = FitnessStrategy::find_fitness_strategy("length");
	l = fs->fitness(best, options);
	std::cout << "Length: " << l << std::endl;
	best->printTable(std::cout, options);
	cout << "Correct " << SchedulingHelper::is_correct(best) << endl;
	return best;
}

// used for running the algorithm with changing one parameter
void scheduleTest(const SchedulingAlgorithm& alg, Options::oPtr options, ofstream& resfile)
{
	//const Graph&& graph = initSampleGraph();
	const Graph&& graph = initRandomGraph(options);
	//const Graph&& graph = initGraph();
	auto best = alg.schedule(graph, options);
	resfile << ", preResult=" << best->getFitness();
	unsigned l = SchedulingHelper::computeLengthAndReuseIdleTime(best, options);
	resfile << ", result=" << l;
}

void doTest(SchedulingAlgorithm::ptr alg, Options::oPtr options)
{
	ofstream resfile;
	resfile.open("result.txt");
	for (unsigned i = 10; i <= 30; i += 10) {
		srand(161803);
		std::cout << "Testing " << i << std::endl;
		Options::oPtr opt = make_shared<const Options>(*options.get(), i);
		resfile << "{type=genetic, scheduleMutationRate=" << i;
		double elapsed = 0.0;
		std::chrono::time_point<std::chrono::high_resolution_clock> startCHRONO, finishCHRONO;
		startCHRONO = std::chrono::high_resolution_clock::now();
		scheduleTest(*alg.get(), opt, resfile); // pass alg by reference
		finishCHRONO = std::chrono::high_resolution_clock::now();
		std::chrono::duration<types::DataType> elapsedCHRONO = finishCHRONO - startCHRONO;
		elapsed = elapsedCHRONO.count();
		resfile << ", time=" << elapsed << "}\n";
	}
	resfile.close();
}

template <class T>
class Algo {
	T alg;
public:
	Algo(T a) : alg(a) {}
	Solution::sPtr schedule(Graph::gPtr& graph, Options::oPtr options) const {
		return alg.schedule(graph, options);
	}
};

struct holder {
	int i;
};

int main(int argc, char *argv[])
{
	/* initialize random seed: */
	srand(161803);

	// loading GA configuration
	Options::oPtr options = std::make_shared<const Options>("Options.cfg");

	//const Graph&& graph = graph::creator::createRandomLeveledDAG(options->getGraphSize(), options->getGraphWidening(), options->getGraphEdgeProb());
	Graph graph = initRandomGraph(options);
	GraphExchanger ge;
	try {
		ge.ExportGraph(graph, "leveled.graphml");
	}
	catch (...) {
		std::cerr << "Graph cannot be serialized.\n";
	}

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


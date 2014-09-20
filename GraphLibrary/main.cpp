#include "Graph.h"
#include "GraphExchanger.h"
#include "StackCompiler.h"
#include "Algorithms.h"
#include "NodeAdd.h"
#include "NodeConstant.h"
#include "NodeDivide.h"
#include "NodeMultiply.h"
#include "NodeSquareRoot.h"
#include "BlueEdge.h"
#include "BFSIterator.h"
#include "DFSIterator.h"
#include "GraphCreator.h"
#include "GreedySchedulingAlgorithm.h"
#include "GeneticAlgorithm.h"
#include "CriticalPathSchedulingAlgorithm.h"
#include "CoffmanGrahamSchedulingAlgorithm.h"
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

Graph::gPtr initGraph()
{
	return msonlab::graph::creator::createQuadrant();
}

Graph::gPtr initTestGraph()
{
	return msonlab::graph::creator::createTest();
}

Graph::gPtr initSampleGraph()
{
	return msonlab::graph::creator::createSample();
}

Graph::gPtr initRandomGraph(Options::oPtr Options)
{
	return msonlab::graph::creator::createRandom(Options->getGraphSize(), Options->getGraphEdgeProb(), Options->getGraphWidening(), Options->getNumberOfPus());
}

void test_small_graph(Options::oPtr options)
{
	Graph::gPtr g = initSampleGraph();

	CriticalPathSchedulingAlgorithm alg;
	auto result = alg.schedule(g, options);

	vector<unsigned int> testOrder;
	GeneticAlgorithm::transfromResult(result, testOrder);

	auto sp1 = StackCompiler::getStackProgram(g, 2, testOrder);
	sp1->print_out_programs();
	sp1->run(sp1, 2);
}

void runHusScheduling(Options::oPtr options)
{
	// choosing fitness strategy for the GA
	auto fsstrategy = FitnessStrategy::find_fitness_strategy("length");
	//FitnessStrategy::fsPtr fsstrategy(new LengthFitnessStartegy());

	CriticalPathSchedulingAlgorithm alg;

	// getting the graph
	//auto graph = initRandomGraph(Options);
	auto graph = initGraph();

	auto result = alg.schedule(graph, options);
	result->printTable(std::cout, options);
	unsigned fitness = fsstrategy->fitness(result, options);
	std::cout << fitness << std::endl;
}


Solution::sPtr schedule(Graph::gPtr& graph, SchedulingAlgorithm::algPtr alg, Options::oPtr options)
{
	auto best = alg->schedule(graph, options);
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
void scheduleTest(SchedulingAlgorithm::algPtr alg, Options::oPtr options, ofstream& resfile)
{
	//auto graph = initSampleGraph();
	auto graph = initRandomGraph(options);
	//auto graph = initGraph();
	auto best = alg->schedule(graph, options);
	resfile << ", preResult=" << best->getFitness();
	//best->printTable(std::cout, options->getCommOverhead());
	unsigned l = SchedulingHelper::computeLengthAndReuseIdleTime(best, options);
	resfile << ", result=" << l;
}

void doTest(SchedulingAlgorithm::algPtr alg, Options::oPtr options)
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
		scheduleTest(alg, opt, resfile);
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

int main(int argc, char *argv[])
{
	NodeAdd na (0, L"", make_shared<double>(2.3));
	auto nasp = make_shared<NodeAdd>(na);
	NodeAdd &nar = na;
	cout << "value " << sizeof(na) << endl;
	cout << "smart pointer " << sizeof(nasp) << endl;

	shared_ptr<GreedySchedulingAlgorithm> greedy = make_shared<GreedySchedulingAlgorithm>();
	GreedySchedulingAlgorithm greedyal;
	auto cp = make_shared<CriticalPathSchedulingAlgorithm>();
	CriticalPathSchedulingAlgorithm cpal;
	cout << "greedy value " << sizeof(greedyal) << endl;
	cout << "greedy pointer " << sizeof(greedy) << endl;
	cout << "cp value " << sizeof(cpal) << endl;
	cout << "cp pointer " << sizeof(cp) << endl;
	//auto genetic = std::make_shared<GeneticAlgorithm>();

	/*std::cin.get();

	return 0;*/
	/* initialize random seed: */
	srand(161803);

	// loading GA configuration
	Options::oPtr options = std::make_shared<const Options>("Options.cfg");

	//auto graph = graph::creator::createRandomLeveledDAG(options->getGraphSize(), options->getGraphWidening(), options->getGraphEdgeProb());
	auto graph = initRandomGraph(options);
	GraphExchanger ge;
	try {
		ge.ExportGraph(graph, "leveled.graphml");
	}
	catch (...) {
		std::cerr << "Graph cannot be serialized.\n";
	}

	// choosing algorithm
	SchedulingAlgorithm::algPtr alg;
	std::cout << "Using ";
	if (options->getAlgorithm().compare("genetic") == 0) {
		std::cout << "Genetic algorithm with " << options->getFitnessStrategy() << std::endl;
		auto fs = FitnessStrategy::find_fitness_strategy(options->getFitnessStrategy());
		if (fs == nullptr)
		{
			std::cerr << "Cannot find fitness strategy named " << options->getFitnessStrategy() << std::endl;
			// may throw an exception
			return 1;
		}

		alg = std::make_shared<GeneticAlgorithm>(options, fs);
		std::cout << "Genetic algorithm with " << options->getFitnessStrategy() << std::endl;
	}
	else if (options->getAlgorithm().compare("criticalPath") == 0) {
		std::cout << "Critical Path algorithm.\n";
		alg = std::make_shared<CriticalPathSchedulingAlgorithm>();
	}
	else if (options->getAlgorithm().compare("coffman") == 0) {
		std::cout << "CoffmanGraham algorithm.\n";
		alg = std::make_shared<CoffmanGrahamSchedulingAlgorithm>();
	}
	else {
		std::cout << "Greedy algorithm.\n";
		alg = std::make_shared<GreedySchedulingAlgorithm>();
	}

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


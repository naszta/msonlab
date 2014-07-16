#include "Graph.h"
#include "GraphExchanger.h"
#include "StackCompiler.h"

#include "NodeAdd.h"
#include "NodeConstant.h"
#include "NodeDivide.h"
#include "NodeMultiply.h"
#include "NodeSquareRoot.h"
#include "BlueEdge.h"
#include "BFSIterator.h"
#include "DFSIterator.h"
#include "GraphGenerator.h"
#include "GreedySchedulingAlgorithm.h"
#include "GeneticAlgorithm.h"
#include "HusSchedulingAlgorithm.h"
#include "Options.h"
#include "FitnessStrategy.h"
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
using namespace std;

Graph::gPtr initGraph()
{
	auto graph = make_unique<Graph>();
	msonlab::Node::nPtr a = (make_shared<msonlab::NodeConstant>(0, L"a", make_shared<Types::DataType>(4)));
	msonlab::Node::nPtr b = (make_shared<msonlab::NodeConstant>(1, L"b", make_shared<Types::DataType>(2)));
	msonlab::Node::nPtr c = (make_shared<msonlab::NodeConstant>(2, L"c", make_shared<Types::DataType>(-5)));

	msonlab::Node::nPtr constNumber_1 = (make_shared<msonlab::NodeConstant>(3, L"1", make_shared<Types::DataType>(1)));
	msonlab::Node::nPtr constNumber_2 = (make_shared<msonlab::NodeConstant>(4, L"2", make_shared<Types::DataType>(2)));
	msonlab::Node::nPtr constNumber_minus1 = (make_shared<msonlab::NodeConstant>(5, L"-1", make_shared<Types::DataType>(-1)));
	msonlab::Node::nPtr constNumber_minus4 = (make_shared<msonlab::NodeConstant>(6, L"-4", make_shared<Types::DataType>(-4)));

	msonlab::Node::nPtr multiply_2a(new msonlab::NodeMultiply(7, L"2a", make_shared<Types::DataType>(0)));
	msonlab::Node::nPtr multiply_bb(new msonlab::NodeMultiply(8, L"b^2", make_shared<Types::DataType>(0)));
	msonlab::Node::nPtr multiply_minusb(new msonlab::NodeMultiply(9, L"-b", make_shared<Types::DataType>(0)));
	msonlab::Node::nPtr multiply_minus4ac(new msonlab::NodeMultiply(10, L"-4ac", make_shared<Types::DataType>(0)));

	msonlab::Node::nPtr divide_1_2a(new msonlab::NodeDivide(11, L"1/2a", make_shared<Types::DataType>(0)));

	msonlab::Node::nPtr add_bb_minus4ac(new msonlab::NodeAdd(12, L"b^2-4ac", make_shared<Types::DataType>(0)));

	msonlab::Node::nPtr squareRoot_bb_minus4ac(new msonlab::NodeSquareRoot(13, L"(b^2-4ac)^0.5", make_shared<Types::DataType>(0)));

	msonlab::Node::nPtr multiply_minus_squareRoot_bb_minus4ac(new msonlab::NodeMultiply(14, L"-(b^2-4ac)^0.5", make_shared<Types::DataType>(0)));

	msonlab::Node::nPtr add_toDivide1(new msonlab::NodeAdd(15, L"-b+(b^2-4ac)^0.5", make_shared<Types::DataType>(0)));
	msonlab::Node::nPtr add_toDivide2(new msonlab::NodeAdd(16, L"-b-(b^2-4ac)^0.5", make_shared<Types::DataType>(0)));

	msonlab::Node::nPtr y1(new msonlab::NodeMultiply(17, L"Y1", make_shared<Types::DataType>(0)));
	msonlab::Node::nPtr y2(new msonlab::NodeMultiply(18, L"Y2", make_shared<Types::DataType>(0)));


	msonlab::Edge::ePtr e1 (new msonlab::BlueEdge(1, L"e1",0,a,multiply_2a));
	msonlab::Edge::ePtr e2 (new msonlab::BlueEdge(2, L"e2",0,constNumber_2,multiply_2a));
	msonlab::Edge::ePtr e3(new msonlab::BlueEdge(3, L"e3", 0, a, multiply_minus4ac));

	msonlab::Edge::ePtr e4 (new msonlab::BlueEdge(4,L"e4",0,b,multiply_bb));
	msonlab::Edge::ePtr e5 (new msonlab::BlueEdge(5,L"e5",0,b,multiply_bb));
	msonlab::Edge::ePtr e6 (new msonlab::BlueEdge(6,L"e6",0,b,multiply_minusb));
	msonlab::Edge::ePtr e7 (new msonlab::BlueEdge(7,L"e7",0,constNumber_minus1,multiply_minusb));

	msonlab::Edge::ePtr e8 (new msonlab::Edge(8,L"e8",0,c,multiply_minus4ac));
	msonlab::Edge::ePtr e9 (new msonlab::BlueEdge(9,L"e9",0,constNumber_minus4,multiply_minus4ac));

	msonlab::Edge::ePtr e10 (new msonlab::Edge(10,L"e10",0,constNumber_1,divide_1_2a));
	msonlab::Edge::ePtr e11 (new msonlab::Edge(11,L"e11",0,multiply_2a,divide_1_2a));

	msonlab::Edge::ePtr e12 (new msonlab::Edge(12,L"e12",0,multiply_bb,add_bb_minus4ac));
	msonlab::Edge::ePtr e13 (new msonlab::Edge(13,L"e13",0,multiply_minus4ac,add_bb_minus4ac));

	msonlab::Edge::ePtr e14 (new msonlab::Edge(14,L"e14",0,add_bb_minus4ac,squareRoot_bb_minus4ac));

	msonlab::Edge::ePtr e15 (new msonlab::BlueEdge(15,L"e15",0,squareRoot_bb_minus4ac,multiply_minus_squareRoot_bb_minus4ac));

	msonlab::Edge::ePtr e16 (new msonlab::Edge(16,L"e16",0,multiply_minusb,add_toDivide1));
	msonlab::Edge::ePtr e17 (new msonlab::Edge(17,L"e17",0,squareRoot_bb_minus4ac,add_toDivide1));

	msonlab::Edge::ePtr e18 (new msonlab::Edge(18,L"e18",0,multiply_minusb,add_toDivide2));
	msonlab::Edge::ePtr e19 (new msonlab::Edge(19,L"e19",0,multiply_minus_squareRoot_bb_minus4ac,add_toDivide2));

	msonlab::Edge::ePtr e20 (new msonlab::Edge(20,L"e20",0,add_toDivide1,y1));
	msonlab::Edge::ePtr e21 (new msonlab::Edge(21,L"e21",0,divide_1_2a,y1));

	msonlab::Edge::ePtr e22 (new msonlab::Edge(22,L"e22",0,add_toDivide2,y2));
	msonlab::Edge::ePtr e23 (new msonlab::Edge(23,L"e23",0,divide_1_2a,y2));


	graph->addEdge(e1);
	graph->addEdge(e2);
	graph->addEdge(e3);
	graph->addEdge(e4);
	graph->addEdge(e5);
	graph->addEdge(e6);
	graph->addEdge(e7);
	graph->addEdge(e8);
	graph->addEdge(e9);
	graph->addEdge(e10);
	graph->addEdge(e11);
	graph->addEdge(e12);
	graph->addEdge(e13);
	graph->addEdge(e14);
	graph->addEdge(e15);
	graph->addEdge(e16);
	graph->addEdge(e17);
	graph->addEdge(e18);
	graph->addEdge(e19);
	graph->addEdge(e20);
	graph->addEdge(e21);
	graph->addEdge(e22);
	graph->addEdge(e23);

	return graph;
}

Graph::gPtr initTestGraph()
{
	auto testG = make_unique<Graph>();

	msonlab::Node::nPtr node1 = make_shared<msonlab::Node>(0, L"0", make_shared<Types::DataType>(5), 2);
	msonlab::Node::nPtr node2 = make_shared<msonlab::Node>(1, L"1", make_shared<Types::DataType>(2), 3);
	msonlab::Node::nPtr node3 = make_shared<msonlab::Node>(2, L"2", make_shared<Types::DataType>(3), 3);
	msonlab::Node::nPtr node4 = make_shared<msonlab::Node>(3, L"3", make_shared<Types::DataType>(0), 4);
	msonlab::Node::nPtr node5 = make_shared<msonlab::Node>(4, L"4", make_shared<Types::DataType>(0), 5);
	msonlab::Node::nPtr node6 = make_shared<msonlab::Node>(5, L"5", make_shared<Types::DataType>(0), 4);
	msonlab::Node::nPtr node7 = make_shared<msonlab::Node>(6, L"6", make_shared<Types::DataType>(0), 4);
	msonlab::Node::nPtr node8 = make_shared<msonlab::Node>(7, L"7", make_shared<Types::DataType>(0), 4);
	msonlab::Node::nPtr node9 = make_shared<msonlab::Node>(8, L"8", make_shared<Types::DataType>(0), 1);

	msonlab::Edge::ePtr edge1 = make_shared<msonlab::Edge>(0, L"e12", nullptr, node1, node2);
	msonlab::Edge::ePtr edge2 = make_shared<msonlab::Edge>(1, L"e13", nullptr, node1, node3);
	msonlab::Edge::ePtr edge3 = make_shared<msonlab::Edge>(2, L"e14", nullptr, node1, node4);
	msonlab::Edge::ePtr edge4 = make_shared<msonlab::Edge>(3, L"e15", nullptr, node1, node5);
	msonlab::Edge::ePtr edge5 = make_shared<msonlab::Edge>(4, L"e17", nullptr, node1, node7);
	msonlab::Edge::ePtr edge6 = make_shared<msonlab::Edge>(6, L"e26", nullptr, node2, node6);
	msonlab::Edge::ePtr edge7 = make_shared<msonlab::Edge>(7, L"e27", nullptr, node2, node7);
	msonlab::Edge::ePtr edge8 = make_shared<msonlab::Edge>(8, L"e38", nullptr, node3, node8);
	msonlab::Edge::ePtr edge9 = make_shared<msonlab::Edge>(9, L"e48", nullptr, node4, node8);
	msonlab::Edge::ePtr edge10 = make_shared<msonlab::Edge>(10, L"e69", nullptr, node6, node9);
	msonlab::Edge::ePtr edge11 = make_shared<msonlab::Edge>(11, L"e79", nullptr, node7, node9);
	msonlab::Edge::ePtr edge12 = make_shared<msonlab::Edge>(12, L"e89", nullptr, node8, node9);

	testG->addEdge(edge1);
	testG->addEdge(edge2);
	testG->addEdge(edge3);
	testG->addEdge(edge4);
	testG->addEdge(edge5);
	testG->addEdge(edge6);
	testG->addEdge(edge7);
	testG->addEdge(edge8);
	testG->addEdge(edge9);
	testG->addEdge(edge10);
	testG->addEdge(edge11);
	testG->addEdge(edge12);
	return testG;
}

Graph::gPtr initSampleGraph()
{
	Graph::gPtr g = make_unique<Graph>();

	msonlab::Node::nPtr a(new msonlab::NodeConstant(0, L"a", make_shared<Types::DataType>(2)));
	msonlab::Node::nPtr b(new msonlab::NodeConstant(1, L"b", make_shared<Types::DataType>(3)));
	msonlab::Node::nPtr c(new msonlab::NodeConstant(2, L"c", make_shared<Types::DataType>(2)));
	msonlab::Node::nPtr d(new msonlab::NodeConstant(3, L"d", make_shared<Types::DataType>(3)));

	msonlab::Node::nPtr ab(new msonlab::NodeMultiply(4, L"a*b", nullptr));
	msonlab::Node::nPtr cd(new msonlab::NodeMultiply(5, L"c*d", nullptr));

	msonlab::Node::nPtr ab2(new msonlab::NodeMultiply(6, L"2*a*b", nullptr));
	msonlab::Node::nPtr c2(new msonlab::NodeConstant(7, L"2", make_shared<Types::DataType>(2)));

	msonlab::Node::nPtr res(new msonlab::NodeAdd(8, L"2*a*b + c*d", nullptr));

	msonlab::Edge::ePtr e1(new msonlab::Edge(9, L"e1", 0, a, ab));
	msonlab::Edge::ePtr e2(new msonlab::Edge(10, L"e2", 0, b, ab));

	msonlab::Edge::ePtr e3(new msonlab::Edge(11, L"e3", 0, c, cd));
	msonlab::Edge::ePtr e4(new msonlab::Edge(12, L"e4", 0, d, cd));

	msonlab::Edge::ePtr e5(new msonlab::Edge(13, L"e5", 0, ab, ab2));
	msonlab::Edge::ePtr e6(new msonlab::Edge(14, L"e6", 0, c2, ab2));

	msonlab::Edge::ePtr e7(new msonlab::Edge(15, L"e7", 0, ab2, res));

	msonlab::Edge::ePtr e8(new msonlab::Edge(16, L"e8", 0, cd, res));

	g->addEdge(e1);
	g->addEdge(e2);
	g->addEdge(e3);
	g->addEdge(e4);
	g->addEdge(e5);
	g->addEdge(e6);
	g->addEdge(e7);
	g->addEdge(e8);

	return g;
}

Graph::gPtr initRandomGraph(Options::oPtr Options)
{
	return GraphGenerator::generate(Options->getGraphSize(), Options->getGraphEdgeProb(), Options->getGraphWidening(), Options->getNumberOfPus());
}

Graph::gPtr initStackGraph()
{
	auto qeGraph = make_unique<Graph>();

	msonlab::Node::nPtr a = (make_shared<msonlab::NodeConstant>(1, L"a", make_shared<Types::DataType>(2)));
	msonlab::Node::nPtr b = (make_shared<msonlab::NodeConstant>(2, L"b", make_shared<Types::DataType>(4)));
	msonlab::Node::nPtr c = (make_shared<msonlab::NodeConstant>(3, L"c", make_shared<Types::DataType>(-4)));

	msonlab::Node::nPtr constNumber_1 = (make_shared<msonlab::NodeConstant>(4, L"1", make_shared<Types::DataType>(1.0)));
	msonlab::Node::nPtr constNumber_2 = (make_shared<msonlab::NodeConstant>(5, L"2", make_shared<Types::DataType>(2.0)));
	msonlab::Node::nPtr constNumber_minus1 = (make_shared<msonlab::NodeConstant>(6, L"-1", make_shared<Types::DataType>(-1.0)));
	msonlab::Node::nPtr constNumber_minus4 = (make_shared<msonlab::NodeConstant>(7, L"-4", make_shared<Types::DataType>(-4.0)));
	msonlab::Node::nPtr constNumber_05 = (make_shared<msonlab::NodeConstant>(20, L"0.5", make_shared<Types::DataType>(0.5)));

	msonlab::Node::nPtr multiply_2a(new msonlab::NodeMultiply(8, L"2a", make_shared<Types::DataType>(0.0)));
	msonlab::Node::nPtr multiply_bb(new msonlab::NodeMultiply(9, L"b^2", make_shared<Types::DataType>(0.0)));
	msonlab::Node::nPtr multiply_minusb(new msonlab::NodeMultiply(10, L"-b", make_shared<Types::DataType>(0.0)));
	msonlab::Node::nPtr multiply_minus4ac(new msonlab::NodeMultiply(11, L"-4ac", make_shared<Types::DataType>(0.0)));

	msonlab::Node::nPtr divide_1_2a(new msonlab::NodeDivide(12, L"1/2a", make_shared<Types::DataType>(0.0)));

	msonlab::Node::nPtr add_bb_minus4ac(new msonlab::NodeAdd(13, L"b^2-4ac", make_shared<Types::DataType>(0.0)));

	msonlab::Node::nPtr squareRoot_bb_minus4ac(new msonlab::NodeSquareRoot(14, L"(b^2-4ac)^0.5", make_shared<Types::DataType>(0.0)));

	msonlab::Node::nPtr add_toDivide1(new msonlab::NodeAdd(16, L"-b+(b^2-4ac)^0.5", make_shared<Types::DataType>(0.0)));

	msonlab::Node::nPtr y1(new msonlab::NodeMultiply(18, L"Y1", make_shared<Types::DataType>(0.0)));


	msonlab::Edge::ePtr e1(new msonlab::BlueEdge(1, L"e1", make_shared<Types::DataType>(0.0), a, multiply_2a));
	msonlab::Edge::ePtr e2(new msonlab::BlueEdge(2, L"e2", make_shared<Types::DataType>(0.0), constNumber_2, multiply_2a));
	msonlab::Edge::ePtr e3(new msonlab::BlueEdge(3, L"e3", make_shared<Types::DataType>(0.0), a, multiply_minus4ac));

	msonlab::Edge::ePtr e4(new msonlab::BlueEdge(4, L"e4", make_shared<Types::DataType>(0.0), b, multiply_bb));
	msonlab::Edge::ePtr e5(new msonlab::BlueEdge(5, L"e5", make_shared<Types::DataType>(0.0), b, multiply_bb));
	msonlab::Edge::ePtr e6(new msonlab::BlueEdge(6, L"e6", make_shared<Types::DataType>(0.0), b, multiply_minusb));
	msonlab::Edge::ePtr e7(new msonlab::BlueEdge(7, L"e7", make_shared<Types::DataType>(0.0), constNumber_minus1, multiply_minusb));

	msonlab::Edge::ePtr e8(new msonlab::Edge(8, L"e8", make_shared<Types::DataType>(0.0), c, multiply_minus4ac));
	msonlab::Edge::ePtr e9(new msonlab::BlueEdge(9, L"e9", make_shared<Types::DataType>(0.0), constNumber_minus4, multiply_minus4ac));

	msonlab::Edge::ePtr e10(new msonlab::Edge(10, L"e10", make_shared<Types::DataType>(0.0), constNumber_1, divide_1_2a));
	msonlab::Edge::ePtr e11(new msonlab::Edge(11, L"e11", make_shared<Types::DataType>(0.0), multiply_2a, divide_1_2a));

	msonlab::Edge::ePtr e12(new msonlab::Edge(12, L"e12", make_shared<Types::DataType>(0.0), multiply_bb, add_bb_minus4ac));
	msonlab::Edge::ePtr e13(new msonlab::Edge(13, L"e13", make_shared<Types::DataType>(0.0), multiply_minus4ac, add_bb_minus4ac));

	msonlab::Edge::ePtr e14(new msonlab::Edge(14, L"e14", make_shared<Types::DataType>(0.0), add_bb_minus4ac, squareRoot_bb_minus4ac));

	msonlab::Edge::ePtr e16(new msonlab::Edge(16, L"e16", make_shared<Types::DataType>(0.0), multiply_minusb, add_toDivide1));
	msonlab::Edge::ePtr e17(new msonlab::Edge(17, L"e17", make_shared<Types::DataType>(0.0), squareRoot_bb_minus4ac, add_toDivide1));


	msonlab::Edge::ePtr e20(new msonlab::Edge(20, L"e20", make_shared<Types::DataType>(0.0), add_toDivide1, y1));
	msonlab::Edge::ePtr e21(new msonlab::Edge(21, L"e21", make_shared<Types::DataType>(0.0), divide_1_2a, y1));


	msonlab::Edge::ePtr e24(new msonlab::Edge(24, L"e24", 0, constNumber_05, squareRoot_bb_minus4ac));


	qeGraph->addEdge(e1);
	qeGraph->addEdge(e2);
	qeGraph->addEdge(e3);
	qeGraph->addEdge(e4);
	qeGraph->addEdge(e5);
	qeGraph->addEdge(e6);
	qeGraph->addEdge(e7);
	qeGraph->addEdge(e8);
	qeGraph->addEdge(e9);
	qeGraph->addEdge(e10);
	qeGraph->addEdge(e11);
	qeGraph->addEdge(e12);
	qeGraph->addEdge(e13);
	qeGraph->addEdge(e14);
	qeGraph->addEdge(e16);
	qeGraph->addEdge(e17);
	qeGraph->addEdge(e20);
	qeGraph->addEdge(e21);
	qeGraph->addEdge(e24);

	return qeGraph;
}

Graph::gPtr initSampleGraph()
{
	Graph::gPtr graph(new Graph());
	
	Node::nPtr nc0(new NodeConstant(0, L"nc0", std::make_shared<Types::DataType>(1.0)));
	Node::nPtr nc1(new NodeConstant(1, L"nc1", std::make_shared<Types::DataType>(5.0)));
	Node::nPtr nc2(new NodeConstant(2, L"nc2", std::make_shared<Types::DataType>(3.0)));
	Node::nPtr nc3(new NodeConstant(3, L"nc3", std::make_shared<Types::DataType>(4.0)));
	Node::nPtr na(new NodeAdd(4, L"na", nullptr));
	Node::nPtr nm1(new NodeMultiply(5, L"nm1", nullptr));
	Node::nPtr nm2(new NodeMultiply(6, L"nm2", nullptr));

	Edge::ePtr e0(new Edge(0, L"e0", nullptr, nc0, na));
	Edge::ePtr e1(new Edge(1, L"e1", nullptr, nc1, na));
	Edge::ePtr e2(new Edge(2, L"e2", nullptr, nc2, na));
	Edge::ePtr e3(new Edge(3, L"e3", nullptr, na, nm2));
	Edge::ePtr e4(new Edge(4, L"e4", nullptr, nc2, nm1));
	Edge::ePtr e5(new Edge(5, L"e5", nullptr, nc3, nm1));
	Edge::ePtr e6(new Edge(6, L"e6", nullptr, nm1, nm2));

	graph->addEdge(e0);
	graph->addEdge(e1);
	graph->addEdge(e2);
	graph->addEdge(e3);
	graph->addEdge(e4);
	graph->addEdge(e5);
	graph->addEdge(e6);

	return graph;
}

void test_small_graph(Options::oPtr options)
{
	Graph::gPtr g = initSampleGraph();

	HusSchedulingAlgorithm alg;
	auto result = alg.schedule(g, options);

	vector<unsigned int> testOrder;
	GeneticAlgorithm::transfromResult(result, testOrder);

	// 0 - 9
	//testOrder.push_back(0);
	//testOrder.push_back(0);
	//testOrder.push_back(1);//
	//testOrder.push_back(1);//
	//testOrder.push_back(1);//
	//testOrder.push_back(1);//
	//testOrder.push_back(0);
	//testOrder.push_back(0);
	//testOrder.push_back(0);
	//testOrder.push_back(0);

	//// 10 - 16
	//testOrder.push_back(0);
	//testOrder.push_back(1);//
	//testOrder.push_back(1);//
	//testOrder.push_back(0);
	//testOrder.push_back(0);
	//testOrder.push_back(0);
	//testOrder.push_back(0);


	//GraphExchanger ge(g);
	//ge.ExportGraph("test.msg");
	//
	//Graph::gPtr imported = GraphExchanger::ImportGraph("test.msg");
	//GraphExchanger ge2(imported);
	//ge2.ExportGraph("test2.msg");

	auto sp1 = StackCompiler::getStackProgram(g, 2, testOrder);
	sp1->print_out_programs();
	sp1->run(sp1, 2);

	/*auto sp2 = StackCompiler::getStackProgram(imported, 2, testOrder);
	sp2->print_out_programs();
	*/
}

msonlab::GraphAlgorithms ga;

void runGA(Options::oPtr options)
{
	// choosing fitness strategy for the GA
	FitnessStrategy::fsPtr fsstrategy(new LengthFitnessStartegy());
	//FitnessStrategy::fsPtr fsstrategy(new PUUsageFitnessStrategy());

	// initializing GA
	GeneticAlgorithm gena(options, fsstrategy);

	// getting the graph
	auto graph = initRandomGraph(options);
	//auto graph = initGraph();

	GreedySchedulingAlgorithm greedyAlg;
	auto greedy = greedyAlg.schedule(graph, options);

	LengthFitnessStartegy lengtFS;
	std::cout << "Greedy length: " << lengtFS.fitness(greedy, options) << std::endl;
	std::cout << "Greedy fitness: " << fsstrategy->fitness(greedy, options) << std::endl;

	Population::pPtr population = gena.generateInitialSolution(graph, options);
	population->limit();

	unsigned last = population->best()->getFitness();
	unsigned bests_round = 0;

	for (size_t i = 0; i < options->getNumberOfYears(); ++i)
	{
		gena.simulateMating(population, options->getPopMaxSize(), false);
		population->limit();
		unsigned best = population->best()->getFitness();
		DEBUG("Generation " << i + 1);
		DEBUG("Best fitness: " << best);
		DEBUG("Avarage fitness: " << population->avarageFittness());
		if (best != last)
		{
			last = best;
			bests_round = i;
		}
	}

	auto best = population->best();
	best->printChromosome(std::cout);
	std::cout << "Best found in round " << bests_round << std::endl;
	DEBUG("Best fitness: " << best->getFitness());
	std::cout << "Best length: " << lengtFS.fitness(best, options) << std::endl;
	best->printTable(std::cout, options);
	std::vector<unsigned> result;
	gena.transfromResult(best, result);
	std::copy(result.begin(), result.end(), ostream_iterator<unsigned>(std::cout, " "));
}

void runHusScheduling(Options::oPtr options) 
{
	// choosing fitness strategy for the GA
	FitnessStrategy::fsPtr fsstrategy(new LengthFitnessStartegy());

	HusSchedulingAlgorithm alg;

	// getting the graph
	//auto graph = initRandomGraph(Options);
	auto graph = initGraph();

	auto result = alg.schedule(graph, options);
	result->printTable(std::cout, options);
	unsigned fitness = fsstrategy->fitness(result, options);
	std::cout << fitness << std::endl;
}

void schedule(SchedulingAlgorithm::algPtr alg, Options::oPtr options)
{
	//auto graph = initSampleGraph();
	auto graph = initRandomGraph(options);
	//auto graph = initGraph();
	auto best = alg->schedule(graph, options);
	std::cout << "Best length: " << best->getFitness() << std::endl;
	//best->printTable(std::cout, options->getCommOverhead());
	unsigned l = GraphAlgorithms::computeLengthAndReuseIdleTime(best, options);
	std::cout << "Rescheduled length: " << l << std::endl;
	LengthFitnessStartegy fs;
	l = fs.fitness(best, options);
	std::cout << "Length: " << l << std::endl;
	best->printTable(std::cout, options);
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
	unsigned l = GraphAlgorithms::computeLengthAndReuseIdleTime(best, options);
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
		std::chrono::duration<Types::DataType> elapsedCHRONO = finishCHRONO - startCHRONO;
		elapsed = elapsedCHRONO.count();
		resfile << ", time=" << elapsed << "}\n";
	}
	resfile.close();
			}

int main(int argc, char *argv[])
			{
	/* initialize random seed: */
	srand(161803);
	// loading GA configuration
	Options::oPtr options = std::make_shared<const Options>("Options.cfg");

	SchedulingAlgorithm::algPtr alg;
	std::cout << "Using ";
	if (options->getAlgorithm().compare("genetic") == 0) {
		std::cout << "Genetic";
		FitnessStrategy::fsPtr fs;
		if (options->getFitnessStrategy().compare("puUsage") == 0) {
			fs = std::make_shared<PUUsageFitnessStrategy>();
			std::cout << "Fitness set to pu usage.\n";
		}
		else if (options->getFitnessStrategy().compare("loadBalance") == 0) {
			fs = std::make_shared<LoadBalanceFitnessStrategy>();
			std::cout << "Fitness set to load balance.\n";
		}
		else if (options->getFitnessStrategy().compare("reschedule") == 0) {
			fs = std::make_shared<RescheduleIdleTimeFitnessStartegy>();
			std::cout << "Fitness set to reschedule idle time.\n";
		}
		else {
			fs = std::make_shared<LengthFitnessStartegy>();
			std::cout << "Fitness set to Length.\n";
		}

		alg = std::make_shared<GeneticAlgorithm>(options, fs);
	}
	else if (options->getAlgorithm().compare("criticalPath") == 0) {
		std::cout << "Critical Path";
		alg = std::make_shared<HusSchedulingAlgorithm>();
	}
	else {
		std::cout << "Greedy";
		alg = std::make_shared<GreedySchedulingAlgorithm>();
	}

	std::cout << " algorithm.\n";
	//doTest(alg, options);
#if MEASURE != 0
	double average = 0.0;
	for (int i = 0; i < MEASURE; ++i)
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> startCHRONO, finishCHRONO;
		startCHRONO = std::chrono::high_resolution_clock::now();
#endif
		test_small_graph(options);
		//schedule(alg, options);
#if MEASURE != 0
		finishCHRONO = std::chrono::high_resolution_clock::now();
		std::chrono::duration<Types::DataType> elapsedCHRONO = finishCHRONO - startCHRONO;
		average += elapsedCHRONO.count();
	}
	average /= MEASURE;
	std::cout << "Elapsed time " << std::setprecision(10) << average << std::endl;
#endif
#if WAIT == 1
	std::cout << "Press a key to continue...";
	std::cin.get();
#endif

	return 0;
}


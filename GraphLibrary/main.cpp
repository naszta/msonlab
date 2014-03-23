#include "Graph.h"
#include "NodeAdd.h"
#include "NodeConstant.h"
#include "NodeDivide.h"
#include "NodeMultiply.h"
#include "NodePower.h"
#include "NodeSquareRoot.h"
#include "BlueEdge.h"
#include "BFSIterator.h"
#include "DFSIterator.h"
#include "GraphAlgorithms.h"
#include "GraphGenerator.h"
#include "GeneticAlgorithm.h"
#include "GAOptions.h"
#include "FitnessStrategy.h"
#include "StackCompiler.h"
#include "StackRunner.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <locale>
#include <codecvt>

using namespace msonlab;
using namespace std;

msonlab::Graph::gPtr initGraph()
{
	msonlab::Graph::gPtr graph(new Graph());
	msonlab::Node::nPtr a(new msonlab::NodeConstant(0, L"a", msonlab::Types::DataType(new double(4))));
	msonlab::Node::nPtr b(new msonlab::NodeConstant(1, L"b", msonlab::Types::DataType(new double(2))));
	msonlab::Node::nPtr c(new msonlab::NodeConstant(2, L"c", msonlab::Types::DataType(new double(-5))));

	msonlab::Node::nPtr constNumber_1(new msonlab::NodeConstant(3, L"1", msonlab::Types::DataType(new double(1))));
	msonlab::Node::nPtr constNumber_2(new msonlab::NodeConstant(4, L"2", msonlab::Types::DataType(new double(2))));
	msonlab::Node::nPtr constNumber_minus1(new msonlab::NodeConstant(5, L"-1", msonlab::Types::DataType(new double(-1))));
	msonlab::Node::nPtr constNumber_minus4(new msonlab::NodeConstant(6, L"-4", msonlab::Types::DataType(new double(-4))));

	msonlab::Node::nPtr multiply_2a(new msonlab::NodeMultiply(7, L"2a", msonlab::Types::DataType(new double(0))));
	msonlab::Node::nPtr multiply_bb(new msonlab::NodeMultiply(8, L"b^2", msonlab::Types::DataType(new double(0))));
	msonlab::Node::nPtr multiply_minusb(new msonlab::NodeMultiply(9, L"-b", msonlab::Types::DataType(new double(0))));
	msonlab::Node::nPtr multiply_minus4ac(new msonlab::NodeMultiply(10, L"-4ac", msonlab::Types::DataType(new double(0))));

	msonlab::Node::nPtr divide_1_2a(new msonlab::NodeDivide(11, L"1/2a", msonlab::Types::DataType(new double(0))));

	msonlab::Node::nPtr add_bb_minus4ac(new msonlab::NodeAdd(12, L"b^2-4ac", msonlab::Types::DataType(new double(0))));

	msonlab::Node::nPtr squareRoot_bb_minus4ac(new msonlab::NodeSquareRoot(13, L"(b^2-4ac)^0.5", msonlab::Types::DataType(new double(0))));

	msonlab::Node::nPtr multiply_minus_squareRoot_bb_minus4ac(new msonlab::NodeMultiply(14, L"-(b^2-4ac)^0.5", msonlab::Types::DataType(new double(0))));

	msonlab::Node::nPtr add_toDivide1(new msonlab::NodeAdd(15, L"-b+(b^2-4ac)^0.5", msonlab::Types::DataType(new double(0))));
	msonlab::Node::nPtr add_toDivide2(new msonlab::NodeAdd(16, L"-b-(b^2-4ac)^0.5", msonlab::Types::DataType(new double(0))));

	msonlab::Node::nPtr y1(new msonlab::NodeMultiply(17, L"Y1", msonlab::Types::DataType(new double(0))));
	msonlab::Node::nPtr y2(new msonlab::NodeMultiply(18, L"Y2", msonlab::Types::DataType(new double(0))));


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

msonlab::Graph::gPtr initTestGraph()
{
	msonlab::Graph::gPtr testG(new Graph());

	msonlab::Node::nPtr node1(new msonlab::NodeConstant(0, L"5", msonlab::Types::DataType(new double(5))));
	msonlab::Node::nPtr node2(new msonlab::NodeConstant(1, L"2", msonlab::Types::DataType(new double(2))));
	msonlab::Node::nPtr node3(new msonlab::NodeConstant(2, L"3", msonlab::Types::DataType(new double(3))));
	msonlab::Node::nPtr node4(new msonlab::NodeAdd(3, L"*", msonlab::Types::DataType(new double(0))));
	msonlab::Node::nPtr node5(new msonlab::NodeMultiply(4, L"+", msonlab::Types::DataType(new double(0))));

	msonlab::Edge::ePtr edge1(new msonlab::Edge(0, L"e0", 0, node1, node5));
	msonlab::Edge::ePtr edge2(new msonlab::Edge(1, L"e1", 0, node2, node4));
	msonlab::Edge::ePtr edge3(new msonlab::Edge(2, L"e2", 0, node3, node4));
	msonlab::Edge::ePtr edge4(new msonlab::Edge(3, L"e3", 0, node4, node5));

	testG->addEdge(edge1);
	testG->addEdge(edge2);
	testG->addEdge(edge3);
	testG->addEdge(edge4);

	return testG;
}

msonlab::Graph::gPtr initRandomGraph(GAOptions::gaPtr gaoptions)
{
	return GraphGenerator::generate(gaoptions->getGraphSize(), gaoptions->getGraphEdgeProb(), gaoptions->getGraphWidening(), gaoptions->getNumberOfPus());
}

msonlab::Graph::gPtr initStackGraph()
{
	msonlab::Graph::gPtr qeGraph(new msonlab::Graph());

	msonlab::Node::nPtr a(new msonlab::NodeConstant(1, L"a", msonlab::Types::DataType(new double(2))));
	msonlab::Node::nPtr b(new msonlab::NodeConstant(2, L"b", msonlab::Types::DataType(new double(4))));
	msonlab::Node::nPtr c(new msonlab::NodeConstant(3, L"c", msonlab::Types::DataType(new double(-4))));

	msonlab::Node::nPtr constNumber_1(new msonlab::NodeConstant(4, L"1", msonlab::Types::DataType(new double(1.0))));
	msonlab::Node::nPtr constNumber_2(new msonlab::NodeConstant(5, L"2", msonlab::Types::DataType(new double(2.0))));
	msonlab::Node::nPtr constNumber_minus1(new msonlab::NodeConstant(6, L"-1", msonlab::Types::DataType(new double(-1.0))));
	msonlab::Node::nPtr constNumber_minus4(new msonlab::NodeConstant(7, L"-4", msonlab::Types::DataType(new double(-4.0))));
	msonlab::Node::nPtr constNumber_05(new msonlab::NodeConstant(20, L"0.5", msonlab::Types::DataType(new double(0.5))));

	msonlab::Node::nPtr multiply_2a(new msonlab::NodeMultiply(8, L"2a", msonlab::Types::DataType(new double(0.0))));
	msonlab::Node::nPtr multiply_bb(new msonlab::NodeMultiply(9, L"b^2", msonlab::Types::DataType(new double(0.0))));
	msonlab::Node::nPtr multiply_minusb(new msonlab::NodeMultiply(10, L"-b", msonlab::Types::DataType(new double(0.0))));
	msonlab::Node::nPtr multiply_minus4ac(new msonlab::NodeMultiply(11, L"-4ac", msonlab::Types::DataType(new double(0.0))));

	msonlab::Node::nPtr divide_1_2a(new msonlab::NodeDivide(12, L"1/2a", msonlab::Types::DataType(new double(0.0))));

	msonlab::Node::nPtr add_bb_minus4ac(new msonlab::NodeAdd(13, L"b^2-4ac", msonlab::Types::DataType(new double(0.0))));

	msonlab::Node::nPtr squareRoot_bb_minus4ac(new msonlab::NodePower(14, L"(b^2-4ac)^0.5", msonlab::Types::DataType(new double(0.0))));

	msonlab::Node::nPtr add_toDivide1(new msonlab::NodeAdd(16, L"-b+(b^2-4ac)^0.5", msonlab::Types::DataType(new double(0.0))));

	msonlab::Node::nPtr y1(new msonlab::NodeMultiply(18, L"Y1", msonlab::Types::DataType(new double(0.0))));


	msonlab::Edge::ePtr e1(new msonlab::BlueEdge(1, L"e1", msonlab::Types::DataType(new double(0.0)), a, multiply_2a));
	msonlab::Edge::ePtr e2(new msonlab::BlueEdge(2, L"e2", msonlab::Types::DataType(new double(0.0)), constNumber_2, multiply_2a));
	msonlab::Edge::ePtr e3(new msonlab::BlueEdge(3, L"e3", msonlab::Types::DataType(new double(0.0)), a, multiply_minus4ac));

	msonlab::Edge::ePtr e4(new msonlab::BlueEdge(4, L"e4", msonlab::Types::DataType(new double(0.0)), b, multiply_bb));
	msonlab::Edge::ePtr e5(new msonlab::BlueEdge(5, L"e5", msonlab::Types::DataType(new double(0.0)), b, multiply_bb));
	msonlab::Edge::ePtr e6(new msonlab::BlueEdge(6, L"e6", msonlab::Types::DataType(new double(0.0)), b, multiply_minusb));
	msonlab::Edge::ePtr e7(new msonlab::BlueEdge(7, L"e7", msonlab::Types::DataType(new double(0.0)), constNumber_minus1, multiply_minusb));

	msonlab::Edge::ePtr e8(new msonlab::Edge(8, L"e8", msonlab::Types::DataType(new double(0.0)), c, multiply_minus4ac));
	msonlab::Edge::ePtr e9(new msonlab::BlueEdge(9, L"e9", msonlab::Types::DataType(new double(0.0)), constNumber_minus4, multiply_minus4ac));

	msonlab::Edge::ePtr e10(new msonlab::Edge(10, L"e10", msonlab::Types::DataType(new double(0.0)), constNumber_1, divide_1_2a));
	msonlab::Edge::ePtr e11(new msonlab::Edge(11, L"e11", msonlab::Types::DataType(new double(0.0)), multiply_2a, divide_1_2a));

	msonlab::Edge::ePtr e12(new msonlab::Edge(12, L"e12", msonlab::Types::DataType(new double(0.0)), multiply_bb, add_bb_minus4ac));
	msonlab::Edge::ePtr e13(new msonlab::Edge(13, L"e13", msonlab::Types::DataType(new double(0.0)), multiply_minus4ac, add_bb_minus4ac));

	msonlab::Edge::ePtr e14(new msonlab::Edge(14, L"e14", msonlab::Types::DataType(new double(0.0)), add_bb_minus4ac, squareRoot_bb_minus4ac));

	msonlab::Edge::ePtr e16(new msonlab::Edge(16, L"e16", msonlab::Types::DataType(new double(0.0)), multiply_minusb, add_toDivide1));
	msonlab::Edge::ePtr e17(new msonlab::Edge(17, L"e17", msonlab::Types::DataType(new double(0.0)), squareRoot_bb_minus4ac, add_toDivide1));


	msonlab::Edge::ePtr e20(new msonlab::Edge(20, L"e20", msonlab::Types::DataType(new double(0.0)), add_toDivide1, y1));
	msonlab::Edge::ePtr e21(new msonlab::Edge(21, L"e21", msonlab::Types::DataType(new double(0.0)), divide_1_2a, y1));


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

void runCompile()
{
	Graph::gPtr graph = initStackGraph();
	msonlab::StackCompiler sc(graph);

	// CPU id� m�r�se
	clock_t startCPU, finishCPU;

	startCPU = clock();
	auto stackProg = sc.getStackProgram();
	finishCPU = clock();

	std::cout << "Printing complied program\n";
	std::cout << *stackProg << std::endl;

	clock_t timeCPU = (finishCPU - startCPU);
	double elapsedCPU = timeCPU;

	// CHRONO id� m�r�se

	std::chrono::high_resolution_clock clockCHRONO;
	std::chrono::time_point<std::chrono::high_resolution_clock> startCHRONO, finishCHRONO;

	startCHRONO = clockCHRONO.now();
	auto stackProg2 = sc.getStackProgram();
	finishCHRONO = clockCHRONO.now();

	double elapsedCHRONO = (finishCHRONO - startCHRONO).count();

	auto result = stackProg->run(1);

	std::cout << std::setprecision(100) << elapsedCHRONO << std::endl;
	std::cout << std::setprecision(100) << elapsedCPU;
	char ch;
	std::cin >> ch;
}

void runStackModel()
{
	const int runCount = 10000;

	msonlab::StackRunner runner;

	msonlab::Types::DataType a(new double(2.0));
	msonlab::Types::DataType b(new double(4.0));
	msonlab::Types::DataType c(new double(-4.0));

	msonlab::Types::DataType c1(new double(-1.0));
	msonlab::Types::DataType c2(new double(2));
	msonlab::Types::DataType c3(new double(4));
	msonlab::Types::DataType c4(new double(0.5));
	msonlab::Types::DataType c5(new double(2));

	runner.addToken(msonlab::StackRunner::PUSH, c1);
	runner.addToken(msonlab::StackRunner::PUSH, b);
	runner.addToken(msonlab::StackRunner::MUL, nullptr);
	runner.addToken(msonlab::StackRunner::PUSH, b);
	runner.addToken(msonlab::StackRunner::PUSH, c2);
	runner.addToken(msonlab::StackRunner::POW, nullptr);
	runner.addToken(msonlab::StackRunner::PUSH, c3);
	runner.addToken(msonlab::StackRunner::PUSH, a);
	runner.addToken(msonlab::StackRunner::PUSH, c);
	runner.addToken(msonlab::StackRunner::MUL, nullptr);
	runner.addToken(msonlab::StackRunner::MUL, nullptr);
	runner.addToken(msonlab::StackRunner::SUB, nullptr);
	runner.addToken(msonlab::StackRunner::PUSH, c4);
	runner.addToken(msonlab::StackRunner::POW, nullptr);
	runner.addToken(msonlab::StackRunner::ADD, nullptr);
	runner.addToken(msonlab::StackRunner::PUSH, c5);
	runner.addToken(msonlab::StackRunner::PUSH, a);
	runner.addToken(msonlab::StackRunner::MUL, nullptr);
	runner.addToken(msonlab::StackRunner::DIV, nullptr);

	// setup measure
	msonlab::Types::DataType ret = nullptr;

	// CHRONO id� m�r�se

	std::chrono::high_resolution_clock clockCHRONO;
	std::chrono::time_point<std::chrono::high_resolution_clock> startCHRONO, finishCHRONO;

	startCHRONO = clockCHRONO.now();
	ret = runner.run(runCount);
	finishCHRONO = clockCHRONO.now();

	double elapsedCHRONO = (finishCHRONO - startCHRONO).count();

	std::cout << std::setprecision(100) << elapsedCHRONO << std::endl;

	// CPU id� m�r�se

	clock_t startCPU, finishCPU;

	startCPU = clock();
	ret = runner.run(runCount);
	finishCPU = clock();

	clock_t timeCPU = (finishCPU - startCPU);
	double elapsedCPU = timeCPU;

	std::cout << std::setprecision(100) << elapsedCPU << std::endl;

	//	char ch;
	//	std::cin >> ch;

}

void runGA()
{
	msonlab::GraphAlgorithms ga;

	// loading GA configuration
	GAOptions::gaPtr gaoptions(new GAOptions("Options.cfg"));

	// choosing fitness strategy for the GA
	FitnessStrategy::fsPtr fsstrategy(new LengthFitnessStartegy());

	// initializing GA
	GeneticAlgorithm gena(gaoptions, fsstrategy);

	// getting the graph
	auto graph = initRandomGraph(gaoptions);

	int greedy = ga.scheduleGreedy(graph, gaoptions->getNumberOfPus());
	
	shared_ptr<Population> population = gena.generateInitialSolution(graph);
	population->limit();

	unsigned last = population->best()->getFitness();
	unsigned not_changed = 0;

	for (size_t i = 0; i < gaoptions->getNumberOfYears(); ++i)
	{
		gena.simulateMating(population, gaoptions->getPopMaxSize());
		population->limit();
		unsigned best = population->best()->getFitness();
		std::cout << "Generation " << i + 1 << std::endl;
		std::cout << "Best fitness: " << best << std::endl;
		std::cout << "Avarage fitness: " << population->avarageFittness() << std::endl;
		if (best == last)
		{
			++not_changed;
		}
		else
		{
			not_changed = 0;
			last = best;
		}
		if (i == gaoptions->getNumberOfYears() - 1 && not_changed < 10)
		{
			i -= 2 * not_changed;
		}
	}
}

int main(int argc, char *argv[])
{
	//runGA();
	runCompile();
	std::cout << "Press a key to continue...";
	std::cin.get();

	return 0;
}

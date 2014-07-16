#include "Graph.h"
#include "GraphExchanger.h"
#include "StackCompiler.h"

#include "NodeAdd.h"
#include "NodeConstant.h"
#include "NodeDivide.h"
#include "NodeMultiply.h"
#include "NodeSquareRoot.h"
#include "Edge.h"
#include "BlueEdge.h"
#include "BFSIterator.h"
#include "DFSIterator.h"
#include <fstream>
#include <ctime>
#include <cstdlib>

#include <xercesc\util\PlatformUtils.hpp>
#include <xercesc\dom\DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc\parsers\XercesDOMParser.hpp>
#include <xercesc\sax\HandlerBase.hpp>
XERCES_CPP_NAMESPACE_USE

using namespace msonlab;



void test_small_graph()
{
	Graph::gPtr g(new Graph());

	msonlab::Node::nPtr a(new msonlab::NodeConstant(0, "a", Types::DataType(new double(2))));
	msonlab::Node::nPtr b(new msonlab::NodeConstant(1, "b", Types::DataType(new double(3))));
	msonlab::Node::nPtr c(new msonlab::NodeConstant(2, "c", Types::DataType(new double(2))));
	msonlab::Node::nPtr d(new msonlab::NodeConstant(3, "d", Types::DataType(new double(3))));

	msonlab::Node::nPtr ab(new msonlab::NodeMultiply(4, "a*b", nullptr));
	msonlab::Node::nPtr cd(new msonlab::NodeMultiply(5, "c*d", nullptr));

	msonlab::Node::nPtr ab2(new msonlab::NodeMultiply(6, "2*a*b", nullptr));
	msonlab::Node::nPtr c2(new msonlab::NodeConstant(7, "2", Types::DataType(new double(2))));

	msonlab::Node::nPtr res(new msonlab::NodeAdd(8, "2*a*b + c*d", nullptr));

	msonlab::Edge::ePtr e1(new msonlab::Edge(9, "e1", 0, a, ab));
	msonlab::Edge::ePtr e2(new msonlab::Edge(10, "e2", 0, b, ab));

	msonlab::Edge::ePtr e3(new msonlab::Edge(11, "e3", 0, c, cd));
	msonlab::Edge::ePtr e4(new msonlab::Edge(12, "e4", 0, d, cd));

	msonlab::Edge::ePtr e5(new msonlab::Edge(13, "e5", 0, ab, ab2));
	msonlab::Edge::ePtr e6(new msonlab::Edge(14, "e6", 0, c2, ab2));

	msonlab::Edge::ePtr e7(new msonlab::Edge(15, "e7", 0, ab2, res));

	msonlab::Edge::ePtr e8(new msonlab::Edge(16, "e8", 0, cd, res));

	g->addEdge(e1);
	g->addEdge(e2);
	g->addEdge(e3);
	g->addEdge(e4);
	g->addEdge(e5);
	g->addEdge(e6);
	g->addEdge(e7);
	g->addEdge(e8);

	vector<unsigned int> testOrder;

	// 0 - 9
	testOrder.push_back(0);
	testOrder.push_back(0);
	testOrder.push_back(1);//
	testOrder.push_back(1);//
	testOrder.push_back(1);//
	testOrder.push_back(1);//
	testOrder.push_back(0);
	testOrder.push_back(0);
	testOrder.push_back(0);
	testOrder.push_back(0);

	// 10 - 16
	testOrder.push_back(0);
	testOrder.push_back(1);//
	testOrder.push_back(1);//
	testOrder.push_back(0);
	testOrder.push_back(0);
	testOrder.push_back(0);
	testOrder.push_back(0);


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

vector<unsigned int> getTestOrder()
{
	vector<unsigned int> ret;

	// 0 - 9 
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(1);	// node on t1
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(1);	// node on t1

	// 10 - 19
	ret.push_back(0);
	ret.push_back(1);	// node on t1
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(1);	// node on t1
	ret.push_back(0);
	ret.push_back(1);	// node on t1
	ret.push_back(0);
	ret.push_back(0);

	// 20 - 29
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(1);	// edge on t1
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);

	// 30 - 39
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(1);	// edge on t1
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(1);	// edge on t1

	// 40 - 42
	ret.push_back(1);	// edge on t1
	ret.push_back(0);
	ret.push_back(0);

	return ret;
}

void testQEGraph()
{
	Graph::gPtr qeGraph(new Graph());

	msonlab::Node::nPtr a(new msonlab::NodeConstant(0, "a", Types::DataType(new double(4))));
	msonlab::Node::nPtr b(new msonlab::NodeConstant(1, "b", Types::DataType(new double(2))));
	msonlab::Node::nPtr c(new msonlab::NodeConstant(2, "c", Types::DataType(new double(-5))));

	msonlab::Node::nPtr constNumber_1(new msonlab::NodeConstant(3, "1", Types::DataType(new double(1))));
	msonlab::Node::nPtr constNumber_2(new msonlab::NodeConstant(4, "2", Types::DataType(new double(2))));
	msonlab::Node::nPtr constNumber_minus1(new msonlab::NodeConstant(5, "-1", Types::DataType(new double(-1))));
	msonlab::Node::nPtr constNumber_minus4(new msonlab::NodeConstant(6, "-4", Types::DataType(new double(-4))));

	msonlab::Node::nPtr multiply_2a(new msonlab::NodeMultiply(7, "2a", 0));
	msonlab::Node::nPtr multiply_bb(new msonlab::NodeMultiply(8, "b^2", 0));
	msonlab::Node::nPtr multiply_minusb(new msonlab::NodeMultiply(9, "-b", 0));
	msonlab::Node::nPtr multiply_minus4ac(new msonlab::NodeMultiply(10, "-4ac", 0));

	msonlab::Node::nPtr divide_1_2a(new msonlab::NodeDivide(11, "1/2a", 0));

	msonlab::Node::nPtr add_bb_minus4ac(new msonlab::NodeAdd(12, "b^2-4ac", 0));

	msonlab::Node::nPtr squareRoot_bb_minus4ac(new msonlab::NodeSquareRoot(13, "(b^2-4ac)^0.5", 0));

	msonlab::Node::nPtr multiply_minus_squareRoot_bb_minus4ac(new msonlab::NodeMultiply(14, "-(b^2-4ac)^0.5", 0));

	msonlab::Node::nPtr add_toDivide1(new msonlab::NodeAdd(15, "-b+(b^2-4ac)^0.5", 0));
	msonlab::Node::nPtr add_toDivide2(new msonlab::NodeAdd(16, "-b-(b^2-4ac)^0.5", 0));

	msonlab::Node::nPtr y1(new msonlab::NodeMultiply(17, "Y1", 0));
	msonlab::Node::nPtr y2(new msonlab::NodeMultiply(18, "Y2", 0));


	msonlab::Edge::ePtr e1(new msonlab::Edge(19, "e1", 0, a, multiply_2a));
	msonlab::Edge::ePtr e2(new msonlab::Edge(20, "e2", 0, constNumber_2, multiply_2a));
	msonlab::Edge::ePtr e3(new msonlab::Edge(21, "e3", 0, a, multiply_minus4ac));

	msonlab::Edge::ePtr e4(new msonlab::Edge(22, "e4", 0, b, multiply_bb));
	msonlab::Edge::ePtr e5(new msonlab::Edge(23, "e5", 0, b, multiply_bb));
	msonlab::Edge::ePtr e6(new msonlab::Edge(24, "e6", 0, b, multiply_minusb));
	msonlab::Edge::ePtr e7(new msonlab::Edge(25, "e7", 0, constNumber_minus1, multiply_minusb));

	msonlab::Edge::ePtr e8(new msonlab::Edge(26, "e8", 0, c, multiply_minus4ac));
	msonlab::Edge::ePtr e9(new msonlab::Edge(27, "e9", 0, constNumber_minus4, multiply_minus4ac));

	msonlab::Edge::ePtr e10(new msonlab::BlueEdge(28, "e10", 0, constNumber_1, divide_1_2a));
	msonlab::Edge::ePtr e11(new msonlab::BlueEdge(29, "e11", 0, multiply_2a, divide_1_2a));

	msonlab::Edge::ePtr e12(new msonlab::BlueEdge(30, "e12", 0, multiply_bb, add_bb_minus4ac));
	msonlab::Edge::ePtr e13(new msonlab::BlueEdge(31, "e13", 0, multiply_minus4ac, add_bb_minus4ac));

	msonlab::Edge::ePtr e14(new msonlab::BlueEdge(32, "e14", 0, add_bb_minus4ac, squareRoot_bb_minus4ac));

	msonlab::Edge::ePtr e15(new msonlab::Edge(33, "e15", 0, squareRoot_bb_minus4ac, multiply_minus_squareRoot_bb_minus4ac));
	msonlab::Edge::ePtr e24(new msonlab::Edge(34, "e24", 0, constNumber_minus1, multiply_minus_squareRoot_bb_minus4ac));

	msonlab::Edge::ePtr e16(new msonlab::Edge(35, "e16", 0, multiply_minusb, add_toDivide1));
	msonlab::Edge::ePtr e17(new msonlab::Edge(36, "e17", 0, squareRoot_bb_minus4ac, add_toDivide1));

	msonlab::Edge::ePtr e18(new msonlab::Edge(37, "e18", 0, multiply_minusb, add_toDivide2));
	msonlab::Edge::ePtr e19(new msonlab::Edge(38, "e19", 0, multiply_minus_squareRoot_bb_minus4ac, add_toDivide2));

	msonlab::Edge::ePtr e20(new msonlab::Edge(39, "e20", 0, add_toDivide1, y1));
	msonlab::Edge::ePtr e21(new msonlab::Edge(40, "e21", 0, divide_1_2a, y1));

	msonlab::Edge::ePtr e22(new msonlab::Edge(41, "e22", 0, add_toDivide2, y2));
	msonlab::Edge::ePtr e23(new msonlab::Edge(42, "e23", 0, divide_1_2a, y2));


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
	qeGraph->addEdge(e15);
	qeGraph->addEdge(e16);
	qeGraph->addEdge(e17);
	qeGraph->addEdge(e18);
	qeGraph->addEdge(e19);
	qeGraph->addEdge(e20);
	qeGraph->addEdge(e21);
	qeGraph->addEdge(e22);
	qeGraph->addEdge(e23);
	qeGraph->addEdge(e24);



	//GraphExchanger ge(qeGraph);
	//ge.ExportGraph("test.msg");

	//Graph::gPtr imported = GraphExchanger::ImportGraph("test.msg");
	//GraphExchanger ge2(imported);
	//ge2.ExportGraph("test2.msg");


	auto sp1 = StackCompiler::getStackProgram(qeGraph, 2, getTestOrder());
	sp1->print_out_programs();
	sp1->run(sp1, 1);



	//StackCompiler sc2(imported);
	//auto sp2 = sc.getStackProgram(2, getTestOrder());
	//sp2->print_out_programs();


}




int main()
{

	//test_small_graph();

	testQEGraph();




	//Graph::gPtr imported = GraphExchanger::ImportGraph("testing.graphml");

	//vector<unsigned int> order;
	//for (size_t i = 0; i < imported->getEdges().size() + imported->getNodes().size(); i++)
	//{
	//	if (i == 0)
	//	{
	//		order.push_back(1);
	//	}
	//	else
	//	{
	//		order.push_back(0);
	//	}
	//}

	//auto sp1 = StackCompiler::getStackProgram(imported, 2, order);
	//sp1->print_out_programs();
	//sp1->run(sp1, 2);


	std::cout << "Press a key to continue...";
	std::cin.get();

	return 0;
}






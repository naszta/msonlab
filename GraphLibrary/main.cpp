#include "Graph.h"

#include "NodeAdd.h"
#include "NodeConstant.h"
#include "NodeDivide.h"
#include "NodeMultiply.h"
#include "NodeSquareRoot.h"
#include "BFSIterator.h"
#include "GraphAlgorithms.h"

#include <fstream>

void main(void)
{
	msonlab::Graph qeGraph;

	msonlab::Node::nPtr a (new msonlab::NodeConstant(1,L'a',4));
	msonlab::Node::nPtr b (new msonlab::NodeConstant(2,L'b',2));
	msonlab::Node::nPtr c (new msonlab::NodeConstant(3,L'c',-5));
	
	msonlab::Node::nPtr constNumber_1 (new msonlab::NodeConstant(4,L'1',1));
	msonlab::Node::nPtr constNumber_2 (new msonlab::NodeConstant(5,L'2',2));
	msonlab::Node::nPtr constNumber_minus1 (new msonlab::NodeConstant(6,L'-1',-1));
	msonlab::Node::nPtr constNumber_minus4 (new msonlab::NodeConstant(7,L'-4',-4));

	msonlab::Node::nPtr multiply_2a (new msonlab::NodeMultiply(8,L'2a',0));
	msonlab::Node::nPtr multiply_bb (new msonlab::NodeMultiply(9,L'b^2',0));
	msonlab::Node::nPtr multiply_minusb (new msonlab::NodeMultiply(10,L'-b',0));
	msonlab::Node::nPtr multiply_minus4ac (new msonlab::NodeMultiply(11,L'-4ac',0));

	msonlab::Node::nPtr divide_1_2a (new msonlab::NodeDivide(12,L'1/2a',0));

	msonlab::Node::nPtr add_bb_minus4ac (new msonlab::NodeAdd(13,L'b^2-4ac',0));

	msonlab::Node::nPtr squareRoot_bb_minus4ac (new msonlab::NodeSquareRoot(14,L'(b^2-4ac)^0.5',0));

	msonlab::Node::nPtr multiply_minus_squareRoot_bb_minus4ac (new msonlab::NodeMultiply(15,L'-(b^2-4ac)^0.5',0));

	msonlab::Node::nPtr add_toDivide1 (new msonlab::NodeAdd(16,L'-b+(b^2-4ac)^0.5',0));
	msonlab::Node::nPtr add_toDivide2 (new msonlab::NodeAdd(17,L'-b-(b^2-4ac)^0.5',0));

	msonlab::Node::nPtr y1 (new msonlab::NodeMultiply(18,L'Y1',0));
	msonlab::Node::nPtr y2 (new msonlab::NodeMultiply(19,L'Y2',0));


	msonlab::Edge::ePtr e1 (new msonlab::Edge(1,L'e1',0,a,multiply_2a));
	msonlab::Edge::ePtr e2 (new msonlab::Edge(2,L'e2',0,constNumber_2,multiply_2a));
	msonlab::Edge::ePtr e3 (new msonlab::Edge(3,L'e3',0,a,multiply_minus4ac));

	msonlab::Edge::ePtr e4 (new msonlab::Edge(4,L'e4',0,b,multiply_bb));
	msonlab::Edge::ePtr e5 (new msonlab::Edge(5,L'e5',0,b,multiply_bb));
	msonlab::Edge::ePtr e6 (new msonlab::Edge(6,L'e6',0,b,multiply_minusb));
	msonlab::Edge::ePtr e7 (new msonlab::Edge(7,L'e7',0,constNumber_minus1,multiply_minusb));

	msonlab::Edge::ePtr e8 (new msonlab::Edge(8,L'e8',0,c,multiply_minus4ac));
	msonlab::Edge::ePtr e9 (new msonlab::Edge(9,L'e9',0,constNumber_minus4,multiply_minus4ac));

	msonlab::Edge::ePtr e10 (new msonlab::Edge(10,L'e10',0,constNumber_1,divide_1_2a));
	msonlab::Edge::ePtr e11 (new msonlab::Edge(11,L'e11',0,multiply_2a,divide_1_2a));

	msonlab::Edge::ePtr e12 (new msonlab::Edge(12,L'e12',0,multiply_bb,add_bb_minus4ac));
	msonlab::Edge::ePtr e13 (new msonlab::Edge(13,L'e13',0,multiply_minus4ac,add_bb_minus4ac));

	msonlab::Edge::ePtr e14 (new msonlab::Edge(14,L'e14',0,add_bb_minus4ac,squareRoot_bb_minus4ac));

	msonlab::Edge::ePtr e15 (new msonlab::Edge(15,L'e15',0,squareRoot_bb_minus4ac,multiply_minus_squareRoot_bb_minus4ac));

	msonlab::Edge::ePtr e16 (new msonlab::Edge(16,L'e16',0,multiply_minusb,add_toDivide1));
	msonlab::Edge::ePtr e17 (new msonlab::Edge(17,L'e17',0,squareRoot_bb_minus4ac,add_toDivide1));

	msonlab::Edge::ePtr e18 (new msonlab::Edge(18,L'e18',0,multiply_minusb,add_toDivide2));
	msonlab::Edge::ePtr e19 (new msonlab::Edge(19,L'e19',0,multiply_minus_squareRoot_bb_minus4ac,add_toDivide2));

	msonlab::Edge::ePtr e20 (new msonlab::Edge(20,L'e20',0,add_toDivide1,y1));
	msonlab::Edge::ePtr e21 (new msonlab::Edge(21,L'e21',0,divide_1_2a,y1));

	msonlab::Edge::ePtr e22 (new msonlab::Edge(22,L'e22',0,add_toDivide2,y2));
	msonlab::Edge::ePtr e23 (new msonlab::Edge(23,L'e23',0,divide_1_2a,y2));


	qeGraph.addEdge(e1);
	qeGraph.addEdge(e2);
	qeGraph.addEdge(e3);
	qeGraph.addEdge(e4);
	qeGraph.addEdge(e5);
	qeGraph.addEdge(e6);
	qeGraph.addEdge(e7);
	qeGraph.addEdge(e8);
	qeGraph.addEdge(e9);
	qeGraph.addEdge(e10);
	qeGraph.addEdge(e11);
	qeGraph.addEdge(e12);
	qeGraph.addEdge(e13);
	qeGraph.addEdge(e14);
	qeGraph.addEdge(e15);
	qeGraph.addEdge(e16);
	qeGraph.addEdge(e17);
	qeGraph.addEdge(e18);
	qeGraph.addEdge(e19);
	qeGraph.addEdge(e20);
	qeGraph.addEdge(e21);
	qeGraph.addEdge(e22);
	qeGraph.addEdge(e23);


	msonlab::Node::nVect inputs = qeGraph.getInputNodes();
	msonlab::Node::nVect outputs = qeGraph.getOutputNodes();

	msonlab::Graph::gPtr ptrGraph (new msonlab::Graph(qeGraph));
	msonlab::BFSIterator itr (ptrGraph);
	itr.setStartNode(add_bb_minus4ac);
	for(;itr.hasMoreNode(); ++itr)
	{
		std::cout << (*itr)->getLabel() << " " << (*itr)->getId() << std::endl;
	}

	msonlab::IProcessable::nSet changed;
	changed.insert(constNumber_2);
	changed.insert(add_bb_minus4ac);
	msonlab::IProcessable::nSet needed;
	needed.insert(y1);
	needed.insert(multiply_minus_squareRoot_bb_minus4ac);
	msonlab::GraphAlgorithms ga;
	msonlab::Graph::gPtr changedGraph = ga.getChangedGraph(ptrGraph, changed, needed);

	std::ofstream file;
	file.open("graph.gv");

	changedGraph->exportGraph(file);

	file.close();

	std::cout << "Press a key to continue...";
	char ch;
	std::cin >> ch;

	return;
}

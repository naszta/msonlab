#include "Graph.h"

#include "NodeAdd.h"
#include "NodeConstant.h"
#include "NodeDivide.h"
#include "NodeMultiply.h"
#include "NodeSquareRoot.h"
#include "Edge.h"
#include "BlueEdge.h"
#include "BFSIterator.h"
#include "DFSIterator.h"
#include "GraphAlgorithms.h"
#include "GraphGenerator.h"
#include "GeneticAlgorithm.h"
#include "GAOptions.h"
#include "FitnessStrategy.h"
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace msonlab;

int main(int argc, char *argv[])
{
	//msonlab::Graph qeGraph;

	//msonlab::Node::nPtr a (new msonlab::NodeConstant(0,L'a',4));
	//msonlab::Node::nPtr b (new msonlab::NodeConstant(1,L'b',2));
	//msonlab::Node::nPtr c (new msonlab::NodeConstant(2,L'c',-5));

	//msonlab::Node::nPtr constNumber_1 (new msonlab::NodeConstant(3,L'1',1));
	//msonlab::Node::nPtr constNumber_2 (new msonlab::NodeConstant(4,L'2',2));
	//msonlab::Node::nPtr constNumber_minus1 (new msonlab::NodeConstant(5,L'-1',-1));
	//msonlab::Node::nPtr constNumber_minus4 (new msonlab::NodeConstant(6,L'-4',-4));

	//msonlab::Node::nPtr multiply_2a (new msonlab::NodeMultiply(7,L'2a',0));
	//msonlab::Node::nPtr multiply_bb (new msonlab::NodeMultiply(8,L'b^2',0));
	//msonlab::Node::nPtr multiply_minusb (new msonlab::NodeMultiply(9,L'-b',0));
	//msonlab::Node::nPtr multiply_minus4ac (new msonlab::NodeMultiply(10,L'-4ac',0));

	//msonlab::Node::nPtr divide_1_2a (new msonlab::NodeDivide(11,L'1/2a',0));

	//msonlab::Node::nPtr add_bb_minus4ac (new msonlab::NodeAdd(12,L'b^2-4ac',0));

	//msonlab::Node::nPtr squareRoot_bb_minus4ac (new msonlab::NodeSquareRoot(13,L'(b^2-4ac)^0.5',0));

	//msonlab::Node::nPtr multiply_minus_squareRoot_bb_minus4ac (new msonlab::NodeMultiply(14,L'-(b^2-4ac)^0.5',0));

	//msonlab::Node::nPtr add_toDivide1 (new msonlab::NodeAdd(15,L'-b+(b^2-4ac)^0.5',0));
	//msonlab::Node::nPtr add_toDivide2 (new msonlab::NodeAdd(16,L'-b-(b^2-4ac)^0.5',0));

	//msonlab::Node::nPtr y1 (new msonlab::NodeMultiply(17,L'Y1',0));
	//msonlab::Node::nPtr y2 (new msonlab::NodeMultiply(18,L'Y2',0));


	//msonlab::Edge::ePtr e1 (new msonlab::BlueEdge(1,L'e1',0,a,multiply_2a));
	//msonlab::Edge::ePtr e2 (new msonlab::BlueEdge(2,L'e2',0,constNumber_2,multiply_2a));
	//msonlab::Edge::ePtr e3 (new msonlab::BlueEdge(3,L'e3',0,a,multiply_minus4ac));

	//msonlab::Edge::ePtr e4 (new msonlab::BlueEdge(4,L'e4',0,b,multiply_bb));
	//msonlab::Edge::ePtr e5 (new msonlab::BlueEdge(5,L'e5',0,b,multiply_bb));
	//msonlab::Edge::ePtr e6 (new msonlab::BlueEdge(6,L'e6',0,b,multiply_minusb));
	//msonlab::Edge::ePtr e7 (new msonlab::BlueEdge(7,L'e7',0,constNumber_minus1,multiply_minusb));

	//msonlab::Edge::ePtr e8 (new msonlab::Edge(8,L'e8',0,c,multiply_minus4ac));
	//msonlab::Edge::ePtr e9 (new msonlab::BlueEdge(9,L'e9',0,constNumber_minus4,multiply_minus4ac));

	//msonlab::Edge::ePtr e10 (new msonlab::Edge(10,L'e10',0,constNumber_1,divide_1_2a));
	//msonlab::Edge::ePtr e11 (new msonlab::Edge(11,L'e11',0,multiply_2a,divide_1_2a));

	//msonlab::Edge::ePtr e12 (new msonlab::Edge(12,L'e12',0,multiply_bb,add_bb_minus4ac));
	//msonlab::Edge::ePtr e13 (new msonlab::Edge(13,L'e13',0,multiply_minus4ac,add_bb_minus4ac));

	//msonlab::Edge::ePtr e14 (new msonlab::Edge(14,L'e14',0,add_bb_minus4ac,squareRoot_bb_minus4ac));

	//msonlab::Edge::ePtr e15 (new msonlab::BlueEdge(15,L'e15',0,squareRoot_bb_minus4ac,multiply_minus_squareRoot_bb_minus4ac));

	//msonlab::Edge::ePtr e16 (new msonlab::Edge(16,L'e16',0,multiply_minusb,add_toDivide1));
	//msonlab::Edge::ePtr e17 (new msonlab::Edge(17,L'e17',0,squareRoot_bb_minus4ac,add_toDivide1));

	//msonlab::Edge::ePtr e18 (new msonlab::Edge(18,L'e18',0,multiply_minusb,add_toDivide2));
	//msonlab::Edge::ePtr e19 (new msonlab::Edge(19,L'e19',0,multiply_minus_squareRoot_bb_minus4ac,add_toDivide2));

	//msonlab::Edge::ePtr e20 (new msonlab::Edge(20,L'e20',0,add_toDivide1,y1));
	//msonlab::Edge::ePtr e21 (new msonlab::Edge(21,L'e21',0,divide_1_2a,y1));

	//msonlab::Edge::ePtr e22 (new msonlab::Edge(22,L'e22',0,add_toDivide2,y2));
	//msonlab::Edge::ePtr e23 (new msonlab::Edge(23,L'e23',0,divide_1_2a,y2));


	//qeGraph.addEdge(e1);
	//qeGraph.addEdge(e2);
	//qeGraph.addEdge(e3);
	//qeGraph.addEdge(e4);
	//qeGraph.addEdge(e5);
	//qeGraph.addEdge(e6);
	//qeGraph.addEdge(e7);
	//qeGraph.addEdge(e8);
	//qeGraph.addEdge(e9);
	//qeGraph.addEdge(e10);
	//qeGraph.addEdge(e11);
	//qeGraph.addEdge(e12);
	//qeGraph.addEdge(e13);
	//qeGraph.addEdge(e14);
	//qeGraph.addEdge(e15);
	//qeGraph.addEdge(e16);
	//qeGraph.addEdge(e17);
	//qeGraph.addEdge(e18);
	//qeGraph.addEdge(e19);
	//qeGraph.addEdge(e20);
	//qeGraph.addEdge(e21);
	//qeGraph.addEdge(e22);
	//qeGraph.addEdge(e23);

	
	//msonlab::Graph::gPtr testG (new msonlab::Graph(qeGraph));

	//msonlab::Node::nPtr node1 (new msonlab::NodeConstant(0,L'1',1));
	//msonlab::Node::nPtr node2 (new msonlab::NodeConstant(1,L'1',1));
	//msonlab::Node::nPtr node3 (new msonlab::NodeConstant(2,L'1',1));
	//msonlab::Node::nPtr node4 (new msonlab::NodeConstant(3,L'1',1));
	//msonlab::Node::nPtr node5 (new msonlab::NodeConstant(4,L'1',1));
	//msonlab::Node::nPtr node6 (new msonlab::NodeConstant(5,L'1',1));
	//msonlab::Node::nPtr node7 (new msonlab::NodeConstant(6,L'1',1));

	//msonlab::Edge::ePtr edge1 (new msonlab::Edge(0,L'e1',0,node1,node2));
	//msonlab::Edge::ePtr edge2 (new msonlab::Edge(1,L'e1',0,node1,node3));
	//msonlab::Edge::ePtr edge3 (new msonlab::Edge(2,L'e1',0,node1,node4));
	//msonlab::Edge::ePtr edge4 (new msonlab::Edge(3,L'e1',0,node2,node5));
	//msonlab::Edge::ePtr edge5 (new msonlab::Edge(4,L'e1',0,node2,node6));
	//msonlab::Edge::ePtr edge6 (new msonlab::Edge(5,L'e1',0,node3,node6));
	//msonlab::Edge::ePtr edge7 (new msonlab::Edge(6,L'e1',0,node3,node7));
	//msonlab::Edge::ePtr edge8 (new msonlab::Edge(7,L'e1',0,node4,node7));
	//msonlab::Edge::ePtr edge9 (new msonlab::Edge(8,L'e1',0,node6,node7));

	//testG->addEdge(edge1);
	//testG->addEdge(edge2);
	//testG->addEdge(edge3);
	//testG->addEdge(edge4);
	//testG->addEdge(edge5);
	//testG->addEdge(edge6);
	//testG->addEdge(edge7);
	//testG->addEdge(edge8);
	//testG->addEdge(edge9);
	//srand(1234);
	msonlab::GraphAlgorithms ga;

	GAOptions::gaPtr gaoptions(new GAOptions("Options.cfg"));
	FitnessStrategy::fsPtr fsstrategy (new LengthFitnessStartegy());
	GeneticAlgorithm gena(gaoptions, fsstrategy);
	//for (int n = 1; n <= 8; ++n)
	{
		//std::cout << "Test case " << n << std::endl;
		shared_ptr<Graph> randomG = GraphGenerator::generate(gaoptions->getGraphSize(), gaoptions->getGraphEdgeProb(), gaoptions->getGraphWidening(), gaoptions->getNumberOfPus());

		int greedy = ga.scheduleGreedy(randomG, gaoptions->getNumberOfPus());
		shared_ptr<Population> population = gena.generateInitialSolution(randomG);
		population->limit();

		unsigned last = population->best()->getFitness();
		unsigned not_changed = 0;

		for (size_t i = 0; i < gaoptions->getNumberOfYears(); ++i)
		{
			gena.simulateMating(population, gaoptions->getPopMaxSize());
			population->limit();
			unsigned best = population->best()->getFitness();
			std::cout << "Generation " << i+1 << std::endl;
			std::cout << "Best fitness: " << best << std::endl;
			std::cout << "Avarage fitness: " << population->avarageFittness() << std::endl;
			if (best == last)
			{
				++not_changed;
			} else
			{
				not_changed = 0;
				last = best;
			}
			if ( i == gaoptions->getNumberOfYears()-1 && not_changed < 10)
			{
				i -= 2*not_changed;
			}
		}

		//std::cout << "Best fitness: " << population->best()->getFitness() << std::endl;
		//std::cout << "Avarage fitness: " << population->avarageFittness() << std::endl;
	}

	std::cout << "Press a key to continue...";
	std::cin.get();

	return 0;
}

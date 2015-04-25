#include <iostream>

#include "../GraphLibrary/Graph.h"
#include "../GraphLibrary/BFSIterator.h"
#include "../GraphLibrary/DFSIterator.h"
#include "../GraphLibrary/GraphCreator.h"
#include "../GraphLibrary/GraphExchanger.h"
#include "../GraphLibrary/Algorithms.h"
#include "../GraphLibrary/GreedySchedulingAlgorithm.h"

using namespace std;
using namespace msonlab;
using namespace msonlab::graph::algorithms;
using namespace msonlab::scheduling;

// to try out features of the GraphLibrary
int main(int argc, char *argv[]){
	/*Graph graph = graph::creator::createQuadrant();
	GreedySchedulingAlgorithm alg{};
	Options options{ "Options.cfg" };
	auto result = alg.schedule(graph, options);
	result->printSolution(std::cout);*/
	auto g = msonlab::graph::creator::createSample();
	unsigned counter = 0;
	for (const auto &node : g.dfs()) {
		std::cout << node->id() << " ";
	}
}
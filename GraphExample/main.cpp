#include <iostream>
#include <vector>

#include "../GraphLibrary/Graph.h"
#include "../GraphLibrary/Node.h"
#include "../GraphLibrary/BFSIterator.h"
#include "../GraphLibrary/DFSIterator.h"
#include "../GraphLibrary/GraphCreator.h"
#include "../GraphLibrary/GraphExchanger.h"
#include "../GraphLibrary/Algorithms.h"
#include "../GraphLibrary/litegraph.h"
#include "../GraphLibrary/GreedySchedulingAlgorithm.h"
#include "../GraphLibrary/ListSchedulingAlgorithm.h"
#include "../GraphLibrary/CriticalPathSchedulingAlgorithm.h"
#include "../GraphLibrary/CoffmanGrahamSchedulingAlgorithm.h"

using namespace std;
using namespace msonlab;
using namespace msonlab::lite;
using namespace msonlab::graph::algorithms;
using namespace msonlab::scheduling;

// to try out features of the GraphLibrary
int main(int argc, char *argv[]){
	Graph graph = graph::creator::createQuadrant();
	//CoffmanGrahamSchedulingAlgorithm alg{};
	CriticalPathSchedulingAlgorithm alg{};
	Options options{ "Options.cfg" };
	auto result = alg.schedule(graph, options);
	result->printSolution(std::cout); 
}
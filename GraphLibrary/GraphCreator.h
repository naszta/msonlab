#ifndef GRAPHLIB_GRAPHGENERATOR_H
#define GRAPHLIB_GRAPHGENERATOR_H

#include "Graph.h"

namespace msonlab {
	namespace graph {
		namespace creator {
			Graph::gPtr createRandomLeveledDAG(size_t node_size, size_t level_size, unsigned edge_limit);
			Graph::gPtr createRandom(size_t size, unsigned edgeProb, unsigned widening, unsigned pus);
			Graph::gPtr createQuadrant();
			Graph::gPtr createSample();
			Graph::gPtr createTest();
			Graph::gPtr createCoffmanExample(unsigned);
		}
	}
}
#endif
#ifndef GRAPHLIB_GRAPHGENERATOR_H
#define GRAPHLIB_GRAPHGENERATOR_H

#include "Graph.h"

namespace msonlab {
	namespace graph {
		namespace creator {
			Graph createRandomLeveledDAG(size_t node_size, size_t level_size, unsigned edge_limit);
			Graph createRandom(size_t size, unsigned edgeProb, unsigned widening, unsigned pus);
			Graph createQuadrant();
			Graph createSample();
			Graph createTest();
			Graph createCoffmanExample(unsigned);
		}
	}
}
#endif
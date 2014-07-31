#ifndef GRAPHLIB_GRAPHGENERATOR_H
#define GRAPHLIB_GRAPHGENERATOR_H

#include "Graph.h"

namespace msonlab {
	namespace graph {
		namespace creator {
			Graph::gPtr createRandom(size_t size, unsigned edgeProb, unsigned widening, unsigned pus);
			Graph::gPtr createQuadrant();
			Graph::gPtr createSample();
			Graph::gPtr createTest();
			Graph::gPtr createCoffmanExample(unsigned);
		}
	}
}
#endif
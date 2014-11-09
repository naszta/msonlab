#include "stdafx.h"
#include "CppUnitTest.h"

#include "GreedySchedulingAlgorithm.h"
#include "GraphCreator.h"
#include "Graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GraphLibraryTest
{	
	using namespace msonlab;
	using namespace msonlab::graph;
	using namespace msonlab::scheduling;

	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
			GreedySchedulingAlgorithm alg();
			Graph graph = msonlab::graph::creator::createQuadrant();
			size_t expected_size = 19;
			Assert::AreEqual(expected_size, graph.size());

		}

	};
}
#include "stdafx.h"
#include "CppUnitTest.h"

#include "../GraphLibrary/Graph.h"
#include "../GraphLibrary/GraphCreator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GraphLibraryTest
{		
	TEST_CLASS(GraphTest)
	{
	public:
		
		TEST_METHOD(TestEmptyGraph)
		{
			msonlab::Graph g;
			size_t expected = 0;
			Assert::AreEqual(expected, g.size(), L"Graph is not empty.", LINE_INFO());
			const auto& edges = g.getEdges();
			Assert::AreEqual(expected, edges.size(), L"Graph has edges.", LINE_INFO());
			const auto& nodes = g.nodes();
			Assert::AreEqual(expected, nodes.size(), L"Graph has nodes.", LINE_INFO());
		}

		TEST_METHOD(TestSimpleGraph)
		{
			auto g = msonlab::graph::creator::createSample();
			size_t expected_nodes = 9;
			Assert::AreEqual(expected_nodes, g.size(), L"Graph has a different size.", LINE_INFO());
			const auto& edges = g.getEdges();
			size_t expected_edges = 8;
			Assert::AreEqual(expected_edges, edges.size(), L"Graph has different number of edges.", LINE_INFO());
			const auto& nodes = g.nodes();
			Assert::AreEqual(expected_nodes, nodes.size(), L"Graph has different number of nodes.", LINE_INFO());
			
		}

	};
}
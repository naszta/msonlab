#include "stdafx.h"
#include "GraphLibTestUtils.hpp"

#include "../GraphLibrary/Graph/Graph.h"
#include "../GraphLibrary/Graph/GraphCreator.h"

namespace GraphLibraryTest
{		
	TEST_CLASS(GraphTest)
	{
	public:
		
		TEST_METHOD(TestEmptyGraph)
		{
			MEMORY_LEAK_CHECK(
				msonlab::Graph g;
				size_t expected = 0;
				Assert::AreEqual(expected, g.order(), L"Graph is not empty.", LINE_INFO());
				const auto& edges = g.getEdges();
				Assert::AreEqual(expected, edges.size(), L"Graph has edges.", LINE_INFO());
				const auto& nodes = g.nodes();
				Assert::AreEqual(expected, nodes.size(), L"Graph has nodes.", LINE_INFO());
			)
		}

		TEST_METHOD(TestSimpleGraph)
		{
			MEMORY_LEAK_CHECK(
				auto g = msonlab::graph::creator::createSample();
				size_t expected_nodes = 9;
				Assert::AreEqual(expected_nodes, g.order(), L"Graph has a different order.", LINE_INFO());
				size_t expected_edges = 8;
				Assert::AreEqual(expected_edges, g.size(), L"Graph has a different size.", LINE_INFO());
				const auto& edges = g.getEdges();
				Assert::AreEqual(expected_edges, edges.size(), L"Graph has different number of edges.", LINE_INFO());
				const auto& nodes = g.nodes();
				Assert::AreEqual(expected_nodes, nodes.size(), L"Graph has different number of nodes.", LINE_INFO());
				unsigned id = 0;
				for (const auto& node : nodes) {
					Assert::AreEqual(id++, node->id(), L"Node order is not by id.", LINE_INFO());
				}
			)
		}

		TEST_METHOD(TestQuadrantGraph)
		{
			MEMORY_LEAK_CHECK(
				auto g = msonlab::graph::creator::createQuadrant();
				size_t expected_nodes = 19;
				Assert::AreEqual(expected_nodes, g.order(), L"Graph has a different order.", LINE_INFO());
				size_t expected_edges = 23;
				Assert::AreEqual(expected_edges, g.size(), L"Graph has a different size.", LINE_INFO());
				const auto& edges = g.getEdges();
				Assert::AreEqual(expected_edges, edges.size(), L"Graph has different number of edges.", LINE_INFO());
				const auto& nodes = g.nodes();
				Assert::AreEqual(expected_nodes, nodes.size(), L"Graph has different number of nodes.", LINE_INFO());
				unsigned id = 0;
				for (const auto& node : nodes) {
					Assert::AreEqual(id++, node->id(), L"Node order is not by id.", LINE_INFO());
				}
			)
		}

		TEST_METHOD(TestRandomGraph)
		{
			MEMORY_LEAK_CHECK(
				size_t expected_nodes = 42;
				auto g = msonlab::graph::creator::createRandom(expected_nodes, 35, 8, 2);
				const auto& nodes = g.nodes();
				Assert::AreEqual(expected_nodes, nodes.size(), L"Graph has different number of nodes.", LINE_INFO());
				unsigned id = 0;
				for (const auto& node : nodes) {
					Assert::AreEqual(id++, node->id(), L"Node order is not by id.", LINE_INFO());
				}
			)
		}
	};
}
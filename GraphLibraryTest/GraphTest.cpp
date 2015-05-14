#define _CRTDBG_MAP_ALLOC
#include <malloc.h>
#include <stdlib.h>
#include <crtdbg.h>

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
			Assert::AreEqual(expected, g.order(), L"Graph is not empty.", LINE_INFO());
			const auto& edges = g.getEdges();
			Assert::AreEqual(expected, edges.size(), L"Graph has edges.", LINE_INFO());
			const auto& nodes = g.nodes();
			Assert::AreEqual(expected, nodes.size(), L"Graph has nodes.", LINE_INFO());
		}

		TEST_METHOD(TestSimpleGraph)
		{
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
			
		}

		TEST_METHOD(TestQuadrantGraph)
		{
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
		}

		TEST_METHOD(TestRandomGraph)
		{
			_CrtMemState s1, s2, s3;
			_CrtMemCheckpoint(&s1);
			{
				size_t expected_nodes = 42;
				auto g = msonlab::graph::creator::createRandom(expected_nodes, 35, 8, 2);
				const auto& nodes = g.nodes();
				Assert::AreEqual(expected_nodes, nodes.size(), L"Graph has different number of nodes.", LINE_INFO());
				unsigned id = 0;
				for (const auto& node : nodes) {
					Assert::AreEqual(id++, node->id(), L"Node order is not by id.", LINE_INFO());
				}
			}
			_CrtMemCheckpoint(&s2);
			bool leaked = _CrtMemDifference(&s3, &s1, &s2);
			Assert::IsTrue(!leaked);
		}
	};
}
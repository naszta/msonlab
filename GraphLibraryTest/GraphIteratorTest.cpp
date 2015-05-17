#include "stdafx.h"
#include "GraphLibTestUtils.hpp"

#include <iostream>

#include "../GraphLibrary/Graph.h"
#include "../GraphLibrary/BFSIterator.h"
#include "../GraphLibrary/DFSIterator.h"
#include "../GraphLibrary/GraphCreator.h"

namespace GraphLibraryTest
{
	TEST_CLASS(GraphIteratorTest)
	{
	public:
		TEST_METHOD(TestBFSIterator)
		{ CREATE_MEMORY_SNAPSHOT {
			auto g = msonlab::graph::creator::createSample();
			unsigned counter = 0;
			for (const auto &node : g.bfs()) {
				++counter;
			}

			Assert::AreEqual(g.order(), counter, L"Iterated nodes and order of graph doesn't match", LINE_INFO());
			
			// assert that depth may increase just by one at a time
			unsigned last_depth = 0;
			auto it = g.bfs().begin();
			auto end = g.bfs().end();
			for (; it != end; ++it) {
				unsigned current_depth = it.depth();
				Assert::IsTrue(current_depth == 0 || current_depth == last_depth || current_depth == last_depth + 1,
					L"Invalid depth value", LINE_INFO());
				last_depth = current_depth;
			}

			unsigned node_order[] { 0, 4, 6, 8, 1, 2, 5, 3, 7  };
			unsigned index = 0;
			for (const auto &node : g.bfs()) {
				Assert::AreEqual(node_order[index], node->id(), L"Node doesn't match " + index, LINE_INFO());
				++index;
			}
		} ASSERT_MEMORY_SNAPSHOT }

		TEST_METHOD(TestBFSIteratorSkipNode)
		{ CREATE_MEMORY_SNAPSHOT{
			auto g = msonlab::graph::creator::createSample();
			unsigned node_order[] { 0, 4, 6, 1, 2, 5, 3, 7  };
			unsigned index = 0;
			msonlab::BFSIterator end_it{ g.bfs().end() };
			for (auto it = g.bfs().begin(); it != end_it; ++it) {
				Assert::AreEqual(node_order[index], (*it)->id(), L"Node doesn't match " + index, LINE_INFO());
				++index;
				// skipping node id 8
				if ((*it)->id() == 6) {
					it.skipChildren();
				}
			}
		} ASSERT_MEMORY_SNAPSHOT }

		TEST_METHOD(TestDFSIterator)
		{ CREATE_MEMORY_SNAPSHOT {
			auto g = msonlab::graph::creator::createSample();
			unsigned counter = 0;
			for (const auto &node : g.dfs()) {
				++counter;
			}

			Assert::AreEqual(g.order(), counter, L"Iterated nodes and order of graph doesn't match", LINE_INFO());

			unsigned node_order[] { 7, 6, 8, 3, 5, 2, 1, 4, 0 };
			unsigned index = 0;
			for (const auto &node : g.dfs()) {
				Assert::AreEqual(node_order[index], node->id(), L"Node doesn't match " + index, LINE_INFO());
				++index;
			}
		} ASSERT_MEMORY_SNAPSHOT }

		TEST_METHOD(TestDFSIteratorSkipNode)
		{ CREATE_MEMORY_SNAPSHOT {
			auto g = msonlab::graph::creator::createSample();
			unsigned node_order[] { 7, 6, 3, 5, 2, 1, 4, 0 };
			unsigned index = 0;
			msonlab::DFSIterator end_it{ g.dfs().end() };
			for (auto it = g.dfs().begin(); it != end_it; ++ it) {
				Assert::AreEqual(node_order[index], (*it)->id(), L"Node doesn't match " + index, LINE_INFO());
				++index;
				// skipping node id 8
				if ((*it)->id() == 6) {
					it.skipChildren();
				}
			}
		} ASSERT_MEMORY_SNAPSHOT }
	};
}
#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>

#include "../GraphLibrary/Graph.h"
#include "../GraphLibrary/BFSIterator.h"
#include "../GraphLibrary/DFSIterator.h"
#include "../GraphLibrary/GraphCreator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GraphLibraryTest
{
	TEST_CLASS(GraphIteratorTest)
	{
	public:

		TEST_METHOD(TestBFSIterator)
		{
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
		}

		TEST_METHOD(TestDFSIterator)
		{
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
		}

	};
}
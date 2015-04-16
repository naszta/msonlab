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
			for (auto node : g.bfs()) {
				++counter;
			}

			Assert::AreEqual(g.size(), counter, L"Iterated nodes and order of graph doesn't match", LINE_INFO());
			
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

			for (auto node : g.bfs()) {

			}

		}

		TEST_METHOD(TestDFSIterator)
		{

		}

	};
}
#include "stdafx.h"
#include "CppUnitTest.h"

#include "../GraphLibrary/Graph.h"

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
			Assert::AreEqual(expected, edges.size(), L"Graph has edges.", LINE_INFO());
		}

	};
}
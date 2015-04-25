#include "stdafx.h"
#include "CppUnitTest.h"

#include <sstream>

#include "../GraphLibrary/Graph.h"
#include "../GraphLibrary/GraphCreator.h"
#include "../GraphLibrary/GreedySchedulingAlgorithm.h"
#include "../GraphLibrary/SchedulingHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace msonlab;
using namespace msonlab::graph;
using namespace msonlab::scheduling;

namespace Microsoft{ namespace VisualStudio { namespace CppUnitTestFramework
	{
		static std::wstring ToString(const vector<unsigned>& t)
		{
			std::wstringstream stream{};
			for (auto u : t) {
				stream << u << L",";
			}
			return stream.str();
		}

	}}}
namespace GraphLibraryTest
{
	TEST_CLASS(GreedySchedulingAlgorithmTest)
	{
	public:

		TEST_METHOD(TestGreedyWithEmptyGraph)
		{
			msonlab::Graph graph;
			GreedySchedulingAlgorithm alg{};
			Options options{ "Options.cfg" };
			auto result = alg.schedule(graph, options);
			unsigned expected = 0;
			Assert::AreEqual(expected, result->size(), L"Result is not empty", LINE_INFO());
			Assert::AreEqual(expected, result->fitness(), L"Result's fitness is not zero.", LINE_INFO());
			Assert::AreEqual(expected, result->mapping().size(), L"Result's mapping is not empty.", LINE_INFO());
			Assert::AreEqual(expected, result->scheduling().size(), L"Result's scheduling is not empty.", LINE_INFO());
		}

		TEST_METHOD(TestGreedyWithSimpleGraph)
		{
			auto graph = msonlab::graph::creator::createSample();
			GreedySchedulingAlgorithm alg{};
			Options options{ "Options.cfg" };
			auto result = alg.schedule(graph, options);
			Assert::IsTrue(is_correct(*result), L"Result is not correct", LINE_INFO());
			auto expected = graph.order();
			Assert::AreEqual(expected, result->size(), L"Result's size and graph order not equal", LINE_INFO());
			unsigned expected_fitness = 20;
			Assert::AreEqual(expected_fitness, result->fitness(), L"Result's fitness is not the expected.", LINE_INFO());
			
			Assert::AreEqual(vector < unsigned > {0, 1, 0, 1, 0, 1, 0, 1, 0},
				result->mapping(), L"Result's mapping is not empty.", LINE_INFO());
			vector<const NodePtr> expected_scheduling;

			Assert::AreEqual(expected, result->scheduling().size(), L"Result's scheduling is not empty.", LINE_INFO());
		}

		TEST_METHOD(TestGreedyWithQuadrantGraph)
		{
			auto graph = msonlab::graph::creator::createQuadrant();
			GreedySchedulingAlgorithm alg{};
			Options options{ "Options.cfg" };
			auto result = alg.schedule(graph, options);
			Assert::IsTrue(is_correct(*result), L"Result is not correct", LINE_INFO());

			auto expected = graph.order();
			Assert::AreEqual(expected, result->size(), L"Result's size and graph order not equal", LINE_INFO());
			unsigned expected_fitness = 48;
			Assert::AreEqual(expected_fitness, result->fitness(), L"Result's fitness is not the expected.", LINE_INFO());

			Assert::AreEqual(vector < unsigned > {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1},
				result->mapping(), L"Result's mapping is not empty.", LINE_INFO());
		}

	};
}
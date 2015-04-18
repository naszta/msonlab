#include "stdafx.h"
#include "CppUnitTest.h"

#include "../GraphLibrary/Graph.h"
#include "../GraphLibrary/GraphCreator.h"
#include "../GraphLibrary/Algorithms.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace msonlab;
using namespace msonlab::graph::algorithms;

namespace GraphLibraryTest
{
	TEST_CLASS(AlgorithmsTest)
	{
	public:
		TEST_METHOD(TestLayeredOrderConstructionForSampleGraph)
		{
			vector<vector<unsigned>> expected_ids{
				vector < unsigned > {8}, vector < unsigned > {6, 5},
				vector < unsigned > {4, 7, 2, 3}, vector < unsigned > {0, 1} };

			auto g = graph::creator::createSample();
			vector<vector<const NodePtr>> layers;
			constructLayeredOrder<Graph, const NodePtr>(g, layers);
			AssertLayers(expected_ids, layers);
		}

		TEST_METHOD(TestLayeredOrderConstructionForQuadrantGraph)
		{
			vector<vector<unsigned>> expected_ids{
				vector < unsigned > {17, 18}, vector < unsigned > {15, 16, 11},
				vector < unsigned > {9, 14, 3, 7}, vector < unsigned > {5, 13, 4},
				vector < unsigned > {12}, vector < unsigned > {8, 10},
				vector < unsigned > {1, 0, 2, 6} };

			auto g = graph::creator::createQuadrant();
			vector<vector<const NodePtr>> layers;
			constructLayeredOrder<Graph, const NodePtr>(g, layers);
			AssertLayers(expected_ids, layers);
		}

		void AssertLayers(const vector < vector<unsigned>>& expected, const vector<vector<const NodePtr>>& actual)
		{
			size_t layer_id = 0;
			for (const auto& layer : actual) {
				size_t id = 0;
				for (const auto& node : layer) {
					Assert::AreEqual(expected[layer_id][id], node->id(), L"Order doesn't match", LINE_INFO());
					++id;
				}
				++layer_id;
			}
		}

		TEST_METHOD(TestCreateChangedGraph)
		{
			auto g = graph::creator::createQuadrant();
			const auto& input = g.getInputNodes();
			const auto& output = g.getOutputNodes();
			NodePtr changedNode;
			for (const auto& node : input) {
				if (node->id() == 2) changedNode = node;
			}

			NodeSet changed{ changedNode };
			NodeSet needed{ output.begin(), output.end() };

			auto subgraph = computeChangedGraph(g, changed, needed);
			size_t expected_nodes = 14;
			Assert::AreEqual(expected_nodes, subgraph.order(), L"Sub-Graph has a different order.", LINE_INFO());
			size_t expected_edges = 15;
			Assert::AreEqual(expected_edges, g.size(), L"Sub-Graph has a different size.", LINE_INFO());
		}
	};
}

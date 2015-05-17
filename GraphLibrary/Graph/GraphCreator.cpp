#include "GraphCreator.h"

#include "NodeConstant.h"
#include "NodeAdd.h"
#include "NodeMultiply.h"
#include "NodeDivide.h"
#include "NodeSquareRoot.h"
#include "NodeTest.h"

#include "BlueEdge.h"

#include <sstream>

namespace msonlab {
	namespace graph {
		namespace creator {

			using std::make_shared;
			using std::make_unique;

			Graph createRandomLeveledDAG(size_t node_size, size_t level_size, unsigned edge_limit) {
				NodeVect nodes(node_size);
				vector<unsigned> in_count(node_size);
				vector<unsigned> out_count(node_size); // sure?

				// per level nodes
				unsigned pln = node_size / level_size;

				// current level size
				unsigned cls = pln + 1;
				// next level size
				unsigned nls = (rand() % ((pln + 1) / 2)) + (3 * pln + 1) / 4;
				// the first node in the next level
				unsigned first = cls;

				Graph graph;
				// generating nodes for the graph
				size_t i;
				for (i = 0; i < cls; ++i)
				{
					nodes[i] = make_shared<NodeConstant>(i, L"input", make_shared<types::DataType>(i));
					graph.addNode(nodes[i]);
				}

				for (i = cls; i < node_size; ++i)
				{
					//int ct = rand() % 4 + 2;
					std::wstringstream wss;
					wss << "#" << i;
					nodes[i] = make_shared<NodeAdd>(i, wss.str(), make_shared<types::DataType>(i));
					graph.addNode(nodes[i]);
				}

				// the actual node's id that's out edges are generated
				//unsigned node_counter = 0;
				unsigned edge_counter = node_size;
				// for each level
				for (unsigned i = 0; i < level_size - 1; ++i) {
					for (unsigned c = 0; c < cls; ++c) {
						// number of out edges from this node
						unsigned out = rand() % edge_limit + 1;
						unsigned from_id = first - cls + c;
						for (unsigned j = 0; j < out; ++j) {
							unsigned to = rand() % nls;
							unsigned to_id = first + to;
							unsigned retries = nls - 1;
							while (in_count[to_id] == edge_limit && --retries > 0) {
								to_id = first + (to + retries) % nls;
							}
							
							// if the limit lets it, add the edge
							if (in_count[to_id] < edge_limit) {
								auto edge = std::make_shared<Edge>(edge_counter, L"a", make_shared<types::DataType>(edge_counter), nodes[from_id], nodes[to_id]);
								graph.addEdge(edge);
								++in_count[to_id];
								++edge_counter;
							}
						}

						//++node_counter;
					}

					// update level information
					first += nls;
					cls = nls;
					nls = (rand() % ((pln + 1) / 2)) + (3 * pln + 1) / 4;
					if (first + nls > node_size){
						nls = node_size - first;
					}
					else if (level_size - 3 == i && first + nls < node_size) {
						nls = node_size - first;
					}
				}

				return std::move(graph);
			}

			Graph createRandom(size_t size, unsigned edgeProb, unsigned widening, unsigned pus)
			{
				NodeVect nodes(size);
				Graph graph;

				size_t input_size = 2 * pus < size ? 2 * pus : size / 3;
				size_t output_size = std::min(input_size - 1, widening);
				unsigned closer_favor = widening;

				size_t i;
				for (i = 0; i < input_size; ++i)
				{
					nodes[i] = make_shared<NodeConstant>(i, L"input", make_shared<types::DataType>(i));
					graph.addNode(nodes[i]);
				}

				for (i = input_size; i < size; ++i)
				{
					int ct = rand() % 4 + 2;
					std::wstringstream wss;
					wss << "#" << i;
					nodes[i] = make_shared<NodeTest>(i, wss.str(), make_shared<types::DataType>(i), ct);
					graph.addNode(nodes[i]);
				}

				// adding inputs
				size_t edge_counter = size;
				// generating edges from the input nodes
				for (size_t i = 0; i < input_size; ++i)
				{
					// the number of output edges from node with id i
					unsigned edges = rand() % (edgeProb - 1) + 1;
					// space of id-s to randomly choose an end node for the edge
					// the first 'widening' number of nodes has 'close_favor' more chance
					// to get elected than the other nodes
					unsigned space = size - input_size + (closer_favor - 1) * widening;
					for (size_t j = 0; j < edges; ++j)
					{
						unsigned node_id = (rand() % (space - 1)) + 1;
						if (node_id < closer_favor * widening)
						{
							node_id /= closer_favor;
						}
						else
						{
							node_id -= (closer_favor - 1) * widening;
						}

						node_id += input_size;
						EdgePtr e = make_shared<Edge>(edge_counter, L"a", make_shared<types::DataType>(i + node_id), nodes[i], nodes[node_id]);
						++edge_counter;
						graph.addEdge(e);
					}
				}

				for (size_t i = input_size; i < size - output_size; ++i)
				{
					// number of edges of the node with id 'i'
					unsigned edges = rand() % edgeProb;
					unsigned favored = size - i - 1 < widening ? size - i - 1 : widening;
					unsigned space = size - i + (closer_favor - 1) * favored;
					for (size_t j = 0; j < edges; ++j)
					{
						unsigned node_id = (rand() % (space - 1)) + 1;
						if (node_id < closer_favor * favored)
						{
							node_id /= closer_favor;
						}
						else
						{
							node_id -= (closer_favor - 1) * favored;
						}

						if (node_id == 0) ++node_id;
						node_id += i + 3;
						if (node_id >= graph.order()) {
							node_id = graph.order() - 1;
						}
						EdgePtr e = make_shared<Edge>(edge_counter, L"a", make_shared<types::DataType>(i + node_id), nodes[i], nodes[node_id]);
						++edge_counter;
						graph.addEdge(e);
					}
				}

				std::cout << "Random graph created\n";
				return graph;
			}

			Graph createQuadrant(){
				msonlab::NodePtr a = make_shared<msonlab::NodeConstant>(0, L"a", make_shared<types::DataType>(4));
				msonlab::NodePtr b = make_shared<msonlab::NodeConstant>(1, L"b", make_shared<types::DataType>(2));
				msonlab::NodePtr c = make_shared<msonlab::NodeConstant>(2, L"c", make_shared<types::DataType>(-5));

				msonlab::NodePtr constNumber_1 = (make_shared<msonlab::NodeConstant>(3, L"1", make_shared<types::DataType>(1)));
				msonlab::NodePtr constNumber_2 = (make_shared<msonlab::NodeConstant>(4, L"2", make_shared<types::DataType>(2)));
				msonlab::NodePtr constNumber_minus1 = (make_shared<msonlab::NodeConstant>(5, L"-1", make_shared<types::DataType>(-1)));
				msonlab::NodePtr constNumber_minus4 = (make_shared<msonlab::NodeConstant>(6, L"-4", make_shared<types::DataType>(-4)));

				msonlab::NodePtr multiply_2a(new msonlab::NodeMultiply(7, L"2a", make_shared<types::DataType>(0)));
				msonlab::NodePtr multiply_bb(new msonlab::NodeMultiply(8, L"b^2", make_shared<types::DataType>(0)));
				msonlab::NodePtr multiply_minusb(new msonlab::NodeMultiply(9, L"-b", make_shared<types::DataType>(0)));
				msonlab::NodePtr multiply_minus4ac(new msonlab::NodeMultiply(10, L"-4ac", make_shared<types::DataType>(0)));

				msonlab::NodePtr divide_1_2a(new msonlab::NodeDivide(11, L"1/2a", make_shared<types::DataType>(0)));

				msonlab::NodePtr add_bb_minus4ac(new msonlab::NodeAdd(12, L"b^2-4ac", make_shared<types::DataType>(0)));

				msonlab::NodePtr squareRoot_bb_minus4ac(new msonlab::NodeSquareRoot(13, L"(b^2-4ac)^0.5", make_shared<types::DataType>(0)));

				msonlab::NodePtr multiply_minus_squareRoot_bb_minus4ac(new msonlab::NodeMultiply(14, L"-(b^2-4ac)^0.5", make_shared<types::DataType>(0)));

				msonlab::NodePtr add_toDivide1(new msonlab::NodeAdd(15, L"-b+(b^2-4ac)^0.5", make_shared<types::DataType>(0)));
				msonlab::NodePtr add_toDivide2(new msonlab::NodeAdd(16, L"-b-(b^2-4ac)^0.5", make_shared<types::DataType>(0)));

				msonlab::NodePtr y1(new msonlab::NodeMultiply(17, L"Y1", make_shared<types::DataType>(0)));
				msonlab::NodePtr y2(new msonlab::NodeMultiply(18, L"Y2", make_shared<types::DataType>(0)));


				msonlab::EdgePtr e1(new msonlab::BlueEdge(19, L"e1", 0, a, multiply_2a));
				msonlab::EdgePtr e2(new msonlab::BlueEdge(20, L"e2", 0, constNumber_2, multiply_2a));
				msonlab::EdgePtr e3(new msonlab::BlueEdge(21, L"e3", 0, a, multiply_minus4ac));

				msonlab::EdgePtr e4(new msonlab::BlueEdge(22, L"e4", 0, b, multiply_bb));
				msonlab::EdgePtr e5(new msonlab::BlueEdge(23, L"e5", 0, b, multiply_bb));
				msonlab::EdgePtr e6(new msonlab::BlueEdge(24, L"e6", 0, b, multiply_minusb));
				msonlab::EdgePtr e7(new msonlab::BlueEdge(25, L"e7", 0, constNumber_minus1, multiply_minusb));

				msonlab::EdgePtr e8(new msonlab::Edge(26, L"e8", 0, c, multiply_minus4ac));
				msonlab::EdgePtr e9(new msonlab::BlueEdge(27, L"e9", 0, constNumber_minus4, multiply_minus4ac));

				msonlab::EdgePtr e10(new msonlab::Edge(28, L"e10", 0, constNumber_1, divide_1_2a));
				msonlab::EdgePtr e11(new msonlab::Edge(29, L"e11", 0, multiply_2a, divide_1_2a));

				msonlab::EdgePtr e12(new msonlab::Edge(30, L"e12", 0, multiply_bb, add_bb_minus4ac));
				msonlab::EdgePtr e13(new msonlab::Edge(31, L"e13", 0, multiply_minus4ac, add_bb_minus4ac));

				msonlab::EdgePtr e14(new msonlab::Edge(32, L"e14", 0, add_bb_minus4ac, squareRoot_bb_minus4ac));

				msonlab::EdgePtr e15(new msonlab::BlueEdge(33, L"e15", 0, squareRoot_bb_minus4ac, multiply_minus_squareRoot_bb_minus4ac));

				msonlab::EdgePtr e16(new msonlab::Edge(34, L"e16", 0, multiply_minusb, add_toDivide1));
				msonlab::EdgePtr e17(new msonlab::Edge(35, L"e17", 0, squareRoot_bb_minus4ac, add_toDivide1));

				msonlab::EdgePtr e18(new msonlab::Edge(36, L"e18", 0, multiply_minusb, add_toDivide2));
				msonlab::EdgePtr e19(new msonlab::Edge(37, L"e19", 0, multiply_minus_squareRoot_bb_minus4ac, add_toDivide2));

				msonlab::EdgePtr e20(new msonlab::Edge(38, L"e20", 0, add_toDivide1, y1));
				msonlab::EdgePtr e21(new msonlab::Edge(39, L"e21", 0, divide_1_2a, y1));

				msonlab::EdgePtr e22(new msonlab::Edge(40, L"e22", 0, add_toDivide2, y2));
				msonlab::EdgePtr e23(new msonlab::Edge(41, L"e23", 0, divide_1_2a, y2));

				Graph graph{ e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11,
				e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23};

				return std::move(graph);
			}

			Graph createOneEdge() {
				auto data = make_shared<types::DataType>(4);
				auto a = make_shared<msonlab::NodeConstant>(0, L"a", data);
				auto b = make_shared<msonlab::NodeConstant>(1, L"b", data);
				auto e1 = make_shared<msonlab::Edge>(9, L"e1", data, a, b);
				return Graph{ e1 };
			}

			Graph createSample() {
				msonlab::NodePtr a(new msonlab::NodeConstant(0, L"a", make_shared<types::DataType>(2)));
				msonlab::NodePtr b(new msonlab::NodeConstant(1, L"b", make_shared<types::DataType>(3)));
				msonlab::NodePtr c(new msonlab::NodeConstant(2, L"c", make_shared<types::DataType>(2)));
				msonlab::NodePtr d(new msonlab::NodeConstant(3, L"d", make_shared<types::DataType>(3)));

				msonlab::NodePtr ab(new msonlab::NodeMultiply(4, L"a*b", nullptr));
				msonlab::NodePtr cd(new msonlab::NodeMultiply(5, L"c*d", nullptr));

				msonlab::NodePtr ab2(new msonlab::NodeMultiply(6, L"2*a*b", nullptr));
				msonlab::NodePtr c2(new msonlab::NodeConstant(7, L"2", make_shared<types::DataType>(2)));

				msonlab::NodePtr res(new msonlab::NodeAdd(8, L"2*a*b + c*d", nullptr));

				msonlab::EdgePtr e1(new msonlab::Edge(9, L"e1", 0, a, ab));
				msonlab::EdgePtr e2(new msonlab::Edge(10, L"e2", 0, b, ab));

				msonlab::EdgePtr e3(new msonlab::Edge(11, L"e3", 0, c, cd));
				msonlab::EdgePtr e4(new msonlab::Edge(12, L"e4", 0, d, cd));

				msonlab::EdgePtr e5(new msonlab::Edge(13, L"e5", 0, ab, ab2));
				msonlab::EdgePtr e6(new msonlab::Edge(14, L"e6", 0, c2, ab2));

				msonlab::EdgePtr e7(new msonlab::Edge(15, L"e7", 0, ab2, res));

				msonlab::EdgePtr e8(new msonlab::Edge(16, L"e8", 0, cd, res));

				Graph graph{e1, e2, e3, e4, e5, e6, e7, e8};
				return std::move(graph);
			}

			Graph createTest() {
				NodePtr node0 = make_shared<NodeConstant>(0, L"0", make_shared<types::DataType>(5));
				NodePtr node1 = make_shared<NodeConstant>(1, L"1", make_shared<types::DataType>(2));
				NodePtr node2 = make_shared<NodeAdd>(2, L"2", make_shared<types::DataType>(3));
				NodePtr node3 = make_shared<NodeAdd>(3, L"3", make_shared<types::DataType>(2));
				NodePtr node4 = make_shared<NodeAdd>(4, L"4", make_shared<types::DataType>(9));
				NodePtr node5 = make_shared<NodeAdd>(5, L"5", make_shared<types::DataType>(2));
				NodePtr node6 = make_shared<NodeAdd>(6, L"6", make_shared<types::DataType>(6));
				NodePtr node7 = make_shared<NodeAdd>(7, L"7", make_shared<types::DataType>(5));
				NodePtr node8 = make_shared<NodeAdd>(8, L"8", make_shared<types::DataType>(4));

				EdgePtr edge1 = make_shared<Edge>(9, L"e19", nullptr, node0, node8);
				EdgePtr edge2 = make_shared<Edge>(10, L"e69", nullptr, node5, node8);
				EdgePtr edge3 = make_shared<Edge>(11, L"e13", nullptr, node0, node2);
				EdgePtr edge4 = make_shared<Edge>(12, L"e14", nullptr, node0, node3);
				EdgePtr edge5 = make_shared<Edge>(13, L"e15", nullptr, node0, node4);
				EdgePtr edge6 = make_shared<Edge>(14, L"e17", nullptr, node0, node6);
				EdgePtr edge7 = make_shared<Edge>(16, L"e26", nullptr, node1, node5);
				EdgePtr edge8 = make_shared<Edge>(17, L"e27", nullptr, node1, node6);
				EdgePtr edge9 = make_shared<Edge>(18, L"e38", nullptr, node2, node7);
				EdgePtr edge10 = make_shared<Edge>(19, L"e48", nullptr, node3, node7);
				EdgePtr edge11 = make_shared<Edge>(20, L"e79", nullptr, node6, node8);
				EdgePtr edge12 = make_shared<Edge>(21, L"e89", nullptr, node7, node8);
				EdgePtr edge13 = make_shared<Edge>(22, L"e89", nullptr, node4, node5);
				EdgePtr edge14 = make_shared<Edge>(23, L"e89", nullptr, node4, node8);
				EdgePtr edge15 = make_shared<Edge>(24, L"e89", nullptr, node2, node8);
				EdgePtr edge16 = make_shared<Edge>(25, L"e07", nullptr, node0, node7);

				Graph graph{ edge1, edge2, edge3, edge4, edge5, edge6, edge7, edge8,
					edge9, edge10, edge11, edge12, edge13, edge14, edge15, edge16 };
				return std::move(graph);
			}

			Graph createCoffmanExample(unsigned comp_time) {
				NodePtr nodeA = make_shared<NodeTest>(0, L"A", make_shared<types::DataType>(5), comp_time);
				NodePtr nodeB = make_shared<NodeTest>(1, L"B", make_shared<types::DataType>(2), comp_time);
				NodePtr nodeC = make_shared<NodeTest>(2, L"C", make_shared<types::DataType>(3), comp_time);
				NodePtr nodeD = make_shared<NodeTest>(3, L"D", make_shared<types::DataType>(2), comp_time);
				NodePtr nodeE = make_shared<NodeTest>(4, L"E", make_shared<types::DataType>(9), comp_time);
				NodePtr nodeF = make_shared<NodeTest>(5, L"F", make_shared<types::DataType>(2), comp_time);
				NodePtr nodeG = make_shared<NodeTest>(6, L"G", make_shared<types::DataType>(6), comp_time);
				NodePtr nodeH = make_shared<NodeTest>(7, L"H", make_shared<types::DataType>(5), comp_time);
				NodePtr nodeI = make_shared<NodeTest>(8, L"I", make_shared<types::DataType>(4), comp_time);
				NodePtr nodeJ = make_shared<NodeTest>(9, L"J", make_shared<types::DataType>(4), comp_time);

				auto edge_value = make_shared<types::DataType>(1);
				EdgePtr gh = make_shared<Edge>(10, L"GH", edge_value, nodeG, nodeH);
				EdgePtr dg = make_shared<Edge>(11, L"DG", edge_value, nodeD, nodeG);
				EdgePtr eg = make_shared<Edge>(12, L"EG", edge_value, nodeE, nodeG);
				EdgePtr fg = make_shared<Edge>(13, L"FG", edge_value, nodeF, nodeG);
				EdgePtr ad = make_shared<Edge>(14, L"AD", edge_value, nodeA, nodeD);
				EdgePtr ae = make_shared<Edge>(15, L"AE", edge_value, nodeA, nodeE);
				EdgePtr af = make_shared<Edge>(16, L"AF", edge_value, nodeA, nodeF);

				EdgePtr be = make_shared<Edge>(15, L"BE", edge_value, nodeB, nodeE);
				EdgePtr bf = make_shared<Edge>(16, L"BF", edge_value, nodeB, nodeD);
				EdgePtr bi = make_shared<Edge>(17, L"BI", edge_value, nodeB, nodeI);
				EdgePtr bj = make_shared<Edge>(18, L"BJ", edge_value, nodeB, nodeJ);

				EdgePtr ce = make_shared<Edge>(19, L"CE", edge_value, nodeC, nodeE);
				EdgePtr cf = make_shared<Edge>(20, L"CF", edge_value, nodeC, nodeD);
				EdgePtr ci = make_shared<Edge>(21, L"CI", edge_value, nodeC, nodeI);
				EdgePtr cj = make_shared<Edge>(22, L"CJ", edge_value, nodeC, nodeJ);

				Graph graph{gh, dg, eg, fg, ad, ae, af, be, bf, bi, bj, ce, cf, ci, cj};
				return std::move(graph);
			}
		}
	}
}
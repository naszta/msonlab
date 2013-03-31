#include <boost/lambda/lambda.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <ostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <string>

#include "Node.hpp"
#include "Graph.hpp"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	using namespace boost;
	using namespace zgraph;
	using std::string;
	using std::ostream;
	using std::ofstream;
	using std::vector;

	{
		//std::cout << "Give me a graph, please!" << std::endl;
		ofstream file;
		file.open("graph.gv");
		Graph<int> g;
		Node<int> a("a", 1);
		Node<int> b("b", 2);
		Node<int> c("c", 3);
		g.add_edge(a, b);
		g.add_edge(a, c);
		g.add_edge(b, c);
		file << g;
		file.close();

		IGraphAlgorithms<int> algo;
		algo.print_graph(std::cout, g);
		// creates png from the gv file
		//system("dot -Tpng graph.gv -o graph1.png");
	}

	_CrtDumpMemoryLeaks();
	return 0;
}
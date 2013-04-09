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
		shared_ptr< Graph<int> > graph(new Graph<int>());
		Node<int> a("a", 1);
		Node<int> b("b", 2);
		Node<int> c("c", 3);
		Node<int> d("d", 4);
		Node<int> e("e", 5);
		Node<int> f("f", 6);
		Node<int> g("g", 7);
		Node<int> h("h", 8);
		graph->add_edge(a, b);
		graph->add_edge(a, c);
		graph->add_edge(b, c);
		graph->add_edge(b, d);
		graph->add_edge(d, e);
		graph->add_edge(c, f);
		graph->add_edge(f, h);
		graph->add_edge(d, g);
		graph->add_edge(a, e);
		file << *graph;
		file.close();


		set< shared_ptr< INode<int> > > changed;
		set< shared_ptr< INode<int> > > needed;
		// adding node "b" 
		changed.insert(graph->get_all_nodes()[1]);
		needed.insert(graph->get_all_nodes()[5]);
		//changed.insert("b");

		IGraphAlgorithms<int>::BFS_iterator it(graph);
		for (; it != it.end(); ++it)
		{
			std::cout << **it << std::endl;
			if (changed.count(*it) > 0)
			{
				it.skip();
			}
		}

		IGraphAlgorithms<int> algo;
		shared_ptr< IGraph<int> > ngraph = algo.get_changed_graph(graph, changed, needed);

		file.open("graph2.gv");
		file << *ngraph;
		file.close();
		//vector< shared_ptr< INode<int> > > order = algo.topological_order(graph);
		//for(unsigned i = 0; i < order.size(); ++i)
		//{
		//	std::cout << i+1 << order[i]->get_label() << "\n";
		//}

		//algo.print_graph(std::cout, *graph);
		// creates png from the gv file
		system("dot -Tpng graph2.gv -o graph2.png");
	}

	_CrtDumpMemoryLeaks();
	return 0;
}
#pragma once

#include <vector>
#include "Graph.h"
#include "Node.h"

using std::vector;
using std::shared_ptr;

namespace msonlab
{
	class GraphGenerator
	{

	public:
		///
		// genererates a random graph
		// size: number of nodes in the graph
		// edgeProb: the probability of an edge between any two node
		// return: the generated graph
		///
		static shared_ptr<Graph> generate(size_t size, unsigned edgeProb, unsigned widening, unsigned pus)
		{
			IProcessable::nVect nodes(size);
			shared_ptr<Graph> graph(new Graph());

			for (size_t i = 0; i < size; ++i)
			{
				nodes[i] = Node::nPtr(new Node(i, L"a", shared_ptr<double>(new double(i))));
				graph->addNode(nodes[i]);
			}

			size_t input_size = pus+4 < size ? pus+4 : size / 3;
			size_t output_size = input_size - 1 > widening ? widening : input_size - 1;
			unsigned closer_favor = widening - 1;

			// adding inputs
			for (size_t i = 0; i < input_size; ++i)
			{
				unsigned edges = i < pus ? (rand () % (edgeProb-1))+1 : edgeProb +1 ;
				unsigned space = size - input_size + closer_favor * widening;
				for (size_t j = 0; j < edges; ++j)
				{
					unsigned nodeId = (rand()%(space-1))+1;
					if (nodeId < (closer_favor+1)*widening)
					{
						nodeId /= (closer_favor+1);
					} else
					{
						nodeId -= closer_favor*widening;
					}

					nodeId += input_size;
					IProcessable::ePtr e(new Edge(i + nodeId, L"a", shared_ptr<double>(new double(i + nodeId)), nodes[i], nodes[nodeId]));
					graph->addEdge(e);
				}
			}

			for (size_t i = input_size; i < size - output_size; ++i)
			{
				unsigned edges = rand () % edgeProb;
				unsigned favored = size - i - 1 < widening ? size - i -1  : widening;
				unsigned space = size - i + closer_favor* favored;
				if (edges == edgeProb -1 ) edges += widening;
				for (size_t j = 0; j < edges; ++j)
				{
					unsigned nodeId = (rand()%(space-1))+1;
					if (nodeId < (closer_favor+1)*favored)
					{
						nodeId /= (closer_favor+1);
						++nodeId;
					} else
					{
						nodeId -= closer_favor*favored;
					}

					nodeId += i;
					IProcessable::ePtr e(new Edge(i + nodeId, L"a", shared_ptr<double>(new double(i + nodeId)), nodes[i], nodes[nodeId]));
					graph->addEdge(e);
				}
			}

			return graph;
		}
	};
}
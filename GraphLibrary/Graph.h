#pragma once
#include "Global.h"
#include "IProcessable.h"
#include "Node.h"
#include "Edge.h"

namespace msonlab
{

	class Graph
	{
	private:
		IProcessable::pVect nodes;
		IProcessable::pVect edges;

	public:
		Graph();
		Graph(const Graph& other);

		bool addNode(Node::nPtr toAdd);
		bool addEdge(Edge::ePtr toAdd);

		size_t numberOfNodes() const;
		size_t numberOfEdges() const;
		size_t numberOfOutputs() const;

		bool importGraph(std::istream &in) const;
		bool exportGraph(std::ostream &out) const;
	};

}
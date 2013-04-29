#pragma once
#include "TypeDefinitions.h"

#include "IProcessable.hpp"
#include "Node.hpp"
#include "Edge.hpp"

template <class DataType>
class Graph
{
private:
	typedef vector<IProcessable::pptr> pVect;

	pVect nodes;
	pVect edges;

	pVect outputs;
	pVect inputs;

public:
	Graph();

	friend class NodeIterator;
	NodeIterator findNode(IProcessable::pPtr toFind);

	bool addEdge(const Edge::ePtr toAdd);
	bool addNode(const Node::nPtr toAdd);


	size_t numberOfNodes() const;
	size_t numberOfEdges() const;
	size_t numberOfOutputs() const;


	bool importGraph(istream &in) const;
	bool exportGraph(ostream &out) const;
};



class NodeIterator
{
public:
	bool operator==(const NodeIterator& other) const;
	NodeIterator& operator++();
	NodeIterator operator++(int);
};
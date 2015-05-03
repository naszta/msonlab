#pragma once
#include "Graph.h"
#include "BFSIterator.h"
#include "DFSIterator.h"
#include "NodeTest.h"

#include <stdexcept>

namespace msonlab
{

	using std::swap;

	Graph::Graph()
	{
		_nodes.clear();
		_edges.clear();
		this->iteratorEnd = std::make_shared<NodeTest>(0, L"0", nullptr, 0);
	}

	Graph::Graph(Graph&& other)
	{
		swap(_nodes, other._nodes);
		swap(_edges, other._edges);
		swap(iteratorEnd, other.iteratorEnd);
	}

	Graph::Graph(std::initializer_list<EdgePtr> edges_)
		: iteratorEnd(std::make_shared<NodeTest>(0, L"0", nullptr, 0))
	{
		for (const auto& edge : edges_)
			this->addEdge(edge);
	}

	bool Graph::addNode(NodePtr toAdd)
	{
		auto id = toAdd->id();
		ensureSize(id);
		
		if (_nodes[id] == toAdd) {
			return false;
		}

		if (_nodes[id] != nullptr) {
			throw std::invalid_argument("id already exists in graph");
		}

		_nodes[id] = toAdd;

		return true;
	}

	struct id_compare : public std::unary_function<EdgePtr, bool>
	{
		explicit id_compare(unsigned baseid) : baseid(baseid) {}
		bool operator() (const EdgePtr &arg)
		{
			return baseid == arg->id();
		}
		unsigned baseid;
	};

	bool Graph::addEdge(EdgePtr toAdd)
	{
		msonlab::EdgeVect::iterator it;
		it = std::find_if(_edges.begin(), _edges.end(), id_compare(toAdd->id()));

		if (it != _edges.end())
			return false;

		msonlab::NodePtr fromNode = (*toAdd).getFrom();
		addNode(fromNode);
		msonlab::NodePtr toNode = (*toAdd).getTo();
		addNode(toNode);

		it = _edges.insert(_edges.end(), toAdd);

		if (*it == toAdd)
		{
			(*fromNode).registerSuccessor(toAdd);
			(*toNode).registerPredecessor(toAdd);
			return true;
		}
		else
		{
			throw Exceptions::FailedToAddEdgeException("Failed to add the edge to the graph!");
		}
	}

	size_t Graph::order() const {
		return _nodes.size();
	}

	size_t Graph::size() const {
		return _edges.size();
	}

	const NodeVect& Graph::getNodes() const {
		return _nodes;
	}

	const EdgeVect& Graph::getEdges() const {
		return _edges;
	}

	NodeVect Graph::getInputNodes() const
	{
		NodeVect retVal;

		NodeVect::const_iterator it = _nodes.begin();
		while (it != _nodes.end())
		{
			if ((*(*it)).getPlace() == IProcessable::Input)
				retVal.insert(retVal.end(), (*it));

			++it;
		}
		return retVal;
	}

	NodeVect Graph::getOutputNodes() const
	{
		NodeVect retVal;

		NodeVect::const_iterator it = _nodes.begin();
		while (it != _nodes.end())
		{
			if ((*(*it)).getPlace() == IProcessable::Output)
				retVal.insert(retVal.end(), (*it));

			++it;
		}
		return retVal;
	}

	GraphPtr Graph::getPartialGraphByEdgeType(Edge::EdgeTypeEnum edgeType) const
	{
		GraphPtr ptrGraph = std::make_unique<Graph>();

		EdgeVect::const_iterator it = _edges.begin();
		while (it != _edges.end())
		{
			if ((*(*it)).getEdgeType() == edgeType)
			{
				(*ptrGraph).addEdge((*it));
			}

			++it;
		}

		return ptrGraph;
	}

	Graph::bfs_iterator::bfs_iterator(const Graph& g_) : graph(g_) {}

	BFSIterator Graph::bfs_iterator::begin() const{
		return BFSIterator(graph);
	}

	BFSIterator Graph::bfs_iterator::end() const{
		return BFSIterator(graph.iteratorEnd);
	}

	Graph::dfs_iterator::dfs_iterator(const Graph& g_) : graph(g_) {}

	DFSIterator Graph::dfs_iterator::begin() const{
		return DFSIterator(graph);
	}

	DFSIterator Graph::dfs_iterator::end() const{
		return DFSIterator(graph.iteratorEnd);
	}

	void Graph::ensureSize(size_t size) {
		if (_nodes.size() < size + 1) {
			_nodes.resize(size + 1);
		}
	}
}
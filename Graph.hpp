#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED

//
//  Graph.hpp
//
//  Author: Zoltán Szekeres
//  Date: 2013-02-27
//  Last modified: 2013-03-24
//

#include <boost/shared_ptr.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/optional.hpp>
//#include <boost/graph/graphviz.hpp>
#include <vector>
#include <ostream>
#include <queue>
#include <map>
#include <string>
#include <algorithm> // for_each

#include "IGraph.hpp"
#include "Node.hpp"

namespace zgraph
{
	//using namespace boost;
	using boost::shared_ptr;
	using boost::optional;
	using std::for_each;
	using std::map;
	using std::make_pair;
	using std::pair;
	using std::queue;
	using std::string;
	using std::vector;
	using namespace boost::lambda;

	// This class represents a graph. Adjacency matrix is used to store the graph.

	template<class NodeValue = int>
	class Graph : public IGraph<NodeValue>
	{
	private:
		typedef INode<NodeValue> BNode;
		typedef shared_ptr< INode<NodeValue> > ptr_Node;
		
		int edge_count;
		int capacity; // capacity of matrix
		optional<int> null; // the default value for the matrix
		vector<ptr_Node> nodes; // collection of the nodes in the graph
		vector< vector< optional<int> > > matrix; // adjacency matrix
		map< string, int > nodemap; // maps the outer id of the node to the inner id

		int find_node(const BNode& node) const; // find node in the graph by INode object
		int find_node(const string& node) const; // find node in the graph by label
		void resize(int n); // expand matrix
		void print_graph(ostream &out) const;

	public:
		Graph();
		Graph(int capacity);
		Graph(const Graph<NodeValue>&);
		~Graph() {}

		void add_node(const BNode& node);
		void add_edge(const BNode& source, const BNode& target);
		bool contains_node(const BNode& node) const;
		bool contains_node(const string& node) const;
		bool is_adjacent(const BNode& source, const BNode& target) const;
		bool is_adjacent(const string& source, const string& target) const;
		vector<ptr_Node> get_dependencies(const ptr_Node& node) const;
		vector<ptr_Node> get_neighbours(const ptr_Node& node) const;
		vector<ptr_Node> get_all_nodes() const;
		void erase_node(const BNode&);
		void erase_edge(const BNode&, const BNode&);
		int size() const;

		void clear();

		// breadth first traversal
		void BFT(const string& source_node);

		Graph<NodeValue>& operator=(const Graph<NodeValue>&);

		ostream& printNodes(std::ostream&) const;

		template<class NodeValue>
		friend ostream &operator<<(ostream &out, const Graph<NodeValue> &graph);
		template<class NodeValue>
		friend istream &operator>>(istream &out, Graph<NodeValue> &graph);
	};

	//
	// constructors
	//

	template<class NodeValue>
	Graph<NodeValue>::Graph() : edge_count(0)
	{
		this->resize(10);
	}

	template<class NodeValue>
	Graph<NodeValue>::Graph(int capacity) : edge_count(0)
	{
		this->resize(capacity);
	}

	// copy constructor
	template<class NodeValue>
	Graph<NodeValue>::Graph(const Graph<NodeValue>& graph) : edge_count(graph.edge_count)
	{
		this->resize(this->capacity);
		vector< ptr_Node >::iterator it;
		for(it = graph.nodes.begin(); it != graph.nodes.end(); i++)
		{
			this->add_node(*(*it));
		}

		for(int i = 0; i < graph.matrix.size(); i++)
		{
			for(int j = 0; j < graph.matrix[i].size; ++j)
			{
				this->matrix[i][j] = graph.matrix[i][j];
			}
		}
	}

	//
	// private methods
	//
	
	// finds the id of a certain node
	// returns the id if found; otherwise -1
	template<class NodeValue>
	int Graph<NodeValue>::find_node(const BNode& node) const
	{
		return this->find_node(node.get_label());
	}

	// finds the id of a certain node
	// returns the id if found; otherwise -1
	template<class NodeValue>
	int Graph<NodeValue>::find_node(const string& node) const
	{
		map<string, int>::const_iterator cit = this->nodemap.find(node);
		if (cit != this->nodemap.cend())
		{
			return cit->second;
		}

		return -1;
	}

	// resizes the graph so that it can contain n nodes
	template<class NodeValue>
	void Graph<NodeValue>::resize(int n)
	{
		if (capacity < n)
		{
			// an empty row
			vector< optional<int> > v(n, this->null);
			
			// adding new columns to existing rows
			for (unsigned i = 0; i < this->matrix.size(); ++i)
			{
				this->matrix[i].resize(n, this->null);
			}

			// adding new rows
			this->matrix.resize(n, v);
			capacity = n;
		}
	}

	// print the graph nodes to the stream
	template<class NodeValue>
	void Graph<NodeValue>::print_graph(ostream &out) const
	{
		out << "digraph G {" << std::endl;
		for (unsigned i = 0; i < this->nodes.size(); ++i)
		{
			// print node
			out << i << "[label=" << this->nodes[i]->get_label() << "];" << std::endl;

			// print edges from this node
			for (unsigned j = 0; j < this->nodes.size(); ++j)
			{
				if (i == j) continue;
				if (this->matrix[i][j])
				{
					out << i << "->" << j << " ;" << std::endl;
				}
			}
		}

		out << "}";

		// collecting labels
		//char ** labels = new char*[this->size()];
		//for (unsigned i = 0; i < this->nodes.size(); ++i)
		//{
		//	string label = this->nodes[i]->get_label();
		//	labels[i] = new char[label.size()+1];
		//	labels[i][label.size()] = 0;
		//	memcpy(labels[i], label.c_str(), label.size());
		//}

		//// collecting edges
		//vector< pair<int, int> > edges;
		//for(unsigned i = 0; i < this->matrix.size(); ++i)
		//{
		//	for(unsigned j = 0; j < this->matrix[i].size(); ++j)
		//	{
		//		if (this->matrix[i][j])
		//		{
		//			edges.push_back(make_pair(i,j));
		//		}
		//	}
		//}

		//vector<int> weights(this->edge_count, 1);

		// typedef adjacency_list< vecS, vecS, directedS, 
		//	  property< vertex_color_t, default_color_type >,
		//	  property< edge_weight_t, int >
		//	> GVGraph;
		// GVGraph g(&edges[0], &edges[0]+this->edge_count, &weights[0], this->size());

		// write_graphviz(out, g, make_label_writer(labels));

		// for (unsigned i = 0; i < this->nodes.size(); ++i)
		// {
		//	 delete[] labels[i];
		// }

		// delete[] labels;
	}

	//
	// modifying methods
	//

	// add a node, if it is not already in the graph
	template<class NodeValue>
	void Graph<NodeValue>::add_node(const BNode& node)
	{
		if(this->find_node(node) == -1)
		{
			if (this->capacity == this->size())
			{
				// resizing the graph with 10 nodes
				this->resize(this->capacity + 10);
			}

			ptr_Node pNode = shared_ptr< Node<NodeValue> >( new Node<NodeValue>(node) );

			//adding to map
			this->nodemap[node.get_label()] = this->nodes.size();

			// add to list
			this->nodes.push_back(pNode);
		}	
	}

	// add edge to the graph
	// if nodes not in the graph, the nodes are added
	template<class NodeValue>
	void Graph<NodeValue>::add_edge(const BNode& source, const BNode& target)
	{
		this->add_node(source);
		this->add_node(target);
		int sId = this->find_node(source);
		int tId = this->find_node(target);

		// if the edge does not yet exist
		if (!this->matrix[sId][tId])
		{
			this->matrix[sId][tId] = 1;
			this->edge_count++;
		}
	}
	
	// erease a node if it is in the graph
	template<class NodeValue>
	void Graph<NodeValue>::erase_node(const BNode& node)
	{
		int id = this->find_node(node);
		if (id != -1)
		{
			// erasing node from matrix
			this->matrix.erase(this->matrix.begin() + id);
			for(unsigned i = 0; i < this->matrix.size(); ++i)
			{
				this->matrix[i].erase(this->matrix[i].begin() + id);
			}

			// erase node from nodes vector
			this->nodes.erase(this->nodes.begin() + id);

			// actualize nodemap
			for (unsigned i = id; i < this->nodes.size(); ++i)
			{
				this->nodemap[this->nodes[i]->get_label()] = i;
			}
		}
	}

	// erease an edge
	template<class NodeValue>
	void Graph<NodeValue>::erase_edge(const BNode& source, const BNode& target)
	{
		int sId = this->find_node(source);
		int tId = this->find_node(target);
		if (sId > -1 && tId > -1 && this->matrix[sId][tId])
		{
			this->matrix[sId][tId] = this->null;
			this->edge_count--;
		}
	}

	// Erases every element of the graph
	template<class NodeValue>
	void Graph<NodeValue>::clear()
	{
		this->nodes.clear();
		for (unsigned i = 0; i < this->matrix.size(); ++i)
			this->matrix[i].clear();
		this->matrix.clear();
		this->nodemap.clear();
	}

	//
	// query methods
	//

	// return true if the graph contains the node; otherwise false
	template<class NodeValue>
	bool Graph<NodeValue>::contains_node(const BNode& node) const
	{	
		return this->contains_node(node.get_label());
	}

	// return true if the graph contains the node; otherwise false
	template<class NodeValue>
	bool Graph<NodeValue>::contains_node(const string& node) const
	{
		// if graph is empty, return false
		if (this->size() == 0) return false;
		
		return this->find_node(node) != -1;
	}

	// return true if the graph contains the edge; otherwise false
	template<class NodeValue>
	bool Graph<NodeValue>::is_adjacent(const BNode& source, const BNode& target) const
	{
		return this->is_adjacent(source.get_label(), target.get_label());
	}

	// return true if the graph contains the edge; otherwise false
	template<class NodeValue>
	bool Graph<NodeValue>::is_adjacent(const string& source, const string& target) const
	{
		int sId = this->find_node(source);
		int tId = this->find_node(target);

		if (sId != -1 && tId != -1)
		{
			return matrix[sId][tId];
		}

		return false;
	}

	// gets those nodes, who have an edge to node
	template<class NodeValue>
	vector< shared_ptr< INode<NodeValue> > > Graph<NodeValue>::get_dependencies(const shared_ptr< INode<NodeValue> >& node) const
	{
		vector< shared_ptr< INode<NodeValue> > > result;
		int id = this->find_node(node->get_label());
		if (id != -1)
		{
			for (unsigned i = 0; i < this->matrix[id].size(); ++i)
			{
				if (this->matrix[i][id])
				{
					// there is an edge form i to id
					// put node i in the result vector
					result.push_back(this->nodes[i]);
				}
			}
		}

		return result;
	}

	// gets those nodes, who has an edge from node
	template<class NodeValue>
	vector< shared_ptr< INode<NodeValue> > > Graph<NodeValue>::get_neighbours(const shared_ptr< INode<NodeValue> >& node) const
	{
		vector< shared_ptr< INode<NodeValue> > > result;
		int id = this->find_node(node->get_label());
		if (id != -1)
		{
			for (unsigned i = 0; i < this->matrix[id].size(); ++i)
			{
				if (this->matrix[id][i])
				{
					// there is an edge form id to i
					// put node i in the result vector
					result.push_back(this->nodes[i]);
				}
			}
		}

		return result;
	}

	// gets all the nodes in the graph
	template<class NodeValue>
	vector< shared_ptr< INode<NodeValue> > > Graph<NodeValue>::get_all_nodes() const
	{
		return this->nodes;
	}

	// returns the number of nodes in the graph
	template<class NodeValue>
	int Graph<NodeValue>::size() const
	{
		return this->nodes.size();
	}

	template<class NodeValue>
	Graph<NodeValue>& Graph<NodeValue>::operator=(const Graph<NodeValue> &base)
	{
		this->Clear();
		for(int i = 0; i < base.GetNodeSize(); i++)
		{
			this->AddNode(base.nodes[i]);
		}
		for(int i = 0; i < base.GetEdgeSize(); i++)
		{
			this->AddEdge(base.edges[i]);
		}
	}

	// print the nodes to the given ostream object
	template<class NodeValue>
	ostream& Graph<NodeValue>::printNodes(std::ostream& out) const
	{
		for_each(this->nodes.begin(), this->nodes.end(), out << (*_1));
		return out;
	}

	// traversals
	template<class NodeValue>
	void Graph<NodeValue>::BFT(const string& source_node)
	{
		if (this->nodemap.count(source_node) < 1)
		{
			std::cout << "Graph does not contain start node." << std::endl;
			return;
		}

		vector<bool> visited(this->size(), false);
		queue<int> q;
		q.push(nodemap[source_node]);
		int traversed = 0; // # of traversed nodes (in case the graph is disconnected)
		int lastchosen = 0;
		std::cout << "Travesing Graph" << std::endl;
		while(traversed < this->size)
		{
			while(!q.empty())
			{
				int node = q.front();
				q.pop();
				if (visited[node]) continue; // this node is already visited. Can NOT happen in a tree.
				++ traversed;
				std::cout << "Node " << this->nodes[node]->get_label() << ", Value: " << this->nodes[node]->get_value() << std::endl;
				visited[node] = true;
				for (int i = 0; i < this->size; ++i)
				{
					if (matrix[node][i]) // this edge exists
					{
						if (!visited[i])
						{
							q.push(i);
						}
					}
				}
			}

			// choose another node from a not visited component
			if (traversed < this->size())
			{
				std::cout << "Choosing node from a nonvisited component." << std::endl;
				for (int i = lastchosen+1; i < this->size; ++i)
				{
					if (!visited[i])
					{
						q.push(i);
						lastchosen = i;
						break;
					}
				}
			}
		}
	}

	// writes the graph to the stream
	// nodes in one line, edges in E lines
	// N : # of nodes E : # of edges
	// V : values of the nodes in order of the ids
	// S : source node of the edge T : target node of the edge V : value of the edge
	template<class NodeValue>
	ostream &operator<<(ostream &out, const Graph<NodeValue> &graph)
	{
		graph.print_graph(out);
		return out;
	}

	// read a graph from the stream
	// the format equals with the format of writing graphs
	template<class NodeValue>
	istream &operator>>(istream &in, Graph<NodeValue> &graph)
	{
		return in;
	}
}
#endif
#ifndef IGraph_HPP_INCLUDED
#define IGraph_HPP_INCLUDED

//
//  IGraph.hpp
//
//  Author: Zoltán Szekeres
//  Date: 2013-03-19
//  Last modified: 2013-03-24
//

#include <ostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stack>
#include <string>
#include <boost\function.hpp>
#include <boost\shared_ptr.hpp>

namespace zgraph
{
	using boost::function;
	using boost::shared_ptr;
	using std::map;
	using std::ostream;
	using std::queue;
	using std::set;
	using std::stack;
	using std::string;
	using std::vector;

	template<class NodeValue = int>
	class INode
	{
	public :
		virtual void set_label(const string& nl) = 0;
		virtual string get_label() const = 0;
		virtual void set_value(const NodeValue& nv) = 0;
		virtual NodeValue get_value() const = 0;
		virtual shared_ptr<NodeValue> operator()(vector< shared_ptr<NodeValue> >) = 0;

		template<class NodeValue>
		friend ostream &operator<<(ostream &out, const INode<NodeValue> &node)
		{
			node.print_node(out);
			return out;
		}

	private :
		virtual void print_node(ostream& out) const = 0;
	};

	template<class NodeValue = int>
	class IGraph
	{
		typedef INode<NodeValue> BNode;
	public:
		virtual void add_node(const BNode& node) = 0;
		virtual void add_edge(const BNode& source, const BNode& target) = 0;
		virtual bool contains_node(const BNode& node) const = 0;
		virtual bool contains_node(const string& node) const = 0;
		virtual bool is_adjacent(const BNode& source, const BNode& target) const = 0;
		virtual bool is_adjacent(const string& source, const string& target) const = 0;
		virtual vector< shared_ptr<BNode> > get_dependencies(const shared_ptr<BNode>& node) const = 0;
		virtual vector< shared_ptr<BNode> > get_neighbours(const shared_ptr<BNode>& node) const = 0;
		virtual vector< shared_ptr<BNode> > get_all_nodes() const = 0;
		virtual void erase_node(const BNode& node) = 0;
		virtual void erase_edge(const BNode& source, const BNode& target) = 0;
		virtual int size() const = 0;
		virtual void clear() = 0;

		template<class NodeValue>
		friend ostream &operator<<(ostream &out, const IGraph<NodeValue> &graph)
		{
			graph.print_graph(out);
			return out;
		}

	private:
		virtual void print_graph(ostream &out) const = 0;
	};

	template<class NodeValue = int>
	class IGraphAlgorithms
	{
	private:
		typedef INode<NodeValue> BNode;
		typedef IGraph<NodeValue> BGraph;
	public:
		void IsDAG(const BGraph& graph);
		void BFS(const BNode& node, const BGraph& graph, const boost::function<void (const BNode&)> process);
		void DFS(const BNode& node, const BGraph& graph, const boost::function<void (const BNode&)> process);
		void print_graph(ostream& out, const BGraph& graph);
	};

	template<class NodeValue>
	void IGraphAlgorithms<NodeValue>::print_graph(ostream& out, const IGraph<NodeValue>& graph)
	{
		vector< shared_ptr<BNode> > nodes = graph.get_all_nodes();
		map< shared_ptr<BNode>, int > nodemap;
		for(unsigned i = 0; i < nodes.size(); ++i)
		{
			nodemap[nodes[i]] = i;
		}

		out << "digraph G {" << std::endl;
		for(unsigned i = 0; i < nodes.size(); ++i)
		{
			out << i << "[label=" << nodes[i]->get_label() << "];" << std::endl;
			vector< shared_ptr<BNode> > neighbours = graph.get_neighbours(nodes[i]);
			for (unsigned j = 0; j < neighbours.size(); ++j)
			{
				out << i << "->" << nodemap[neighbours[j]] << " ;" << std::endl;
			}
		}

		out << "}";
	}

	// Travesing a graph with breadth-first search
	template<class NodeValue>
	void IGraphAlgorithms<NodeValue>::BFS(const BNode& bnode, const BGraph& graph, const boost::function<void (const BNode&)> process)
	{
		set< shared_ptr<BNode> > visited; // store visited values
		queue< shared_ptr<BNode> > q; // nodes to be travesed
		q.push(bnode);
		vector< shared_ptr<BNode> >::iterator it;
		vector< shared_ptr<BNode> > allnodes = graph.get_all_nodes(); // vector of all nodes
		vector< shared_ptr<BNode> >::iterator node_it = allnodes.begin();
		while(visited.size() < graph.size())
		{
			while(q.size() > 0)
			{
				shared_ptr<BNode> node = q.front();
				q.pop();
			
				// if already visited, than skip node
				if (visited.count(node) > 0) continue;
			
				// add node to visited
				visited.insert(node);

				// processing node
				process(node);

				// adding neighbours to queue
				vector< shared_ptr<BNode> > neighbours = graph.get_neighbours(node);
				for (it = neighbours.begin(); it != neighbours.end(); ++it)
				{
					q.push(*it);
				}
			}

			// searching for another component
			if (visited.size() < graph.size())
			{
				// searching for a untraversad node
				while(visited.count(*node_it) > 0)
				{
					++node_it;
				}

				// pushing unvisited node to the queue
				q.push(*node_it);
			}
		}
	}

	// Travesing a graph with depth-first search
	template<class NodeValue>
	void IGraphAlgorithms<NodeValue>::DFS(const BNode& bnode, const BGraph& graph, const boost::function<void (const BNode&)> process)
	{
		set< shared_ptr<BNode> > explored;
		set< shared_ptr<BNode> > discovered;
		// stack to keep track discovered nodes
		stack< shared_ptr<BNode> > st;
		vector< shared_ptr<BNode> >::iterator it;
		st.push( shared_ptr<BNode>(new BNode(bnode)));
		while(!st.empty())
		{
			shared_ptr<BNode> node = st.top();
			discovered.insert(node);
			vector< shared_ptr<BNode> > neighbours = graph.get_neighbours(node);
			bool is_explored = true;

			// check neighbours
			for(it = neigbours.begin(); it != neighbours.end(); ++it)
			{
				// if there is a undiscovered neighbour, push to stack, and this cannot be explored
				if (discovered.count(*it) == 0)
				{
					st.push(*it);
					is_explored = false;
				}
			}

			// if all neighbour is discovered, it is explored
			if (is_explored)
			{
				explored.insert(node);
				st.pop();

				process(*node);
			}
		}
	}
}

#endif
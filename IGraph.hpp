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
	class INode;

	template<class Value = int>
	class IEdge
	{
		virtual shared_ptr< INode<Value> > get_source() const = 0;
		virtual shared_ptr< INode<Value> > get_target() const = 0;
		virtual shared_ptr< Value > operator()(shared_ptr<Value>) = 0;
	};

	template<class NodeValue>
	class INode
	{
	public :
		//virtual INode<NodeValue>& operator=(const INode<NodeValue>& node);

		virtual void set_label(const string& nl) = 0;
		virtual string get_label() const = 0;
		virtual void set_value(const NodeValue& nv) = 0;
		virtual NodeValue get_value() const = 0;
		virtual shared_ptr<NodeValue> operator()(vector< shared_ptr<NodeValue> >) = 0;
		virtual bool operator==(const INode<NodeValue>& other) const = 0;
		virtual bool operator!=(const INode<NodeValue>& other) const = 0;

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
		class BFS_iterator;

		void IsDAG(const BGraph& graph);
		void BFS(const BNode& node, const BGraph& graph, const boost::function<void (const BNode&)> process);
		void DFS(const BNode& node, const BGraph& graph, const boost::function<void (const BNode&)> process);
		void print_graph(ostream& out, const BGraph& graph);
		vector< shared_ptr<BNode> > topological_order(const shared_ptr<BGraph>& graph);
		shared_ptr<BGraph> get_changed_graph(const shared_ptr<BGraph>& graph, const set< shared_ptr<BNode> >&, const set< shared_ptr<BNode> >&);

		//template<class NodeValue = int>
		class BFS_iterator
		{
		protected:
			shared_ptr<BNode> p;
			shared_ptr<BNode> it_end;
			shared_ptr<BGraph> graph;
			queue< shared_ptr<BNode> > q;
			vector< shared_ptr<BNode> > nodes;
			set< shared_ptr<BNode> > visited;
	
			virtual void move_next()
			{
				while (visited.count(p) > 0)
				{
					if (q.empty())
					{
						p = it_end;
						return;
					}

					p = q.front();
					q.pop();
				}
			
				// add node to visited
				visited.insert(p);

				// adding neighbours to queue
				vector< shared_ptr<BNode> > neighbours = graph->get_neighbours(p);
				vector< shared_ptr<BNode> >::iterator it;
				for (it = neighbours.begin(); it != neighbours.end(); ++it)
				{
					if (visited.count(*it) == 0)
					{
						q.push(*it);
					}
				}
			}

		public:
			BFS_iterator(shared_ptr<BGraph> graph) : graph(graph) 
			{
				this->nodes = graph->get_all_nodes();
				this->it_end = shared_ptr<BNode>(new Node<NodeValue>());
				this->it_end->set_label("_iterator_end");
				this->p = this->nodes.front();
				this->q.push(this->p);
				this->move_next();
			}

			BFS_iterator(BFS_iterator& it)
			{
				if (this->p != it.p)
				{
					*this = it;
				}
			}

			BFS_iterator& operator=(BFS_iterator& it)
			{
				this->it_end = it.it_end;
				this->p = *it;
				this->graph = it.graph;
				this->nodes = it.nodes;
				this->q = it.q;
				this->visited = it.visited;
				return *this; 
			}

			BFS_iterator& operator++() // prefix
			{
				this->move_next();
				return *this;
			}

			BFS_iterator operator++(int) // postfix
			{
				BFS_iterator bfs_it = *this;
				this->move_next();
				return bfs_it;
			}

			// Compares by node
			bool operator==(BFS_iterator& it) const
			{
				return *p == **it;
			}

			// Compares by node
			bool operator!=(BFS_iterator& it) const
			{
				return *p != **it;
			}

			void set_start(shared_ptr<BNode> start)
			{
				this->clear();
				this->p = start;
				this->q.push(p);
				this->move_next();
			}

			// Mark it's neighbours visited, so the move_next will skip them
			virtual void skip()
			{
				vector< shared_ptr<BNode> > neighbours = graph->get_neighbours(p);
				vector< shared_ptr<BNode> >::iterator it;
				for (it = neighbours.begin(); it != neighbours.end(); ++it)
				{
					this->visited.insert(*it);
				}
			}

			shared_ptr<BNode> operator*() const
			{
				return this->p;
			}

			BFS_iterator end() const
			{
				BFS_iterator it(this->graph);
				it.p = this->it_end;
				return it;
			}

			void clear()
			{
				this->p = this->it_end;
				this->visited.clear();
				for(;!this->q.empty();q.pop());
			}
		};

		class RBFS_iterator : public BFS_iterator
		{
		protected:
			void move_next()
			{
				while (visited.count(p) > 0)
				{
					if (q.empty())
					{
						p = it_end;
						return;
					}

					p = q.front();
					q.pop();
				}
			
				// add node to visited
				visited.insert(p);

				// adding neighbours to queue
				vector< shared_ptr<BNode> > neighbours = graph->get_dependencies(p);
				vector< shared_ptr<BNode> >::iterator it;
				for (it = neighbours.begin(); it != neighbours.end(); ++it)
				{
					if (visited.count(*it) == 0)
					{
						q.push(*it);
					}
				}
			}
		public :
			RBFS_iterator(shared_ptr<BGraph> graph) : BFS_iterator(graph)
			{
			}

			// Compares by node
			bool operator==(RBFS_iterator& it) const
			{
				return *p == **it;
			}

			// Compares by node
			bool operator!=(RBFS_iterator& it) const
			{
				return *p != **it;
			}

			RBFS_iterator end() const
			{
				RBFS_iterator it(this->graph);
				it.p = this->it_end;
				return it;
			}

			void skip()
			{
				vector< shared_ptr<BNode> > neighbours = graph->get_dependencies(p);
				vector< shared_ptr<BNode> >::iterator it;
				for (it = neighbours.begin(); it != neighbours.end(); ++it)
				{
					this->visited.insert(*it);
				}
			}
		};
	};
	
	// Returns a topological order of the nodes
	template<class Value>
	vector< shared_ptr< INode<Value> > > IGraphAlgorithms<Value>::topological_order(const shared_ptr<BGraph>& graph)
	{
		vector< shared_ptr<BNode> > order;
		vector< shared_ptr<BNode> > all_nodes = graph->get_all_nodes();
		vector< shared_ptr<BNode> >::iterator it;
		map< shared_ptr<BNode> , int> count;
		queue< shared_ptr<BNode> > q;
		for(it = all_nodes.begin(); it != all_nodes.end(); ++it)
		{
			vector< shared_ptr<BNode> > neighbours = graph->get_dependencies(*it);
			if (neighbours.size() == 0)
			{
				q.push(*it);
			}
		}

		while(!q.empty())
		{
			shared_ptr<BNode> akt = q.front();
			q.pop();
			order.push_back(akt);
			vector< shared_ptr<BNode> > neighbours = graph->get_neighbours(akt);
			for (unsigned i = 0; i < neighbours.size(); ++i)
			{
				count[neighbours[i]]++;
				if ( graph->get_dependencies(neighbours[i]).size() == count[neighbours[i]])
				{
					q.push(neighbours[i]);
				}
			}
		}

		return order;
	}

	// Writes the graph in to the outstream
	// Format: DOT language
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

	// calculates a partial graph, that needs to be recalculated,
	// in order to know the new values of <needed>
	template<class NodeValue>
	shared_ptr< IGraph<NodeValue> > IGraphAlgorithms<NodeValue>::get_changed_graph(const shared_ptr<BGraph>& graph, const  set< shared_ptr<BNode> >& changed, const set< shared_ptr<BNode> >& needed)
	{
		vector< shared_ptr<BNode> > topological_order = this->topological_order(graph);
		set< shared_ptr<BNode> >::iterator it;

		// finding maximum position in topological order
		int max_node_top_value = -1;
		for (int i = topological_order.size()-1; i >= 0; --i)
		{
			if (needed.count(topological_order[i]) > 0)
			{
				max_node_top_value = i;
				break;
			}
		}

		// starting bfs from every changed node
		// stops if topological number is bigger than
		// the maximum of <needed>
		set< shared_ptr<BNode> > modified;
		BFS_iterator bfs_it(graph);
		for( it = changed.cbegin(); it != changed.cend(); ++it)
		{
			bfs_it.set_start(*it);
			for(; bfs_it != bfs_it.end(); ++bfs_it)
			{
				if (modified.count(*bfs_it) > 0)
				{
					bfs_it.skip();
				}
				else
				{
					modified.insert(*bfs_it);
				}
			}
		}

		// collecting nodes into a new graph
		shared_ptr< IGraph<int> > nodeset(new Graph<int>());
		// reverse bfs from here
		RBFS_iterator rbfs_it(graph);
		RBFS_iterator temp_it(graph);
		for( it = needed.cbegin(); it != needed.cend(); ++it)
		{
			rbfs_it.set_start(*it);
			temp_it.set_start(*it);
			for(; rbfs_it != rbfs_it.end();)
			{
				++rbfs_it;
				if (modified.count(*rbfs_it) > 0)
				{
					nodeset->add_edge(**rbfs_it, **temp_it);
				}
				else
				{
					rbfs_it.skip();
				}

				++temp_it;
			}
		}

		return nodeset;
	}
}

#endif
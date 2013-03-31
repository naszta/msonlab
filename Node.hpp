#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

//
//  Node.hpp
//
//  Author: Zoltán Szekeres
//  Date: 2013-02-27
//  Last modified: 2013-03-24
//

#include <boost/shared_ptr.hpp>
#include <ostream>
#include <istream>
#include <string>
#include "IGraph.hpp"

namespace zgraph
{
	using boost::shared_ptr;
	using std::istream;
	using std::ostream;
	using std::string;

	// Represents a Node in the Graph
	template<class NodeValue = int>
	class Node : public INode<NodeValue>
	{
	private:
		shared_ptr<NodeValue> value;
		string label;

		void print_node(ostream& out) const;

	public:
		Node();
		Node(string label);
		Node(string label, const NodeValue& nv);
		Node(const INode<NodeValue>& node);

		void set_label(const string& nl);
		string get_label() const;
		void set_value(const NodeValue& nv);
		NodeValue get_value() const;
		shared_ptr<NodeValue> operator()(vector< shared_ptr<NodeValue> >);
		~Node() {}

		Node<NodeValue>& operator=(Node<NodeValue>& node);
		bool operator==(const Node<NodeValue>& other) const;
		bool operator!=(const Node<NodeValue>& other) const;

		template<class NodeValue>
		friend ostream &operator<<(ostream &out, const Node<NodeValue> &node);
		template<class NodeValue>
		friend istream &operator>>(istream &out, Node<NodeValue> &node);
	};

	// constructors
	template<class NodeValue>
	Node<NodeValue>::Node()
	{}

	template<class NodeValue>
	Node<NodeValue>::Node(string label) : label(label)
	{}
	
	template<class NodeValue>
	Node<NodeValue>::Node(string label, const NodeValue& nv) : label(label)
	{
		this->set_value(nv);
	}

	// copy constructor
	template<class NodeValue>
	Node<NodeValue>::Node(const INode<NodeValue>& node) : label(node.get_label())
	{
		shared_ptr<NodeValue> val(new NodeValue(node.get_value()));
		this->value.swap(val);
	}

	//
	// private methods
	//

	template<class NodeValue>
	void Node<NodeValue>::print_node(ostream& out) const
	{
		out << this->get_label() << " " <<  this->get_value();
	}

	// methods

	// sets the label of the node
	template<class NodeValue>
	void Node<NodeValue>::set_label(const string& nl)
	{
		this->label = nl;
	}

	// returns the label of the node
	template<class NodeValue>
	string Node<NodeValue>::get_label() const
	{
		return this->label;
	}

	// sets the value of the node
	template<class NodeValue>
	void Node<NodeValue>::set_value(const NodeValue& nv)
	{
		this->value = shared_ptr<NodeValue>(new NodeValue(nv));
	}

	// returns the value of the Node
	template<class NodeValue>
	NodeValue Node<NodeValue>::get_value() const
	{
		return *this->value;
	}

	template<class NodeValue>
	shared_ptr<NodeValue> Node<NodeValue>::operator()(vector< shared_ptr<NodeValue> > vals)
	{
		// TODO: process vals
		return this->value;
	}

	// operators
	template<class NodeValue>
	Node<NodeValue>& Node<NodeValue>::operator=(Node<NodeValue>& base)
	{
		shared_ptr<NodeValue> v(new NodeValue(*base.value));

		this->value.swap(v);
		this->label = base.label;

		return *this;
	}

	// comparing nodes by label
	template<class NodeValue>
	bool Node<NodeValue>::operator==(const Node<NodeValue>& other) const
	{
		return this->label.compare(other.label) == 0;
	}

	// comparing nodes by label
	template<class NodeValue>
	bool Node<NodeValue>::operator!=(const Node<NodeValue>& other) const
	{
		return this->label.compare(other.label) != 0;
	}

	// write node value to stream
	template<class NodeValue>
	ostream &operator<<(ostream &out, const Node<NodeValue> &node)
	{
		out << node.get_label() << " " <<  node.get_value();
		return out;
	}

	// read node value from stream
	template<class NodeValue>
	istream &operator>>(istream &in, Node<NodeValue> &node)
	{
		NodeValue val;
		in >> node.label >> val;
		node.value = shared_ptr<NodeValue>(new V(val));
		return in;
	}
}

#endif // ! NODE_HPP_INCLUDED

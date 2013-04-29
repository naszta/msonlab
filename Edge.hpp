#pragma once
#include "iprocessable.hpp"
#include "Node.hpp"

template <class DataType>
class Edge : public IProcessable<DataType>
{
private:
	Node::nPtr from;
	Node::nPtr to;

public:
	typedef boost::shared_ptr<Edge<DataType>> ePtr;

	Edge(unsigned int _id, wchar_t _label, DataType _value, const Node::nPtr x, const Node::nPtr y)  : id(_id), label(_label), value(_value);
	Edge(const Edge& other); 

	bool process();

	Node::nPtr opposite(const Node::nPtr x);

	Node::nPtr getFrom() const;
	Node::nPtr getTo() const;
};


#pragma once
#include "iprocessable.hpp"
#include "Edge.hpp"
#include <vector>

using std::vector;

template <class DataType>
class Node : public IProcessable<DataType>
{
private:
	typedef boost::shared_ptr<Node<DataType>> nPtr;
	typedef vector<Edge::eptr> eVect;

	eVect predecessors;
	eVect successors;

public:
	enum PlaceEnum  {Input, Inside, Output};

	Node(unsigned int _id, wchar_t _label, DataType _value)  : id(_id), label(_label), value(_value);
	Node(const Node& other); 

	bool process();

	eVect getPredecessors() const;
	eVect getSuccessors() const;

	PlaceEnum getPlace() const;
};


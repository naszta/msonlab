#pragma once
#include "Global.h"
#include "IProcessable.h"

namespace msonlab
{
	class Node : public IProcessable
	{
	private:
		IProcessable::pVect predecessors;
		IProcessable::pVect successors;

	public:
		typedef boost::shared_ptr<Node> nPtr;

		enum PlaceEnum {Input, Inside, Output};

		Node(unsigned int _id, wchar_t _label, Types::DataType _value);
		Node(const Node& other);

		bool process();

		IProcessable::pVect getPredecessors() const;
		IProcessable::pVect getSuccessors() const;

		PlaceEnum getPlace() const;

	};

}
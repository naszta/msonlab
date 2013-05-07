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

		int paramCount;

	public:
		typedef boost::shared_ptr<Node> nPtr;

		enum PlaceEnum {Input, Inside, Output};

		Node(unsigned int _id, wchar_t _label, Types::DataType _value);
		Node(const Node& other);

		bool registerParameter();

		virtual bool process();
		virtual bool isReadyForProcess() const;

		IProcessable::pVect getPredecessors() const;
		IProcessable::pVect getSuccessors() const;

		PlaceEnum getPlace() const;

	};

}
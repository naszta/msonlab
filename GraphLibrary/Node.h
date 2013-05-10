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

		

		Node(unsigned int _id, wchar_t _label, Types::DataType _value);
		Node(const Node& other);

		bool registerParameter();

		virtual IProcessable::pVect process();
		virtual bool isReadyForProcess() const;
		virtual bool resetProcessingState();

		IProcessable::pVect getPredecessors() const;
		IProcessable::pVect getSuccessors() const;

		bool registerPredecessor(IProcessable::pPtr _newPredecessor);
		bool unregisterPredecessor(IProcessable::pPtr _newPredecessor);

		bool registerSuccessor(IProcessable::pPtr _newSuccessor);
		bool unregisterSuccessor(IProcessable::pPtr _newSuccessor);

		PlaceEnum getPlace() const;

	};

}
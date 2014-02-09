#pragma once
#include "Global.h"
#include "IProcessable.h"

namespace msonlab
{
	class Node : public IProcessable
	{
	protected:
		IProcessable::eVect predecessors;
		IProcessable::eVect successors;

		int paramCount;

	public:
		//typedef boost::shared_ptr<Node> nPtr;
		//typedef vector<boost::shared_ptr<Node>> nVect;
		

		Node(unsigned int _id, wchar_t _label, Types::DataType _value);
		Node(const Node& other);

		bool registerParameter();

		virtual IProcessable::pVect process();
		virtual bool isReadyForProcess() const;
		virtual bool resetProcessingState();

		IProcessable::eVect getPredecessors() const;
		size_t getPredecessorsSize() const;
		IProcessable::ePtr getPredecessor(size_t index) const;
		IProcessable::eVect::iterator getPredecessorBegin();
		IProcessable::eVect::iterator getPredecessorEnd();
		IProcessable::eVect getSuccessors() const;

		bool registerPredecessor(IProcessable::ePtr _newPredecessor);
		bool unregisterPredecessor(IProcessable::ePtr _newPredecessor);

		bool registerSuccessor(IProcessable::ePtr _newSuccessor);
		bool unregisterSuccessor(IProcessable::ePtr _newSuccessor);

		PlaceEnum getPlace() const;

		// compile
		virtual void compile(msonlab::StackRunner::srPtr stackProgram);
	};

}
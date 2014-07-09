#pragma once
#include "IProcessable.h"

namespace msonlab
{
	class Node : public IProcessable
	{
	protected:
		IProcessable::eVect predecessors;
		IProcessable::eVect successors;

		int paramCount;
		unsigned compTime;

	public:
		Node(unsigned int _id, Types::LabelType _label, Types::DataType _value);
		Node(unsigned int _id, Types::LabelType _label, Types::DataType _value, unsigned compTime);
		Node(const Node& other);

		bool registerParameter();

		virtual IProcessable::pVect process();
		virtual bool isReadyForProcess() const;
		virtual bool resetProcessingState();

		const IProcessable::eVect& getPredecessors() const;
		size_t getPredecessorsSize() const;
		IProcessable::ePtr getPredecessor(size_t index) const;
		IProcessable::eVect::iterator getPredecessorBegin();
		IProcessable::eVect::iterator getPredecessorEnd();
		const IProcessable::eVect& getSuccessors() const;
		size_t getSuccessorsSize() const;

		bool registerPredecessor(IProcessable::ePtr _newPredecessor);
		bool unregisterPredecessor(IProcessable::ePtr _newPredecessor);

		bool registerSuccessor(IProcessable::ePtr _newSuccessor);
		bool unregisterSuccessor(IProcessable::ePtr _newSuccessor);

		PlaceEnum getPlace() const;

		virtual unsigned getComputationTime() { return compTime; }

		// compile
		virtual void compile(msonlab::StackRunner::srPtr stackProgram);
	};

}
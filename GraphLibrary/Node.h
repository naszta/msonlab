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
		//typedef std::shared_ptr<Node> nPtr;
		//typedef vector<std::shared_ptr<Node>> nVect;
		

		Node(unsigned int _id, std::string _label, Types::DataType _value);
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

		std::string getIdString() const;

		bool registerPredecessor(IProcessable::ePtr _newPredecessor);
		bool unregisterPredecessor(IProcessable::ePtr _newPredecessor);

		bool registerSuccessor(IProcessable::ePtr _newSuccessor);
		bool unregisterSuccessor(IProcessable::ePtr _newSuccessor);

		PlaceEnum getPlace() const;

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		virtual DOMElement* serialize(DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey);
		virtual std::string getTypeString() const;
		virtual std::string get_shape() const;
		virtual std::string get_color() const;
	};

}
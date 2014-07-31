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
		std::string type_string;

	public:
		Node(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		Node(unsigned int _id, types::LabelType _label, types::DataPtr _value, unsigned compTime);
		Node(const Node& other);

		Node& operator=(const Node& other);

		bool registerParameter();

		virtual IProcessable::pVect process();
		virtual bool isReadyForProcess() const;
		virtual bool resetProcessingState();

		const IProcessable::eVect& getPredecessors() const;
		size_t getPredecessorsSize() const;
		unsigned getPredecessorNodeId(size_t index) const;
		const IProcessable::ePtr& getPredecessor(size_t index) const;
		IProcessable::eVect::iterator getPredecessorBegin();
		IProcessable::eVect::iterator getPredecessorEnd();
		const IProcessable::eVect& getSuccessors() const;
		size_t getSuccessorsSize() const;

		std::string getIdString() const;

		bool registerPredecessor(IProcessable::ePtr _newPredecessor);
		bool unregisterPredecessor(IProcessable::ePtr _newPredecessor);

		bool registerSuccessor(IProcessable::ePtr _newSuccessor);
		bool unregisterSuccessor(IProcessable::ePtr _newSuccessor);

		PlaceEnum getPlace() const;

		virtual unsigned getComputationTime() { return compTime; }

		virtual nPtr clone();

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		virtual DOMElement* serialize(DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey);
		virtual std::string getTypeString() const;
		virtual std::string get_shape() const;
		virtual std::string get_color() const;
	};

}
#pragma once
#include "IProcessable.h"

namespace msonlab
{
	using std::string;
	class Node : public IProcessable
	{
	private:
		IProcessable::eVect predecessors;
		IProcessable::eVect successors;

		IProcessable::nVect preNodes;
		IProcessable::nVect sucNodes;
		
		string type_string;
		int paramCount;
		unsigned compTime;

	public:
		Node(unsigned int _id, types::LabelType _label, types::DataPtr _value, string _type_string = "", unsigned _compTime = 1);
		Node(const Node& other);
		virtual ~Node() = default;
		virtual Node& operator=(const Node& other);

		bool registerParameter();

		virtual IProcessable::pVect process() = 0;
		virtual bool isReadyForProcess() const;
		virtual bool resetProcessingState();

		// getting the neighbours skipping the edges
		const IProcessable::nVect& getPreNodes() const { return preNodes; }
		const IProcessable::nVect& getSucNodes() const { return sucNodes; }

		const IProcessable::eVect& getPredecessors() const;
		size_t getPredecessorsSize() const;
		unsigned getPredecessorNodeId(size_t index) const;
		const IProcessable::ePtr& getPredecessor(size_t index) const;
		IProcessable::eVect::iterator getPredecessorBegin();
		IProcessable::eVect::iterator getPredecessorEnd();
		const IProcessable::eVect& getSuccessors() const;
		size_t getSuccessorsSize() const;

		std::string getIdString() const;

		void addPreNode(IProcessable::nPtr node);
		void addSucNode(IProcessable::nPtr node);

		bool registerPredecessor(IProcessable::ePtr _newPredecessor);
		bool unregisterPredecessor(IProcessable::ePtr _newPredecessor);

		bool registerSuccessor(IProcessable::ePtr _newSuccessor);
		bool unregisterSuccessor(IProcessable::ePtr _newSuccessor);

		PlaceEnum getPlace() const;

		virtual unsigned getComputationTime() const { return compTime; }

		virtual nPtr clone() = 0; // change to const

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		virtual DOMElement* serialize(DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey);
		virtual std::string getTypeString() const;
		virtual std::string get_shape() const;
		virtual std::string get_color() const;
	};

}
#ifndef NODE_H
#define NODE_H
#include "IProcessable.h"

namespace msonlab
{
	using std::weak_ptr;
	using std::string;

	class Node : public IProcessable
	{
	private:
		EdgeVect _predecessors;
		EdgeVect _successors;

		vector<weak_ptr<Node>> preNodes;
		NodeVect sucNodes;
		
		string type_string;
		int paramCount;
		unsigned compTime;

		void addPreNode(NodePtr node);
		void addSucNode(NodePtr node);

	public:
		Node(unsigned int id_, types::LabelType label_, types::DataPtr value_, string type_string_ = "", unsigned compTime_ = 1);
		Node(const Node& other);
		virtual ~Node() = default;
		virtual Node& operator=(const Node& other);

		bool registerParameter();

		virtual IProcessableVect process() = 0;
		virtual bool isReadyForProcess() const;
		virtual bool resetProcessingState();

		// getting the neighbours skipping the edges
		NodeVect predecessors() const;
		const NodeVect& successors() const { return sucNodes; }

		const EdgeVect& getPredecessors() const;
		size_t getPredecessorsSize() const;
		unsigned getPredecessorNodeId(size_t index) const;
		const EdgePtr& getPredecessor(size_t index) const;
		EdgeVect::iterator getPredecessorBegin();
		EdgeVect::iterator getPredecessorEnd();
		const EdgeVect& getSuccessors() const;
		size_t s_size() const;

		std::string getIdString() const;

		bool registerPredecessor(EdgePtr _newPredecessor);
		bool unregisterPredecessor(EdgePtr _newPredecessor);

		bool registerSuccessor(EdgePtr _newSuccessor);
		bool unregisterSuccessor(EdgePtr _newSuccessor);

		PlaceEnum getPlace() const;

		virtual unsigned cptime() const { return compTime; }

		virtual NodePtr clone() = 0; // change to const

		void releaseNeighbors();

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		//virtual DOMElement* serialize(DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey);
		virtual std::string getTypeString() const;
		virtual std::string get_shape() const;
		virtual std::string get_color() const;
	};

}
#endif
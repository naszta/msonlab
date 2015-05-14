#ifndef EDGE 
#define EDGE
#include "Global.h"
#include "IProcessable.h"

namespace msonlab
{
	class Edge : public IProcessable
	{
	public:
		enum EdgeTypeEnum { default_edge, blue_edge, red_edge, orange_edge };

	protected:
		//NodePtr from;
		//NodePtr to;
		std::weak_ptr<Node> from;
		std::weak_ptr<Node> to;

		bool paramReady;

		EdgeTypeEnum edgeType;
	public:
		Edge(unsigned int _id, types::LabelType _label, types::DataPtr _value, NodePtr _from, NodePtr _to);
		Edge(const Edge& other);
		Edge& operator=(const Edge& other);
		virtual ~Edge() = default;

		bool registerParameter();

		virtual IProcessableVect process();
		virtual bool isReadyForProcess() const;
		virtual bool resetProcessingState();

		NodePtr opposite(NodePtr x);

		NodePtr getFrom() const;
		unsigned getFromId() const;
		NodePtr getTo() const;
		unsigned getToId() const;

		std::string getIdString() const;

		virtual EdgeTypeEnum getEdgeType() const;

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		//virtual DOMElement* serialize(DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey);
		virtual std::string getTypeString() const;
		virtual std::string get_target_arrow_style() const;
		virtual std::string get_line_style() const;
		virtual std::string get_color() const;
	private:
		NodePtr lock_from() const { return from.lock(); }
		NodePtr lock_to() const { return to.lock(); }
	};
}
#endif
#ifndef GRAPH_EDGE_H 
#define GRAPH_EDGE_H

#include <string>

#include "../Global.h"
#include "../IProcessable.h"

namespace msonlab
{
	using std::string;

	using EdgeId = unsigned;

	// Descripes a node in terms of persistence and visualization
	class EdgeDescriptor
	{
	public:
		class EdgeDescriptor(string typeString, string color, string lineStyle, string targetArrowStyle) :
			_typeString(typeString), _color(color), _lineStyle(lineStyle), _targetArrowStyle(targetArrowStyle) {}
		string typeString() const { return _typeString; }
		string lineStyle() const { return _lineStyle; }
		string targetArrowStyle() const { return _targetArrowStyle; }
		string color() const { return _color; }
	private:
		string _color;
		string _typeString;
		string _targetArrowStyle;
		string _lineStyle;
	};

	class Edge : public IProcessable
	{
	public:
		enum EdgeTypeEnum { default_edge, blue_edge, red_edge, orange_edge };

	protected:
		std::weak_ptr<Node> from;
		std::weak_ptr<Node> to;

		bool paramReady;

		EdgeTypeEnum edgeType;
	public:
		Edge(unsigned int id_, types::LabelType label_, types::DataPtr value_, NodePtr from_, NodePtr to_, 
			EdgeDescriptor descriptor_ = EdgeDescriptor("SIMPLE", "#000000", "line", "standard"));
		Edge(const Edge& other);
		Edge& operator=(const Edge& other);
		virtual ~Edge() = default;

		const EdgeDescriptor& descriptor() const { return _descriptor; }

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

	private:
		NodePtr lock_from() const { return from.lock(); }
		NodePtr lock_to() const { return to.lock(); }
		EdgeDescriptor _descriptor;
	};
}
#endif
#pragma once
#include "Global.h"
#include "IProcessable.h"


namespace msonlab
{
	class Edge : public IProcessable
	{
	public:
		enum EdgeTypeEnum { default_edge, blue_edge, red_edge, orange_edge };

	protected:
		IProcessable::nPtr from;
		IProcessable::nPtr to;

		bool paramReady;

		EdgeTypeEnum edgeType;
	public:
		Edge(unsigned int _id, types::LabelType _label, types::DataPtr _value, IProcessable::nPtr _from, IProcessable::nPtr _to);
		Edge(const Edge& other);
		Edge& operator=(const Edge& other);

		bool registerParameter();

		virtual IProcessable::pVect process();
		virtual bool isReadyForProcess() const;
		virtual bool resetProcessingState();

		IProcessable::nPtr opposite(IProcessable::nPtr x);

		IProcessable::nPtr getFrom() const;
		unsigned getFromId() const;
		IProcessable::nPtr getTo() const;
		unsigned getToId() const;

		std::string getIdString() const;

		virtual EdgeTypeEnum getEdgeType() const;

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		virtual DOMElement* serialize(DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey);
		virtual std::string getTypeString() const;
		virtual std::string get_target_arrow_style() const;
		virtual std::string get_line_style() const;
		virtual std::string get_color() const;
	};

}
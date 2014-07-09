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
		Edge(unsigned int _id, Types::LabelType _label, Types::DataPtr _value, IProcessable::nPtr _from, IProcessable::nPtr _to);
		Edge(const Edge& other);

		bool registerParameter();

		virtual IProcessable::pVect process();
		virtual bool isReadyForProcess() const;
		virtual bool resetProcessingState();

		IProcessable::nPtr opposite(IProcessable::nPtr x);

		IProcessable::nPtr getFrom() const;
		unsigned getFromId() const;
		IProcessable::nPtr getTo() const;
		unsigned getToId() const;

		virtual EdgeTypeEnum getEdgeType() const;

		// compile
		virtual void compile(msonlab::StackRunner::srPtr stackProgram);
	};

}
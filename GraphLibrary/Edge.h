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
		//typedef boost::shared_ptr<Edge> ePtr;
		//typedef vector<boost::shared_ptr<Edge>> eVect;


		Edge(unsigned int _id, wchar_t _label, Types::DataType _value, IProcessable::nPtr _from, IProcessable::nPtr _to);
		Edge(const Edge& other);

		bool registerParameter();

		virtual IProcessable::pVect process();
		virtual bool isReadyForProcess() const;
		virtual bool resetProcessingState();

		IProcessable::nPtr opposite(IProcessable::nPtr x);

		IProcessable::nPtr getFrom() const;
		IProcessable::nPtr getTo() const;

		virtual EdgeTypeEnum getEdgeType() const;

		// compile
		virtual void compile(msonlab::StackRunner::srPtr stackProgram);
	};

}
#pragma once
#include "Global.h"
#include "IProcessable.h"


namespace msonlab
{
	class Edge : public IProcessable
	{
	protected:
		IProcessable::nPtr from;
		IProcessable::nPtr to;

		bool paramReady;
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

	};

}
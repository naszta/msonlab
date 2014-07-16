#pragma once
#include "Global.h"
#include "Edge.h"


namespace msonlab
{
	class BlueEdge : public Edge
	{
	public:
		BlueEdge(unsigned int _id, std::string _label, Types::DataType _value, IProcessable::nPtr _from, IProcessable::nPtr _to);

		virtual EdgeTypeEnum getEdgeType() const;

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string getTypeString() const;
		std::string get_color() const;
	};

}
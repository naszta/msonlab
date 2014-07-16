#pragma once
#include "Edge.h"

namespace msonlab
{
	class MultiplierEdge : public Edge
	{
	private:
		double multiplier;
	public:
		MultiplierEdge(unsigned int _id, std::string _label, Types::DataType _value, IProcessable::nPtr _from, IProcessable::nPtr _to, double _multiplier);

		virtual IProcessable::pVect process();

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string getTypeString() const;
		std::string get_target_arrow_style() const;
		std::string get_custom_data() const;
	};
}
#pragma once
#include "Edge.h"

namespace msonlab
{
	class MultiplierEdge : public Edge
	{
	private:
		Types::DataType multiplier;
	public:
		MultiplierEdge(unsigned int _id, Types::LabelType _label, Types::DataPtr _value, IProcessable::nPtr _from, IProcessable::nPtr _to, Types::DataType _multiplier);

		virtual IProcessable::pVect process();

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string getTypeString() const;
		std::string get_target_arrow_style() const;
		std::string get_custom_data() const;
	};
}
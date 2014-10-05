#pragma once
#include "Edge.h"

namespace msonlab
{
	class MultiplierEdge : public Edge
	{
	private:
		types::DataType multiplier;
	public:
		MultiplierEdge(unsigned int _id, types::LabelType _label, types::DataPtr _value, NodePtr _from, NodePtr _to, types::DataType _multiplier);

		virtual IProcessableVect process();

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string getTypeString() const;
		std::string get_target_arrow_style() const;
		std::string get_custom_data() const;
	};
}
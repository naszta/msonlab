#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeAdd : public Node
	{
	public:
		NodeAdd(unsigned int _id, types::LabelType _label, types::DataPtr _value);

		virtual IProcessable::pVect process();

		unsigned getComputationTime() { return 5; }

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string getTypeString() const;
		std::string get_color() const;
	};
}
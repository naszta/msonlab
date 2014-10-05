#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeAdd : public Node
	{
	public:
		NodeAdd(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		NodeAdd(const NodeAdd& other);

		NodeAdd& operator=(const NodeAdd& other);
		NodePtr clone();

		virtual IProcessableVect process();

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string get_color() const;
	};
}
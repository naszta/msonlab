#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeDivide : public Node
	{
	public:
		NodeDivide(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		NodeDivide(const NodeDivide& other);

		NodeDivide& operator=(const NodeDivide& other);
		NodePtr clone();
		virtual IProcessableVect process();

		//unsigned getComputationTime() { return 5; }

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string get_color() const;
	};
}
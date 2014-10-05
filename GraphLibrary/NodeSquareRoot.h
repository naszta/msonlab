#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeSquareRoot : public Node
	{
	public:
		NodeSquareRoot(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		NodeSquareRoot(const NodeSquareRoot& other);

		NodeSquareRoot& operator=(const NodeSquareRoot& other);
		NodePtr clone();
		virtual IProcessableVect process();

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string get_color() const;

		//unsigned getComputationTime() { return 7; }
	};
}